#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "usb_flash.h"
#include "aes.h"
#include "aes_blocks_enc.h"
#include "stm32f10x_crc.h"
#include <stdlib.h>

#include "rsa_pkcs1v15.h"
#include "err_codes.h"
#include "mytrng.h"
#include "pins.h"

ErrorStatus HSEStartUpStatus;
EXTI_InitTypeDef EXTI_InitStructure;

extern uint8_t userPorts[2]; //Входы/выходы определенные пользователем.
extern __IO uint8_t PrevXferComplete;
extern tpSettings settings;
extern uint8_t AES_key[SESSION_KEY_LEN];
extern uint8_t cryptmess[CIPHER_SESSION_KEY_LEN];
uint8_t cryptkey[CIPHER_PRODUCT_KEY_LEN];
uint8_t Command[MaxMessageSize];
uint16_t MessLen;

uint8_t preallocated_buffer[PREALLOCATED_BUFFER_SIZE];

uint8_t entropy_data[32] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00
};

extern void aes128_block_enc(void* buffer, void* key, uint8_t size);

static void IntToUnicode(uint32_t value, uint8_t* pbuf, uint8_t len);

//Считает контрольную сумму.
uint32_t crc32_native(uint8_t* bfr, int len, int clear)
{
	int l;
	uint32_t* p, x, crc;
	l = len / 4;
	p = (uint32_t*)bfr;
	x = p[l];
	if (clear) CRC_ResetDR();
	while (l--)
	{
		crc = CRC_CalcCRC(__REV(*p++));
	}

	switch (len & 3)
	{
		case 1: crc = CRC_CalcCRC(__REV(x & 0x000000FF)); break;
		case 2: crc = CRC_CalcCRC(__REV(x & 0x0000FFFF)); break;
		case 3: crc = CRC_CalcCRC(__REV(x & 0x00FFFFFF)); break;
	}
	return __REV(0xFFFFFFFF ^ crc);
}

//Генерирует сессионный ключ.
void GenNSessionKey(void)
{
	//for (int i = 0; i < 16; ++i) AES_key[i] = rand();
	for (int i = 0; i < sizeof(AES_key); ++i) AES_key[i] = RndByte();
}

//Шифрует сообщение алгоритмом RSA.
int32_t RSA_Encrypt(RSApubKey_stt* P_pPubKey,
	const uint8_t* P_pInputMessage,
	int32_t P_InputSize,
	uint8_t* P_pOutput)
{
	int32_t status = RNG_SUCCESS;
	RNGstate_stt RNGstate;
	RNGinitInput_stt RNGinit_st;
	RNGinit_st.pmEntropyData = entropy_data;
	RNGinit_st.mEntropyDataSize = sizeof(entropy_data);
	RNGinit_st.mPersDataSize = 0;
	RNGinit_st.mNonceSize = 0;

	status = RNGinit(&RNGinit_st, &RNGstate);
	if (status == RNG_SUCCESS)
	{
		RSAinOut_stt inOut_st;
		membuf_stt mb;

		mb.mSize = sizeof(preallocated_buffer);
		mb.mUsed = 0;
		mb.pmBuf = preallocated_buffer;

		/* Fill the RSAinOut_stt */
		inOut_st.pmInput = P_pInputMessage;
		inOut_st.mInputSize = P_InputSize;
		inOut_st.pmOutput = P_pOutput;

		/* Encrypt the message, this function will write sizeof(modulus) data */
		status = RSA_PKCS1v15_Encrypt(P_pPubKey, &inOut_st, &RNGstate, &mb);
	}
	return(status);
}

void StartTransfer(uint16_t Len)
{
	MessLen = Len;
	for (int i = 0; i < sizeof(Command); ++i) Command[i] = 0;
	RHID_Send(TRANSPORT_START_PACKET_TRANSMISSION);
}

