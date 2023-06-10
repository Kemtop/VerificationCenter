#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "usb_flash.h"
#include "hw_config.h"
#include "commands.h"
#include "transport_layer.h"

extern uint16_t MessLen;
extern uint8_t Command[MaxMessageSize];
extern uint8_t Buffer[wMaxPacketSize + 1];
extern uint32_t crc32_native(uint8_t *bfr, int len, int clear);
extern __IO uint8_t PrevXferComplete;

//Логика транспортного уровня.
void TransportLayerLogic(uint8_t* buffer)
{
	int i;
	int maxpdsize, packcount, pdsize;
	uint32_t crc;
	
	switch (buffer[0])
	{
		case TRANSPORT_START_PACKET_TRANSMISSION:
		if (CheckCRC(buffer, 3))
		{
			StartTransfer(buffer[1] << 8 | buffer[2]);
		}
		break;
		
		case TRANSPORT_SENDING_PACKAGE:
		maxpdsize = (wMaxPacketSize - 6);
		if (CheckCRC(buffer, maxpdsize + 2))
		{
			packcount = MessLen / maxpdsize + 1;
			if (buffer[1] == packcount)
			{
				pdsize = MessLen - buffer[1] * maxpdsize;
			}
			else
			{
				pdsize = maxpdsize;
			}

			for (i = 0; i < pdsize; ++i) Command[buffer[1] * maxpdsize + i] = buffer[i + 2];

			RHID_Send(TRANSPORT_SENDING_PACKAGE);
		}
		break;
		
		case TRANSPORT_EXECUTE_COMMAND:
			RunCommand();
		break;
	
		case TRANSPORT_GET_RESULT:
		maxpdsize = (wMaxPacketSize - 6);
		Buffer[0] = TRANSPORT_GET_RESULT;
		Buffer[1] = buffer[1];
	
		for (i = 0; i < maxpdsize; ++i)	Buffer[i + 2] = Command[buffer[1] * maxpdsize + i];
	
		crc = crc32_native(Buffer, maxpdsize + 2, 1);
		Buffer[maxpdsize + 2] = (uint8_t)(crc >> 24);
		Buffer[maxpdsize + 3] = (uint8_t)(crc >> 16);
		Buffer[maxpdsize + 4] = (uint8_t)(crc >> 8);
		Buffer[maxpdsize + 5] = (uint8_t)crc;

		/* Reset the control token to inform upper layer that a transfer is ongoing */
		PrevXferComplete = 0;

		/* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
		USB_SIL_Write(EP1_IN, Buffer, wMaxPacketSize);
		/* Enable endpoint for transmission */
		SetEPTxValid(ENDP1);

		break;
		
		default:
			RHID_Send(TRANSPORT_UNKNOWN);
		break;
	}
}
