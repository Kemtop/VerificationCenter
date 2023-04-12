#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

#include "platform_config.h"
#include "usb_type.h"

void Set_System(void);
void Set_USBClock(void);
void Set_ADCConfig(void);
void GPIO_AINConfig(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
void USB_Cable_Config (FunctionalState);

void StartTransfer(uint16_t Len);
void RunCommand(void);

void RHID_Send(uint8_t);
uint8_t RHIDCheckState(void);
void Get_SerialNum(void);
bool CheckCRC(uint8_t*, uint8_t);
void GetLastDate(void);
void SetLastDate(uint16_t);

#endif 