//Выполняет команду.
void RunCommand(void)
{
	uint8_t Buffer[7];
	uint16_t AnswLen;
	int i;
	const uint8_t PublicExponent[] =
	{
		0x01, 0x00, 0x01
	};

	switch (Command[0])
	{
	case GET_CRYPT_SERIAL_COMMAND:
		GenNSessionKey();

		FLASH_ReadSettings();

		RSApubKey_stt PubKey_st;

		PubKey_st.mExponentSize = sizeof(PublicExponent);
		PubKey_st.mModulusSize = sizeof(settings.crypt);
		PubKey_st.pmExponent = (uint8_t*)PublicExponent;
		PubKey_st.pmModulus = (uint8_t*)settings.crypt;

		RSA_Encrypt(&PubKey_st, AES_key, sizeof(AES_key), cryptmess);

		for (int i = 0; i < CIPHER_PRODUCT_KEY_LEN; ++i) cryptkey[i] = 0x00;
		for (int i = 0; i < SERIAL_NUM_LEN; ++i) cryptkey[i] = settings.Serial[i];
		aes128_block_enc(cryptkey, AES_key, SERIAL_NUM_LEN);

		Command[0] = GET_CRYPT_SERIAL_COMMAND;
		for (i = 0; i < CIPHER_SESSION_KEY_LEN; ++i) Command[i + 1] = cryptmess[i];
		for (i = 0; i < CIPHER_PRODUCT_KEY_LEN; ++i) Command[i + CIPHER_SESSION_KEY_LEN + 1] = cryptkey[i];
		AnswLen = 0x0121;
		break;

	case GET_HW_ID_COMMAND:
		RHIDCheckState();
		AnswLen = 0x000d;
		break;
	case SAVE_SERIAL_COMMAND:
		FLASH_ReadSettings();
		for (i = 0; i < SERIAL_NUM_LEN; ++i) settings.Serial[i] = Command[i + 1];
		FLASH_WriteSettings();
		AnswLen = 0x0000;
		break;

	case SAVE_RSA_KEY_COMMAND:
		FLASH_ReadSettings();
		for (i = 0; i < CIPHER_SESSION_KEY_LEN; ++i) settings.crypt[i] = Command[i + 1];
		FLASH_WriteSettings();
		AnswLen = 0x0000;
		break;

	case SAVE_DATE_COMMAND:
		SetLastDate(Command[1] << 8 | Command[2]);
		AnswLen = 0x0000;
		break;

	case GET_DATE_COMMAND:
		GetLastDate();
		AnswLen = 0x0003;
		break;

	case SET_LED_VALUE:
		userPorts[1] = Command[1];
		Command[0] = SET_LED_VALUE;
		Command[1] = userPorts[1];
		AnswLen = 0x0002;
		break;
	
	case INPUT_PORTS_STATUS:
		Command[0] = INPUT_PORTS_STATUS;
		Command[1] = userPorts[0];	
		AnswLen = 0x0002;
		break;
	
	default:
		AnswLen = 0x0000;
	}

	Buffer[0] = TRANSPORT_EXECUTE_COMMAND;
	Buffer[1] = (uint8_t)(AnswLen >> 8);
	Buffer[2] = (uint8_t)AnswLen;

	uint32_t crc = crc32_native(Buffer, 3, 1);
	Buffer[3] = (uint8_t)(crc >> 24);
	Buffer[4] = (uint8_t)(crc >> 16);
	Buffer[5] = (uint8_t)(crc >> 8);
	Buffer[6] = (uint8_t)crc;

	/* Reset the control token to inform upper layer that a transfer is ongoing */
	PrevXferComplete = 0;

	/* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
	USB_SIL_Write(EP1_IN, Buffer, 7);
	/* Enable endpoint for transmission */
	SetEPTxValid(ENDP1);
}

//Проверяет контрольную сумму сообщения.
bool CheckCRC(uint8_t* Buffer, uint8_t len)
{
	uint32_t crc = crc32_native(Buffer, len, 1);
	uint32_t buffcrc = Buffer[len] << 24 | Buffer[len + 1] << 16 | Buffer[len + 2] << 8 | Buffer[len + 3];
	if (crc != buffcrc)
	{
		RHID_Send(ANSWER_BAD_CRC);
		return FALSE;
	}
	return TRUE;
}

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power.
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_System(void)
{
	/* Enable the PWR clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_GPIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //Тактирование порта с кнопкой.
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //Тактирование порта со светодиодом.
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
	GPIO_InitTypeDef  ledPort; //Порт к которому подключен светодиод.
	GPIO_InitTypeDef sensorPort; //Порт к которому подключена кнопка.
	
	ledPort.GPIO_Mode = GPIO_Mode_Out_PP;
	ledPort.GPIO_Pin = GPIO_Pin_13;
	ledPort.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LED_PORT, &ledPort);

	//Внимание – не используйте выводы АЦП - PORTA(смотри схему платы).
	//Так как он инициализирован и там идет работа с выводами.
  GPIO_StructInit(&sensorPort);    
  sensorPort.GPIO_Mode = GPIO_Mode_IPU ; //Подтягивает к плюсу через внутренний резистор.
  sensorPort.GPIO_Pin = BUTTON_PIN;
  sensorPort.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(BUTTON_PORT, &sensorPort);
		
	Set_ADCConfig();

	/* Configure the EXTI line 18 connected internally to the USB IP ************/
  //  EXTI_ClearITPendingBit(EXTI_Line18);
  //  EXTI_InitStructure.EXTI_Line = EXTI_Line18;
  //  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  //  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  //  EXTI_Init(&EXTI_InitStructure);

	EXTI->IMR = 0x00040000;
	EXTI->EMR = 0x00000000;
	EXTI->RTSR = 0x00040000;
	EXTI->FTSR = 0x00000000;
}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz).
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/

