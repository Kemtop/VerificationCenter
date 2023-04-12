#include "stm32f10x.h"                  // Device header

#define GO_IDLE_STATE 0 											//Программная перезагрузка
#define SEND_IF_COND 8 												//Для SDC V2 - проверка диапазона напряжений
#define READ_SINGLE_BLOCK 17 									//Чтение указанного блока данных
#define WRITE_SINGLE_BLOCK 24 								//Запись указанного блока данных
#define SD_SEND_OP_COND 41 										//Начало процесса инициализации
#define APP_CMD 55 														//Главная команда из ACMD команд
#define READ_OCR 58 													//Чтение регистра OCR

uint8_t SD_sendCommand(uint8_t cmd, uint32_t arg);
uint8_t SD_init(void);
uint8_t SD_ReadSector(uint32_t BlockNumb,uint8_t *buff);
uint8_t SD_WriteSector(uint32_t BlockNumb,const uint8_t *buff);
