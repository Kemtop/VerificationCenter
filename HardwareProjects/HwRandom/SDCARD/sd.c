#include "sd.h"

//глобальная переменная для определения типа карты 
uint8_t  SDHC;            
//макроопределения для управления выводом SS
#define CS_ENABLE         GPIOA->BSRR = GPIO_BSRR_BR4;         
#define CS_DISABLE    	  GPIOA->BSRR = GPIO_BSRR_BS4;  

//*********************************************************************************************
//function  обмен данными по SPI1                                                            //
//argument  передаваемый байт                                                                //
//return    принятый байт                                                                    //
//*********************************************************************************************
uint8_t spi_send (uint8_t data)
{ 
  while (!(SPI1->SR & SPI_SR_TXE));      //убедиться, что предыдущая передача завершена
  SPI1->DR = data;                       //загружаем данные для передачи
  while (!(SPI1->SR & SPI_SR_RXNE));     //ждем окончания обмена
  return (SPI1->DR);		         //читаем принятые данные
}

//*********************************************************************************************
//function  прием байт по SPI1                                                               //
//argument  none                                                                             //
//return    принятый байт                                                                    //
//*********************************************************************************************
uint8_t spi_read (void)
{ 
  return spi_send(0xff);		  //читаем принятые данные
}


//*********************************************************************************************
//function  инициализация  SPI1                                                              //
//argument  none                                                                             //
//return    none                                                                             //
//*********************************************************************************************
void spi_init(void)
{
  RCC->APB2ENR |=  RCC_APB2ENR_AFIOEN;//включить тактирование альтернативных функций
  RCC->APB2ENR |=  RCC_APB2ENR_IOPAEN;//включить тактирование порта А
 
  //вывод управления SS: выход двухтактный, общего назначения,50MHz
  GPIOA->CRL   |=  GPIO_CRL_MODE4;    //
  GPIOA->CRL   &= ~GPIO_CRL_CNF4;     //
  GPIOA->BSRR   =  GPIO_BSRR_BS4;     //
 
  //вывод SCK: выход двухтактный, альтернативная функция, 50MHz
  GPIOA->CRL   |=  GPIO_CRL_MODE5;    //
  GPIOA->CRL   &= ~GPIO_CRL_CNF5;     //
  GPIOA->CRL   |=  GPIO_CRL_CNF5_1;   //
 
  //вывод MISO: вход цифровой с подтягивающим резистором, подтяжка к плюсу
  GPIOA->CRL   &= ~GPIO_CRL_MODE6;    //
  GPIOA->CRL   &= ~GPIO_CRL_CNF6;     //
  GPIOA->CRL   |=  GPIO_CRL_CNF6_1;   //
  GPIOA->BSRR   =  GPIO_BSRR_BS6;     //
 
  //вывод MOSI: выход двухтактный, альтернативная функция, 50MHz
  GPIOA->CRL   |=  GPIO_CRL_MODE7;    //
  GPIOA->CRL   &= ~GPIO_CRL_CNF7;     //
  GPIOA->CRL   |=  GPIO_CRL_CNF7_1;   //
 
  //настроить модуль SPI
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; //подать тактирование
  SPI1->CR2     = 0x0000;             //
  SPI1->CR1     = SPI_CR1_MSTR;       //контроллер должен быть мастером,конечно    
  SPI1->CR1    |= SPI_CR1_BR_1; //SPI_CR1_BR;         //для начала зададим маленькую скорость
  SPI1->CR1    |= SPI_CR1_SSI;
  SPI1->CR1    |= SPI_CR1_SSM;
  SPI1->CR1    |= SPI_CR1_SPE;        //разрешить работу модуля SPI
}

//********************************************************************************************
//function	 посылка команды в SD                                		            //
//Arguments	 команда и ее аргумент                                                      //
//return	 0xff - нет ответа   			                                    //
//********************************************************************************************
uint8_t SD_sendCommand(uint8_t cmd, uint32_t arg)
{
  uint8_t response, wait=0, tmp;     
 
  //для карт памяти SD выполнить коррекцию адреса, т.к. для них адресация побайтная 
  if(SDHC == 0)		
  if(cmd == READ_SINGLE_BLOCK || cmd == WRITE_SINGLE_BLOCK )  {arg = arg << 9;}
  //для SDHC коррекцию адреса блока выполнять не нужно(постраничная адресация)	
 
  CS_ENABLE;
 
  //передать код команды и ее аргумент
  spi_send(cmd | 0x40);
  spi_send(arg>>24);
  spi_send(arg>>16);
  spi_send(arg>>8);
  spi_send(arg);
 
  //передать CRC (учитываем только для двух команд)
  if(cmd == SEND_IF_COND) spi_send(0x87);            
  else                    spi_send(0x95); 
 
  //ожидаем ответ
  while((response = spi_read()) == 0xff) 
   if(wait++ > 0xfe) break;                //таймаут, не получили ответ на команду
 
  //проверка ответа если посылалась команда READ_OCR
  if(response == 0x00 && cmd == 58)     
  {
    tmp = spi_read();                      //прочитать один байт регистра OCR            
    if(tmp & 0x40) SDHC = 1;               //обнаружена карта SDHC 
    else           SDHC = 0;               //обнаружена карта SD
    //прочитать три оставшихся байта регистра OCR
    spi_read(); 
    spi_read(); 
    spi_read(); 
  }
 
  spi_read();
 
  CS_DISABLE; 
 
  return response;
}

