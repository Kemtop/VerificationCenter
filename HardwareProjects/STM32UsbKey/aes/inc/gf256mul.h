#ifndef G256_H_
#define G256_H_

#ifdef STM32F10X
#include "stm32f10x.h"
#else
#include "stdint.h"
#endif

uint8_t gf256mul(uint8_t a, uint8_t b, uint8_t reducer);

#endif