void Set_ADCConfig(void)
{
	GPIO_InitTypeDef  port;
	port.GPIO_Mode = GPIO_Mode_AIN;
	port.GPIO_Pin = GPIO_Pin_1;        // that's ADC1 (PA1 on STM32)
	GPIO_Init(GPIOA, &port);

	ADC_InitTypeDef ADC_InitStructure;

	//Define ADC config
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  // we work in continuous sampling mode
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;

	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_28Cycles5); // define regular conversion config
	ADC_Init(ADC1, &ADC_InitStructure);   //set config of ADC1

	// enable ADC
	ADC_Cmd(ADC1, ENABLE);  //enable ADC1

	//  ADC calibration (optional, but recommended at power on)
	ADC_ResetCalibration(ADC1); // Reset previous calibration
	while (ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1); // Start new calibration (ADC must be off at that time)
	while (ADC_GetCalibrationStatus(ADC1));

	// start conversion
	ADC_Cmd(ADC1, ENABLE);  //enable ADC1
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); // start conversion (will be endless as we are in continuous mode)
}

void Set_USBClock(void)
{
	/* Select USBCLK source */
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

	/* Enable the USB clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode.
* Description    : Power-off system clocks and power while entering suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
	/* Set the device state to suspend */
	bDeviceState = SUSPENDED;

	/* Clear EXTI Line18 pending bit */
  //  EXTI_ClearITPendingBit(KEY_BUTTON_EXTI_LINE);

	/* Request to enter STOP mode with regulator in low power mode */
	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode.
* Description    : Restores system clocks and power while exiting suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
	DEVICE_INFO* pInfo = &Device_Info;

	/* Set the device state to the correct state */
	if (pInfo->Current_Configuration != 0)
	{
		/* Device configured */
		bDeviceState = CONFIGURED;
	}
	else
	{
		bDeviceState = ATTACHED;
	}

	/*Enable SystemCoreClock*/
	SystemInit();
}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config.
* Description    : Configures the USB interrupts.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 2 bit for pre-emption priority, 2 bits for subpriority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* Enable the USB interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the USB Wake-up interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USBWakeUp_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}


/*******************************************************************************
* Function Name  : USB_Cable_Config.
* Description    : Software Connection/Disconnection of USB Cable.
* Input          : NewState: new state.
* Output         : None.
* Return         : None
*******************************************************************************/
void USB_Cable_Config(FunctionalState NewState)
{
	if (NewState != DISABLE)
	{
		GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
	}
	else
	{
		GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
	}
}
/*******************************************************************************
* Function Name : RHIDCheckState.
* Description   : Decodes the RHID state.
* Input         : None.
* Output        : None.
* Return value  : The state value.
*******************************************************************************/
uint16_t btn1_prev, btn2_prev;
uint8_t Buffer[wMaxPacketSize + 1];
uint8_t RHIDCheckState(void)
{
	Command[0] = 0x3c;
	Command[1] = RHID_StringSerial[2];
	Command[2] = RHID_StringSerial[4];
	Command[3] = RHID_StringSerial[6];
	Command[4] = RHID_StringSerial[8];
	Command[5] = RHID_StringSerial[10];
	Command[6] = RHID_StringSerial[12];
	Command[7] = RHID_StringSerial[14];
	Command[8] = RHID_StringSerial[16];
	Command[9] = RHID_StringSerial[18];
	Command[10] = RHID_StringSerial[20];
	Command[11] = RHID_StringSerial[22];
	Command[12] = RHID_StringSerial[24];
	return 0;
}

//Генерируем сеансовый ключ.
void GenNewSeanceKey(void)
{
	GenNSessionKey();

	const uint8_t PublicExponent[] =
	{
	  0x01, 0x00, 0x01
	};

	FLASH_ReadSettings();

	RSApubKey_stt PubKey_st;

	PubKey_st.mExponentSize = sizeof(PublicExponent);
	PubKey_st.mModulusSize = sizeof(settings.crypt);
	PubKey_st.pmExponent = (uint8_t*)PublicExponent;
	PubKey_st.pmModulus = (uint8_t*)settings.crypt;

	RSA_Encrypt(&PubKey_st, AES_key, sizeof(AES_key), cryptmess);
	RHID_Send(HEAD_CRYPT_SESSION_KEY);
}

