#include "aes.h"
#ifdef STM32F10X
#include "stm32f10x.h"
#else
#include "stdint.h"
#endif

void aes128_dec(void* buffer, aes128_ctx_t* ctx){
	aes_decrypt_core(buffer, (aes_genctx_t*)ctx, 10);
}

