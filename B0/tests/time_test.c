#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "kem.h"

#ifndef TIME_ITERATIONS
#define TIME_ITERATIONS 1000
#endif

static double now_us(void)
{
  struct timeval tv;
  gettimeofday(&tv, 0);
  return (double)tv.tv_sec * 1000000.0 + (double)tv.tv_usec;
}

int main(void)
{
  unsigned int i;
  double t0, t1;
  double keypair_us, enc_us, dec_us;
  uint8_t pk[CRYPTO_PUBLICKEYBYTES];
  uint8_t sk[CRYPTO_SECRETKEYBYTES];
  uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
  uint8_t ss_enc[CRYPTO_BYTES];
  uint8_t ss_dec[CRYPTO_BYTES];

  if(crypto_kem_keypair(pk, sk) != 0)
    return 1;
  if(crypto_kem_enc(ct, ss_enc, pk) != 0)
    return 1;

  t0 = now_us();
  for(i = 0; i < TIME_ITERATIONS; i++)
    if(crypto_kem_keypair(pk, sk) != 0)
      return 1;
  t1 = now_us();
  keypair_us = (t1 - t0) / (double)TIME_ITERATIONS;

  t0 = now_us();
  for(i = 0; i < TIME_ITERATIONS; i++)
    if(crypto_kem_enc(ct, ss_enc, pk) != 0)
      return 1;
  t1 = now_us();
  enc_us = (t1 - t0) / (double)TIME_ITERATIONS;

  t0 = now_us();
  for(i = 0; i < TIME_ITERATIONS; i++)
    if(crypto_kem_dec(ss_dec, ct, sk) != 0)
      return 1;
  t1 = now_us();
  dec_us = (t1 - t0) / (double)TIME_ITERATIONS;

  if(memcmp(ss_enc, ss_dec, CRYPTO_BYTES) != 0) {
    fprintf(stderr, "timing test final shared-secret mismatch\n");
    return 1;
  }

  printf("algorithm=%s\n", CRYPTO_ALGNAME);
  printf("iterations=%u\n", (unsigned int)TIME_ITERATIONS);
  printf("keypair_us=%.3f\n", keypair_us);
  printf("encaps_us=%.3f\n", enc_us);
  printf("decaps_us=%.3f\n", dec_us);
  return 0;
}
