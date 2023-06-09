/* aes_keyschedule.h */

#ifndef AES_KEYSCHEDULE_H_
#define AES_KEYSCHEDULE_H_
#ifdef STM32F10X
#include "stm32f10x.h"
#else
#include "stdint.h"
#endif

#include "aes_types.h"
/**
 * \brief initialize the keyschedule
 *
 * This function computes the keyschedule from a given key with a given length
 * and stores it in the context variable
 * \param key       pointer to the key material
 * \param keysize_b length of the key in bits (valid are 128, 192 and 256)
 * \param ctx       pointer to the context where the keyschedule should be stored
 */
void aes_init(void* key, uint16_t keysize_b, aes_genctx_t* ctx);

/**
 * \brief initialize the keyschedule for 128 bit key
 *
 * This function computes the keyschedule from a given 128 bit key
 * and stores it in the context variable
 * \param key       pointer to the key material
 * \param ctx       pointer to the context where the keyschedule should be stored
 */
void aes128_init(void* key, aes128_ctx_t* ctx);


/**
 * \brief initialize the keyschedule for 192 bit key
 *
 * This function computes the keyschedule from a given 192 bit key
 * and stores it in the context variable
 * \param key       pointer to the key material
 * \param ctx       pointer to the context where the keyschedule should be stored
 */
void aes192_init(void* key, aes192_ctx_t* ctx);

/**
 * \brief initialize the keyschedule for 256 bit key
 *
 * This function computes the keyschedule from a given 256 bit key
 * and stores it in the context variable
 * \param key       pointer to the key material
 * \param ctx       pointer to the context where the keyschedule should be stored
 */
void aes256_init(void* key, aes256_ctx_t* ctx);

#endif /* AES_KEYSCHEDULE_H_ */

