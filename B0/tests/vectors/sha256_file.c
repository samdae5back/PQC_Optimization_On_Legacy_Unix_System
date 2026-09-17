#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef struct {
  uint32_t h[8];
  uint64_t bits;
  unsigned char buffer[64];
  size_t used;
} sha256_ctx;

static const uint32_t k256[64] = {
  0x428a2f98U,0x71374491U,0xb5c0fbcfU,0xe9b5dba5U,0x3956c25bU,0x59f111f1U,0x923f82a4U,0xab1c5ed5U,
  0xd807aa98U,0x12835b01U,0x243185beU,0x550c7dc3U,0x72be5d74U,0x80deb1feU,0x9bdc06a7U,0xc19bf174U,
  0xe49b69c1U,0xefbe4786U,0x0fc19dc6U,0x240ca1ccU,0x2de92c6fU,0x4a7484aaU,0x5cb0a9dcU,0x76f988daU,
  0x983e5152U,0xa831c66dU,0xb00327c8U,0xbf597fc7U,0xc6e00bf3U,0xd5a79147U,0x06ca6351U,0x14292967U,
  0x27b70a85U,0x2e1b2138U,0x4d2c6dfcU,0x53380d13U,0x650a7354U,0x766a0abbU,0x81c2c92eU,0x92722c85U,
  0xa2bfe8a1U,0xa81a664bU,0xc24b8b70U,0xc76c51a3U,0xd192e819U,0xd6990624U,0xf40e3585U,0x106aa070U,
  0x19a4c116U,0x1e376c08U,0x2748774cU,0x34b0bcb5U,0x391c0cb3U,0x4ed8aa4aU,0x5b9cca4fU,0x682e6ff3U,
  0x748f82eeU,0x78a5636fU,0x84c87814U,0x8cc70208U,0x90befffaU,0xa4506cebU,0xbef9a3f7U,0xc67178f2U
};

static uint32_t rotr32(uint32_t x, unsigned int n)
{
  return (x >> n) | (x << (32 - n));
}

static uint32_t load_be32(const unsigned char *p)
{
  return ((uint32_t)p[0] << 24) |
         ((uint32_t)p[1] << 16) |
         ((uint32_t)p[2] << 8) |
         (uint32_t)p[3];
}

static void store_be32(unsigned char *p, uint32_t x)
{
  p[0] = (unsigned char)(x >> 24);
  p[1] = (unsigned char)(x >> 16);
  p[2] = (unsigned char)(x >> 8);
  p[3] = (unsigned char)x;
}

static void sha256_transform(sha256_ctx *ctx, const unsigned char block[64])
{
  uint32_t w[64];
  uint32_t a;
  uint32_t b;
  uint32_t c;
  uint32_t d;
  uint32_t e;
  uint32_t f;
  uint32_t g;
  uint32_t h;
  uint32_t s0;
  uint32_t s1;
  uint32_t ch;
  uint32_t maj;
  uint32_t t1;
  uint32_t t2;
  int i;

  for(i = 0; i < 16; i++)
    w[i] = load_be32(block + 4 * i);

  for(i = 16; i < 64; i++) {
    s0 = rotr32(w[i - 15], 7) ^ rotr32(w[i - 15], 18) ^ (w[i - 15] >> 3);
    s1 = rotr32(w[i - 2], 17) ^ rotr32(w[i - 2], 19) ^ (w[i - 2] >> 10);
    w[i] = w[i - 16] + s0 + w[i - 7] + s1;
  }

  a = ctx->h[0]; b = ctx->h[1]; c = ctx->h[2]; d = ctx->h[3];
  e = ctx->h[4]; f = ctx->h[5]; g = ctx->h[6]; h = ctx->h[7];

  for(i = 0; i < 64; i++) {
    s1 = rotr32(e, 6) ^ rotr32(e, 11) ^ rotr32(e, 25);
    ch = (e & f) ^ ((~e) & g);
    t1 = h + s1 + ch + k256[i] + w[i];
    s0 = rotr32(a, 2) ^ rotr32(a, 13) ^ rotr32(a, 22);
    maj = (a & b) ^ (a & c) ^ (b & c);
    t2 = s0 + maj;
    h = g; g = f; f = e; e = d + t1;
    d = c; c = b; b = a; a = t1 + t2;
  }

  ctx->h[0] += a; ctx->h[1] += b; ctx->h[2] += c; ctx->h[3] += d;
  ctx->h[4] += e; ctx->h[5] += f; ctx->h[6] += g; ctx->h[7] += h;
}

static void sha256_init(sha256_ctx *ctx)
{
  static const uint32_t iv[8] = {
    0x6a09e667U,0xbb67ae85U,0x3c6ef372U,0xa54ff53aU,
    0x510e527fU,0x9b05688cU,0x1f83d9abU,0x5be0cd19U
  };
  memcpy(ctx->h, iv, sizeof(iv));
  ctx->bits = 0;
  ctx->used = 0;
}

static void sha256_update(sha256_ctx *ctx, const unsigned char *data, size_t len)
{
  size_t take;

  ctx->bits += (uint64_t)len * 8U;
  while(len > 0) {
    take = 64 - ctx->used;
    if(take > len)
      take = len;
    memcpy(ctx->buffer + ctx->used, data, take);
    ctx->used += take;
    data += take;
    len -= take;
    if(ctx->used == 64) {
      sha256_transform(ctx, ctx->buffer);
      ctx->used = 0;
    }
  }
}

static void sha256_final(sha256_ctx *ctx, unsigned char out[32])
{
  uint64_t bits;
  int i;

  bits = ctx->bits;
  ctx->buffer[ctx->used++] = 0x80;
  if(ctx->used > 56) {
    while(ctx->used < 64)
      ctx->buffer[ctx->used++] = 0;
    sha256_transform(ctx, ctx->buffer);
    ctx->used = 0;
  }
  while(ctx->used < 56)
    ctx->buffer[ctx->used++] = 0;
  for(i = 7; i >= 0; i--)
    ctx->buffer[ctx->used++] = (unsigned char)(bits >> (8 * i));
  sha256_transform(ctx, ctx->buffer);
  for(i = 0; i < 8; i++)
    store_be32(out + 4 * i, ctx->h[i]);
}

int main(int argc, char **argv)
{
  FILE *fp;
  sha256_ctx ctx;
  unsigned char buffer[8192];
  unsigned char digest[32];
  size_t n;
  int i;

  if(argc != 2) {
    fprintf(stderr, "usage: %s FILE\n", argv[0]);
    return 2;
  }

  fp = fopen(argv[1], "rb");
  if(fp == NULL) {
    fprintf(stderr, "cannot open %s\n", argv[1]);
    return 2;
  }

  sha256_init(&ctx);
  while((n = fread(buffer, 1, sizeof(buffer), fp)) > 0)
    sha256_update(&ctx, buffer, n);
  if(ferror(fp)) {
    fclose(fp);
    return 2;
  }
  fclose(fp);

  sha256_final(&ctx, digest);
  for(i = 0; i < 32; i++)
    printf("%02x", digest[i]);
  printf("\n");
  return 0;
}
