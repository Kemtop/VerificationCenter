/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

 #include "stm32f10x.h"

/*Unique Devices IDs register set*/

#define         ID1          (0x1FFFF7E8)
#define         ID2          (0x1FFFF7EC)
#define         ID3          (0x1FFFF7F0)

  #define RCC_GPIO RCC_APB2Periph_GPIOC
  #define LED_PORT GPIOC

  #define USB_DISCONNECT                      GPIOB
  #define USB_DISCONNECT_PIN                  GPIO_Pin_1

  #define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOD

  #define RCC_APB2Periph_ALLGPIO              (RCC_APB2Periph_GPIOA \
                                               | RCC_APB2Periph_GPIOB \
                                               | RCC_APB2Periph_GPIOC \
                                               | RCC_APB2Periph_GPIOD \
                                               | RCC_APB2Periph_GPIOE )

#endif /* __PLATFORM_CONFIG_H */



