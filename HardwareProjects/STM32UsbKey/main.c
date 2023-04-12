#include "hw_config.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "usb_flash.h"

__IO uint8_t PrevXferComplete = 1;

uint8_t AES_key[16] = { 0x48,0x65,0x6c,0x6c,0x6f,0x2c,0x20,0x77,0x6f,0x72,0x6c,0x64,0x21,0x21,0x21,0x21 };
uint8_t cryptmess[256];

int main(void)
{
	FLASH_Init();
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	
	Set_System();

  USB_Interrupts_Config();

  Set_USBClock();
	
  USB_Init();

	for (int i =0; i < sizeof(cryptmess); ++i) cryptmess[i] = 0x00;
	
	while(1)
	{
	}
}
