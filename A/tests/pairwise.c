#include <stdio.h>
#include <string.h>
#include "kem.h"

#ifndef PAIRWISE_ITERATIONS
#define PAIRWISE_ITERATIONS 100
#endif

int main(void)
{
  unsigned int i;
  uint8_t pk[CRYPTO_PUBLICKEYBYTES];
  uint8_t sk[CRYPTO_SECRETKEYBYTES];
  uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
  uint8_t ss_enc[CRYPTO_BYTES];
  uint8_t ss_dec[CRYPTO_BYTES];

  for(i = 0; i < PAIRWISE_ITERATIONS; i++) {
    if(crypto_kem_keypair(pk, sk) != 0)
      return 1;
    if(crypto_kem_enc(ct, ss_enc, pk) != 0)
      return 1;
    if(crypto_kem_dec(ss_dec, ct, sk) != 0)
      return 1;
    if(memcmp(ss_enc, ss_dec, CRYPTO_BYTES) != 0) {
      fprintf(stderr, "pairwise mismatch at iteration %u\n", i);
      return 1;
    }
  }

  printf("PASS %s pairwise iterations=%u\n", CRYPTO_ALGNAME, (unsigned int)PAIRWISE_ITERATIONS);
  return 0;
}
