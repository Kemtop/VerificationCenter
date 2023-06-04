#include "aes_blocks_enc.h"

void aes128_block_enc(void* buffer, void* key, uint8_t size)
{
	aes128_ctx_t AES_ctx;
	aes128_init(key, &AES_ctx);
	uint8_t block[16];
	for (int bCount = 0; bCount < (size / 16 + (size % 16 ? 1 : 0)); ++bCount)
	{
		for (int i = 0; i < 16; ++i) block[i] = 0;
		for (int i = 0; i < (bCount * 16 < size ? 16 : (size % 16 ? size % 16 : 16)); ++i)
		{
			block[i] = ((uint8_t*)buffer)[16 * bCount + i];
		}
		aes128_enc(block, &AES_ctx);
		for (int i = 0; i < 16; ++i) ((uint8_t*)buffer)[16 * bCount + i] = block[i];
	}
}