//********************************************************************************************
//function	 инициализация карты памяти                         			    //
//return	 0 - карта инициализирована  					            //
//********************************************************************************************
uint8_t SD_init(void)
{
  uint8_t   i;
  uint8_t   response;
  uint8_t   SD_version = 2;	          //по умолчанию версия SD = 2
  uint16_t  retry = 0 ;
 
  spi_init();                            //инициализировать модуль SPI                        
  for(i=0;i<10;i++) spi_send(0xff);      //послать свыше 74 единиц   
 
  //выполним программный сброс карты
  CS_ENABLE;
  while(SD_sendCommand(GO_IDLE_STATE, 0)!=0x01)                                   
    if(retry++>0x20)  return 1;                    
  CS_DISABLE;
  spi_send (0xff);
  spi_send (0xff);
 
  retry = 0;                                     
  while(SD_sendCommand(SEND_IF_COND,0x000001AA)!=0x01)
  { 
    if(retry++>0xfe) 
    { 
      SD_version = 1;
      break;
    } 
  }
 
 retry = 0;                                     
 do
 {
   response = SD_sendCommand(APP_CMD,0); 
   response = SD_sendCommand(SD_SEND_OP_COND,0x40000000);
   retry++;
   if(retry>0xffe) return 1;                     
 }while(response != 0x00);                      
 
 
 //читаем регистр OCR, чтобы определить тип карты
 retry = 0;
 SDHC = 0;
 if (SD_version == 2)
 { 
   while(SD_sendCommand(READ_OCR,0)!=0x00)
	 if(retry++>0xfe)  break;
 }
 
 return 0; 
}

//********************************************************************************************
//function	 чтение выбранного сектора SD                         			    //
//аrguments	 номер сектора,указатель на буфер размером 512 байт                         //
//return	 0 - сектор прочитан успешно   					            //
//********************************************************************************************
uint8_t SD_ReadSector(uint32_t BlockNumb,uint8_t *buff)
{ 
  uint16_t i=0;
 
  //послать команду "чтение одного блока" с указанием его номера
  if(SD_sendCommand(READ_SINGLE_BLOCK, BlockNumb)) return 1;  
  CS_ENABLE;
  //ожидание  маркера данных
  while(spi_read() != 0xfe)                
  if(i++ > 0xfffe) {CS_DISABLE; return 1;}       
 
  //чтение 512 байт	выбранного сектора
  for(i=0; i<512; i++) *buff++ = spi_read();
 
  spi_read(); 
  spi_read(); 
  spi_read(); 
 
  CS_DISABLE;
 
  return 0;
}


//********************************************************************************************
//function	 запись выбранного сектора SD                         			    //
//аrguments	 номер сектора, указатель на данные для записи                              //
//return	 0 - сектор записан успешно   					            //
//********************************************************************************************
uint8_t SD_WriteSector(uint32_t BlockNumb,const uint8_t *buff)
{
  uint8_t     response;
  uint16_t    i,wait=0;
 
  //послать команду "запись одного блока" с указанием его номера
  if( SD_sendCommand(WRITE_SINGLE_BLOCK, BlockNumb)) return 1;
 
  CS_ENABLE;
  spi_send(0xfe);    
 
  //записать буфер сектора в карту
  for(i=0; i<512; i++) spi_send(*buff++);
 
  spi_send(0xff);                //читаем 2 байта CRC без его проверки
  spi_send(0xff);
 
  response = spi_read();
 
  if( (response & 0x1f) != 0x05) //если ошибка при приеме данных картой
  { CS_DISABLE; return 1; }
 
  //ожидаем окончание записи блока данных
  while(!spi_read())             //пока карта занята,она выдает ноль
  if(wait++ > 0xfffe){CS_DISABLE; return 1;}
 
  CS_DISABLE;
  spi_send(0xff);   
  CS_ENABLE;         
 
  while(!spi_read())               //пока карта занята,она выдает ноль
  if(wait++ > 0xfffe){CS_DISABLE; return 1;}
  CS_DISABLE;
 
  return 0;
}

