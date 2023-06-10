#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

#define SESSION_KEY_LEN 16 //Длина сеансового ключа.
#define CIPHER_SESSION_KEY_LEN 256 //Длина шифрованного rsa сеансового ключа.
#define CIPHER_PRODUCT_KEY_LEN 32 //Размер блока с зашифрованным ключем продукта.
#define PREALLOCATED_BUFFER_SIZE 4096
#define SERIAL_NUM_LEN 29 //Длина серийного номера (ключа продукта).

#include "commands.h"
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
bool CheckCRC(uint8_t*, uint8_t); //Проверяет контрольную сумму сообщения.
void GetLastDate(void); //Получить дату (в днях).
void SetLastDate(uint16_t); //Задать дату (в днях).
void SendSeanceKeyBlock(uint8_t nBlock); //Отправляет шифрованный ключ продукта.

#endif 

