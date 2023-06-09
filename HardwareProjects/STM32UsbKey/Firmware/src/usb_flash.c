#include "usb_flash.h"

// ====================================================
// FLASH Settings struct
// ====================================================
#define MY_FLASH_PAGE_ADDR 0x800FC00

tpSettings settings;
 
#define SETTINGS_WORDS sizeof(settings)/4

void FLASH_Init(void) {
    /* Next commands may be used in SysClock initialization function
       In this case using of FLASH_Init is not obligatorily */
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd( FLASH_PrefetchBuffer_Enable);
    /* Flash 2 wait state */
    FLASH_SetLatency( FLASH_Latency_2);
}

void FLASH_ReadSettings(void) {
    //Read settings
    uint32_t *source_addr = (uint32_t *)MY_FLASH_PAGE_ADDR;
    uint32_t *dest_addr = (void *)&settings;
    for (uint16_t i=0; i<SETTINGS_WORDS; i++) {
        *dest_addr = *(__IO uint32_t*)source_addr;
        source_addr++;
        dest_addr++;
    }
}

void FLASH_WriteSettings(void) {
    FLASH_Unlock();
    FLASH_ErasePage(MY_FLASH_PAGE_ADDR);
 
    // Write settings
    uint32_t *source_addr = (void *)&settings;
    uint32_t *dest_addr = (uint32_t *) MY_FLASH_PAGE_ADDR;
    for (uint16_t i=0; i<SETTINGS_WORDS; i++) {
        FLASH_ProgramWord((uint32_t)dest_addr, *source_addr);
        source_addr++;
        dest_addr++;
    }
 
    FLASH_Lock();
}