//Отправляет шифрованный ключ продукта.
void SendSeanceKeyBlock(uint8_t nBlock)
{
	Buffer[0] = HEAD_SEND_CRYPT_PRODUCT_KEY;

	for (int i = 0; i < CIPHER_PRODUCT_KEY_LEN; ++i) Buffer[i + 1] = cryptmess[CIPHER_PRODUCT_KEY_LEN * nBlock + i];

	uint32_t crc = crc32_native(Buffer, CIPHER_PRODUCT_KEY_LEN + 1, 1);

	Buffer[33] = (uint8_t)(crc >> 24);
	Buffer[34] = (uint8_t)(crc >> 16);
	Buffer[35] = (uint8_t)(crc >> 8);
	Buffer[36] = (uint8_t)crc;

	/* Reset the control token to inform upper layer that a transfer is ongoing */
	PrevXferComplete = 0;

	/* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
	USB_SIL_Write(EP1_IN, Buffer, CIPHER_PRODUCT_KEY_LEN + 5);

	/* Enable endpoint for transmission */
	SetEPTxValid(ENDP1);
}

/*******************************************************************************
* Function Name : RHID_Send.
* Description   : prepares buffer to be sent containing event infos.
* Input         : Keys: keys received from terminal.
* Output        : None.
* Return value  : None.
*******************************************************************************/
//void RHID_Send(uint8_t report, uint8_t state)
void RHID_Send(uint8_t report)
{
	//  uint8_t Buffer[2] = {0, 0};

	//  Buffer[0] = report;
	//  Buffer[1] = state;

	uint8_t Buffer[5] = { 0, 0, 0, 0, 0 };

	Buffer[0] = report;

	uint32_t crc = crc32_native(Buffer, 1, 1);

	Buffer[1] = (uint8_t)(crc >> 24);
	Buffer[2] = (uint8_t)(crc >> 16);
	Buffer[3] = (uint8_t)(crc >> 8);
	Buffer[4] = (uint8_t)crc;

	/* Reset the control token to inform upper layer that a transfer is ongoing */
	PrevXferComplete = 0;

	/* Copy buttons data in ENDP1 Tx Packet Memory Area*/
	USB_SIL_Write(EP1_IN, Buffer, 5);
	/* Enable endpoint for transmission */
	SetEPTxValid(ENDP1);
}

//Получить дату (в днях).
void GetLastDate(void)
{
	FLASH_ReadSettings();
	Command[0] = GET_DATE_COMMAND;
	uint16_t date = 0;
	if (settings.Date1 != 0xffff && settings.Date1 > date)  date = settings.Date1;
	if (settings.Date2 != 0xffff && settings.Date2 > date)  date = settings.Date2;
	if (settings.Date3 != 0xffff && settings.Date3 > date)  date = settings.Date3;

	Command[1] = (uint8_t)(date >> 8);
	Command[2] = (uint8_t)date;
}

//Задать дату (в днях).
void SetLastDate(uint16_t days)
{
	uint16_t dats[3];
	uint16_t a;
	bool isWrite = FALSE;
	FLASH_ReadSettings();
	if (settings.Date1 != 0xffff) dats[0] = settings.Date1; else dats[0] = 0x0000;
	if (settings.Date2 != 0xffff) dats[1] = settings.Date2; else dats[1] = 0x0000;
	if (settings.Date3 != 0xffff) dats[2] = settings.Date3; else dats[2] = 0x0000;

	bool isFind = FALSE;
	for (int i = 0; i < 3; ++i) if (dats[i] == days) isFind = TRUE;

	if (!isFind)
	{
		dats[2] = days;

		for (int i = 0; i < 2; ++i)
		{
			for (int j = 2; j < 3; ++j)
			{
				if (dats[i] < dats[j])
				{
					a = dats[i];
					dats[i] = dats[j];
					dats[j] = a;
				}
			}
		}

		settings.Date1 = dats[0];
		settings.Date2 = dats[1];
		settings.Date3 = dats[2];

		isWrite = TRUE;
	}

	if (isWrite) FLASH_WriteSettings();
	RHID_Send(SAVE_DATE_COMMAND);
}

/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_SerialNum(void)
{
	uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

	Device_Serial0 = *(uint32_t*)ID1;
	Device_Serial1 = *(uint32_t*)ID2;
	Device_Serial2 = *(uint32_t*)ID3;

	Device_Serial0 += Device_Serial2;

	if (Device_Serial0 != 0)
	{
		IntToUnicode(Device_Serial0, &RHID_StringSerial[2], 8);
		IntToUnicode(Device_Serial1, &RHID_StringSerial[18], 4);
	}
}

/*******************************************************************************
* Function Name  : HexToChar.
* Description    : Convert Hex 32Bits value into char.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
static void IntToUnicode(uint32_t value, uint8_t* pbuf, uint8_t len)
{
	uint8_t idx = 0;

	for (idx = 0; idx < len; idx++)
	{
		if (((value >> 28)) < 0xA)
		{
			pbuf[2 * idx] = (value >> 28) + '0';
		}
		else
		{
			pbuf[2 * idx] = (value >> 28) + 'A' - 10;
		}

		value = value << 4;

		pbuf[2 * idx + 1] = 0;
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
