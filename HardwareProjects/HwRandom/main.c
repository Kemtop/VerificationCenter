#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

#include "sd.h"
#include "ff.h"

#include "string.h"

#define    DWT_CYCCNT    *(volatile unsigned long *)0xE0001004
#define    DWT_CONTROL   *(volatile unsigned long *)0xE0001000
#define    SCB_DEMCR     *(volatile unsigned long *)0xE000EDFC

void delay_ms(uint32_t ms)
{
	int32_t ms_count_tick =  ms * (SystemCoreClock/1000000);
	//разрешаем использовать счётчик
	SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
         //обнуляем значение счётного регистра
	DWT_CYCCNT  = 0;
        //запускаем счётчик
	DWT_CONTROL|= DWT_CTRL_CYCCNTENA_Msk; 
	while(DWT_CYCCNT < ms_count_tick);
        //останавливаем счётчик
	DWT_CONTROL &= ~DWT_CTRL_CYCCNTENA_Msk;
	
}

uint16_t RndData(void)
{
	uint16_t data = 0;
	
	for (int i=0; i<16; ++i)
	{
		delay_ms(200);
		data |= (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) << i);
	}
	
	//reset generator
//	GPIO_SetBits(GPIOB, GPIO_Pin_12);
//	GPIO_ResetBits(GPIOB, GPIO_Pin_12);

//	while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8))
//	{
//	}
//	data = 
//	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13))|
//	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) << 1)|
//	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) << 2)|
//	(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) << 3)|
//	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) << 4)|
//	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) << 5)|
//	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) << 6)|
//	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) << 7)|
//	(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15) << 8)|
//	(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10) << 9)|
//	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) << 10)|
//	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) << 11)|
//	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) << 12)|
//	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) << 13)|
//	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) << 14)|
//	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) << 15);
	return data;
}

void init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef  port;

	port.GPIO_Mode = GPIO_Mode_Out_PP;
	port.GPIO_Pin = GPIO_Pin_13;
  port.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &port);
	
	port.GPIO_Mode = GPIO_Mode_Out_PP;
	port.GPIO_Pin = GPIO_Pin_12;
  port.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &port);

	port.GPIO_Mode = GPIO_Mode_IPD;
	port.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_15 | GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12;
  port.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &port);

	port.GPIO_Mode = GPIO_Mode_IPD;
	port.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  port.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &port);
}

void initClock(void)
{
	    ErrorStatus HSEStartUpStatus;
 
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
    /* Системный RESET RCC (делать не обязательно, но полезно на этапе отладки) */
    RCC_DeInit();
 
    /* Включаем HSE (внешний кварц) */
    RCC_HSEConfig( RCC_HSE_ON);
 
    /* Ждем пока HSE будет готов */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
 
    /* Если с HSE все в порядке */
    if (HSEStartUpStatus == SUCCESS)
    {
        /* HCLK = SYSCLK */ /* Смотри на схеме AHB Prescaler. Частота не делится (RCC_SYSCLK_Div1) */
        RCC_HCLKConfig( RCC_SYSCLK_Div1);
 
        /* PCLK2 = HCLK */ /* Смотри на схеме APB2 Prescaler. Частота не делится (RCC_HCLK_Div1)  */
        RCC_PCLK2Config( RCC_HCLK_Div1);
 
        /* PCLK1 = HCLK/2 */ /* Смотри на схеме APB1 Prescaler. Частота делится на 2 (RCC_HCLK_Div2)
        потому что на выходе APB1 должно быть не более 36МГц (смотри схему) */
        RCC_PCLK1Config( RCC_HCLK_Div2);
 
        /* PLLCLK = 8MHz * 9 = 72 MHz */
        /* Указываем PLL от куда брать частоту (RCC_PLLSource_HSE_Div1) и на сколько ее умножать (RCC_PLLMul_9) */
        /* PLL может брать частоту с кварца как есть (RCC_PLLSource_HSE_Div1) или поделенную на 2 (RCC_PLLSource_HSE_Div2). Смотри схему */
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
 
        /* Включаем PLL */
        RCC_PLLCmd( ENABLE);
 
        /* Ждем пока PLL будет готов */
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
        }
 
        /* Переключаем системное тактирование на PLL */
        RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK);
 
        /* Ждем пока переключиться */
        while (RCC_GetSYSCLKSource() != 0x08)
        {
        }
    }
}


void delay(int cnt)
{
	const int delay = 25000;
	for (int i=0; i < cnt*delay; ++i);
}

void Blink(void)
{

	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	delay(1);
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
	delay(1);
}

void Blinks(int count)
{
	for (int i = 0; i < count; ++i ) Blink();
}


int main()
{
	uint16_t data, data1;
	FATFS fs;
  FRESULT res;
	
	initClock();
	init();
	
	res = (FRESULT)SD_init();
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
	res = f_mount(&fs, "", 1);
	FIL logFile;
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);

		while(1) 
		{
			res = f_open(&logFile, "log.bin", FA_WRITE | FA_OPEN_ALWAYS );
			FSIZE_t size = f_size(&logFile);
			res = f_lseek(&logFile, size);
			for(int i = 0; i < 512; ++i)
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);
				data1 = data = RndData();
				GPIO_SetBits(GPIOC, GPIO_Pin_13);

				res = f_printf(&logFile, "%c", data1 >> 8);
				res = f_printf(&logFile, "%c", data);
			}
			res = f_close(&logFile);
		}
}
