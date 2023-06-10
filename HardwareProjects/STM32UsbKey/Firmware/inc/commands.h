//Идентификаторы команд, ответов и заголовков.
#define GET_HW_ID_COMMAND 0x3C //Идентификатор команды «Получить серийный номер устройства».
#define SAVE_SERIAL_COMMAND 0xa0 //Сохранить серийный номер продукта.
#define SAVE_DATE_COMMAND 0x15 //Сохранить дату (в днях).
#define GET_DATE_COMMAND 0x17 //Получить дату (в днях).
#define SAVE_RSA_KEY_COMMAND 0xa9 //Сохранить модуль n открытого ключа rsa (256 байт).
#define GET_CRYPT_SERIAL_COMMAND 0x07 //Получить шифрованный ключом серийный номер продукта.
#define GET_RANDOM_COMMAND 0x50 //Получить случайное число.
#define SET_LED_VALUE 0x01 //Установить значение порту светодиода на плате.
#define INPUT_PORTS_STATUS 0x02 //Получить состояние входов на плате.

#define TRANSPORT_START_PACKET_TRANSMISSION 0xb0 //Начать пакетную передачу.
#define TRANSPORT_SENDING_PACKAGE 0xb1 //Отправка пакета.
#define TRANSPORT_EXECUTE_COMMAND 0xb2 //Обработать сообщение на стороне usb устройства.
#define TRANSPORT_GET_RESULT 0xb3 //Получить ответ от USB устройства. Ответ пакетный.
#define TRANSPORT_UNKNOWN 0xf0 //Неизвестная команда


#define ANSWER_BAD_CRC 0xFF //Неверная CRC.

#define HEAD_CRYPT_SESSION_KEY 0xa3 //Шифрованный rsa сеансовый ключ.
#define HEAD_SEND_CRYPT_PRODUCT_KEY 0xa5 //Шифрованный ключ продукта.
