#include "hw_config.h"
#include "usb_desc.h"
#include "usb_lib.h"
#include "usb_istr.h"
#include "transport_layer.h"

uint8_t Receive_Buffer[wMaxPacketSize + 1];
extern __IO uint8_t PrevXferComplete;

/*******************************************************************************
* Function Name  : EP1_OUT_Callback.
* Description    : EP1 OUT Callback Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_OUT_Callback(void)
{
	USB_SIL_Read(EP1_OUT, Receive_Buffer);
	TransportLayerLogic(Receive_Buffer);  
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
