#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "aes256.h"
#include "rng.h"

AES256_CTR_DRBG_struct DRBG_ctx;

static void AES256_ECB(unsigned char *key,
                       unsigned char *ctr,
                       unsigned char *buffer)
{
    b0_aes256_encrypt_block(key, ctr, buffer);
}

int seedexpander_init(AES_XOF_struct *ctx,
                      unsigned char *seed,
                      unsigned char *diversifier,
                      unsigned long maxlen)
{
#if ULONG_MAX > 0xffffffffUL
    if(maxlen > 0xffffffffUL)
        return RNG_BAD_MAXLEN;
#endif

    ctx->length_remaining = maxlen;
    memcpy(ctx->key, seed, 32);
    memcpy(ctx->ctr, diversifier, 8);

    ctx->ctr[11] = (unsigned char)(maxlen % 256);
    maxlen >>= 8;
    ctx->ctr[10] = (unsigned char)(maxlen % 256);
    maxlen >>= 8;
    ctx->ctr[9] = (unsigned char)(maxlen % 256);
    maxlen >>= 8;
    ctx->ctr[8] = (unsigned char)(maxlen % 256);
    memset(ctx->ctr + 12, 0x00, 4);

    ctx->buffer_pos = 16;
    memset(ctx->buffer, 0x00, 16);

    return RNG_SUCCESS;
}

int seedexpander(AES_XOF_struct *ctx,
                 unsigned char *x,
                 unsigned long xlen)
{
    unsigned long offset;
    int i;

    if(x == NULL)
        return RNG_BAD_OUTBUF;
    if(xlen >= ctx->length_remaining)
        return RNG_BAD_REQ_LEN;

    ctx->length_remaining -= xlen;
    offset = 0;

    while(xlen > 0) {
        if(xlen <= (unsigned long)(16 - ctx->buffer_pos)) {
            memcpy(x + offset, ctx->buffer + ctx->buffer_pos, xlen);
            ctx->buffer_pos += (int)xlen;
            return RNG_SUCCESS;
        }

        memcpy(x + offset,
               ctx->buffer + ctx->buffer_pos,
               (size_t)(16 - ctx->buffer_pos));
        xlen -= (unsigned long)(16 - ctx->buffer_pos);
        offset += (unsigned long)(16 - ctx->buffer_pos);

        AES256_ECB(ctx->key, ctx->ctr, ctx->buffer);
        ctx->buffer_pos = 0;

        for(i = 15; i >= 12; i--) {
            if(ctx->ctr[i] == 0xff)
                ctx->ctr[i] = 0x00;
            else {
                ctx->ctr[i]++;
                break;
            }
        }
    }

    return RNG_SUCCESS;
}

void randombytes_init(unsigned char *entropy_input,
                      unsigned char *personalization_string,
                      int security_strength)
{
    unsigned char seed_material[48];
    int i;

    (void)security_strength;

    memcpy(seed_material, entropy_input, 48);
    if(personalization_string != NULL) {
        for(i = 0; i < 48; i++)
            seed_material[i] ^= personalization_string[i];
    }

    memset(DRBG_ctx.Key, 0x00, 32);
    memset(DRBG_ctx.V, 0x00, 16);
    AES256_CTR_DRBG_Update(seed_material, DRBG_ctx.Key, DRBG_ctx.V);
    DRBG_ctx.reseed_counter = 1;
}

int randombytes(unsigned char *x, unsigned long long xlen)
{
    unsigned char block[16];
    unsigned long long offset;
    int j;

    offset = 0;

    while(xlen > 0) {
        for(j = 15; j >= 0; j--) {
            if(DRBG_ctx.V[j] == 0xff)
                DRBG_ctx.V[j] = 0x00;
            else {
                DRBG_ctx.V[j]++;
                break;
            }
        }

        AES256_ECB(DRBG_ctx.Key, DRBG_ctx.V, block);

        if(xlen > 15) {
            memcpy(x + offset, block, 16);
            offset += 16;
            xlen -= 16;
        }
        else {
            memcpy(x + offset, block, (size_t)xlen);
            xlen = 0;
        }
    }

    AES256_CTR_DRBG_Update(NULL, DRBG_ctx.Key, DRBG_ctx.V);
    DRBG_ctx.reseed_counter++;

    return RNG_SUCCESS;
}

void AES256_CTR_DRBG_Update(unsigned char *provided_data,
                            unsigned char *Key,
                            unsigned char *V)
{
    unsigned char temp[48];
    int i;
    int j;

    for(i = 0; i < 3; i++) {
        for(j = 15; j >= 0; j--) {
            if(V[j] == 0xff)
                V[j] = 0x00;
            else {
                V[j]++;
                break;
            }
        }

        AES256_ECB(Key, V, temp + 16 * i);
    }

    if(provided_data != NULL) {
        for(i = 0; i < 48; i++)
            temp[i] ^= provided_data[i];
    }

    memcpy(Key, temp, 32);
    memcpy(V, temp + 32, 16);
}
