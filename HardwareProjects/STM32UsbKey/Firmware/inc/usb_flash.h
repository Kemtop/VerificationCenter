#ifndef __USB_FLASH_H
#define __USB_FLASH_H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_flash.h"

typedef struct
  {
    char Serial[29];        // 29 byte
    uint16_t Date1;     // 2 byte
    uint16_t Date2;    // 2 byte
    uint16_t Date3;    // 2 byte
		uint8_t crypt[256]; //256 byte
		uint8_t nothing;	// 1 byte
                            // 292 byte = 73  32-bits words.  It's - OK
                            // !!! Full size (bytes) must be a multiple of 4 !!!
  } tpSettings;
 

void FLASH_Init(void);
void FLASH_ReadSettings(void);
void FLASH_WriteSettings(void);

#endif
