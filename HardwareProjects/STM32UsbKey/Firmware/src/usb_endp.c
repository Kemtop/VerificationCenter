#include "hw_config.h"
#include "usb_desc.h"
#include "usb_lib.h"
#include "usb_istr.h"
#include "usb_flash.h"

uint8_t Receive_Buffer[wMaxPacketSize+1];
extern __IO uint8_t PrevXferComplete;
extern uint8_t Buffer[wMaxPacketSize+1];
extern uint8_t AES_key[];
extern tpSettings settings;
extern uint8_t Command[MaxMessageSize];
extern uint16_t MessLen;


extern uint32_t crc32_native(uint8_t *bfr, int len, int clear);
/*******************************************************************************
* Function Name  : EP1_OUT_Callback.
* Description    : EP1 OUT Callback Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_OUT_Callback(void)
{
	int i;
	int maxpdsize, packcount, pdsize;
	uint32_t crc;
	USB_SIL_Read(EP1_OUT, Receive_Buffer);
	
  switch (Receive_Buffer[0])
  {
		case 0xb0:
			if (CheckCRC(Receive_Buffer, 3))
			{
				StartTransfer(Receive_Buffer[1] <<8 | Receive_Buffer[2]);
			}
		break;
		case 0xb1:
			maxpdsize = (wMaxPacketSize - 6);
			if (CheckCRC(Receive_Buffer, maxpdsize+2))
			{
				packcount = MessLen / maxpdsize + 1;
				if (Receive_Buffer[1] == packcount)
				{
					pdsize = MessLen - Receive_Buffer[1] * maxpdsize;
				}
				else
				{
					pdsize = maxpdsize;
				}
				
				for (i = 0; i < pdsize; ++i) Command[Receive_Buffer[1]*maxpdsize + i] = Receive_Buffer[i+2];
				
				RHID_Send(0xb1);
			}
		break;
		case 0xb2:
			RunCommand();
		break;
		case 0xb3:
			maxpdsize = (wMaxPacketSize - 6);
			Buffer[0] = 0xb3;
			Buffer[1] = Receive_Buffer[1];
			for (i = 0; i < maxpdsize; ++i)	Buffer[i+2] = Command[Receive_Buffer[1]*maxpdsize+i];
			crc = crc32_native(Buffer,maxpdsize+2,1);
			Buffer[maxpdsize+2] = (uint8_t)(crc>>24);
			Buffer[maxpdsize+3] = (uint8_t)(crc>>16);
			Buffer[maxpdsize+4] = (uint8_t)(crc>>8);
			Buffer[maxpdsize+5] = (uint8_t) crc;
			
		  /* Reset the control token to inform upper layer that a transfer is ongoing */
			PrevXferComplete = 0;

			/* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
			USB_SIL_Write(EP1_IN, Buffer, wMaxPacketSize);
			/* Enable endpoint for transmission */
			SetEPTxValid(ENDP1);
		
		break;
		default:
			RHID_Send(0xf0);
			break;
  }
  SetEPRxStatus(ENDP1, EP_RX_VALID);
}

/*******************************************************************************
* Function Name  : EP1_OUT_Callback.
* Description    : EP1 OUT Callback Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_IN_Callback(void)
{
  PrevXferComplete = 1;
}
