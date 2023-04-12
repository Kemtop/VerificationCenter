#include "mytrng.h"

uint8_t RndBit(void)
{
	uint16_t buf = 0;
	delay(1);
	buf = buf << 1 | (ADC_GetConversionValue(ADC1) & 1);
	return (buf&1)^(buf>>1&1)^(buf>>2&1)^(buf>>10&1);
}

uint8_t RndByte(void)
{
	uint8_t r = 0;
	for (uint8_t i = 0; i < 8; i++)
	{ 
		r = r << 1 | RndBit();
	}
	return r;
}

void delay(uint32_t time_delay)
{	
    uint32_t i;
    for(i = 0; i < time_delay; i++);
}
