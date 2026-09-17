/* Arithmetic-only regression: compile the real B0 and candidate sources.
 * Deliberate source inclusion exposes A's private constant-multiply helper
 * without changing the library API or the benchmarked build.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../ntt.h"
#include "../reduce.h"

#undef zetas
#undef ntt
#undef invntt
#undef basemul
#define zetas b0_zetas
#define ntt b0_ntt
#define invntt b0_invntt
#define basemul b0_basemul
#define fqmul b0_fqmul
#include "../../B0/ntt.c"
#undef zetas
#undef ntt
#undef invntt
#undef basemul
#undef fqmul
#define zetas KYBER_NAMESPACE(zetas)
#define ntt KYBER_NAMESPACE(ntt)
#define invntt KYBER_NAMESPACE(invntt)
#define basemul KYBER_NAMESPACE(basemul)
#include "../ntt.c"

static uint32_t state = 0x4b594245UL;

static uint32_t next_u32(void)
{
  state ^= state << 13;
  state ^= state >> 17;
  state ^= state << 5;
  return state;
}

static int16_t sample(void)
{
  return (int16_t)((int32_t)(next_u32() % 8191U) - 4095);
}

static int32_t modq(int32_t a)
{
  int32_t r;
  r = a % KYBER_Q;
  return r < 0 ? r + KYBER_Q : r;
}

static int32_t mulmod(int32_t a, int32_t b)
{
  return modq(modq(a) * modq(b));
}

static void require(int ok, const char *message)
{
  if(!ok) {
    fprintf(stderr, "FAIL K=%d: %s\n", KYBER_K, message);
    exit(1);
  }
}

#ifdef TEST_A
static void check_constants(void)
{
  unsigned int i;
  int32_t a;
  int16_t c, ci;

  for(i = 0; i < 129; i++) {
    c = i < 128 ? zetas[i] : 1441;
    ci = i < 128 ? zetas_qinv[i] : -10079;
    require(ci == (int16_t)((int32_t)c * QINV), "precomputed constant");
    for(a = -32768; a <= 32767; a++)
      require(fqmul_const((int16_t)a, c, ci) == b0_fqmul((int16_t)a, c),
              "constant multiply must be bit-exact with B0");
  }
  puts("PASS 8454144 fixed-factor products (all int16_t inputs)");
}
#endif

static void check_pair(const int16_t a[2], const int16_t b[2], int16_t z)
{
  int16_t r[2], ref[2], t;
  int32_t acc0, acc1, p11, want0, want1;

  basemul(r, a, b, z);
  b0_basemul(ref, a, b, z);
  t = b0_fqmul(a[1], b[1]);
  acc0 = (int32_t)a[0] * b[0] + (int32_t)t * z;
  acc1 = (int32_t)a[0] * b[1] + (int32_t)a[1] * b[0];
  require(t >= -1921 && t <= 1921, "first REDC range");
  require(acc0 >= -19965569L && acc0 <= 19965569L, "acc0 range");
  require(acc1 >= -33538050L && acc1 <= 33538050L, "acc1 range");
  /* Independent modular oracle: 169 * 65536 == 1 (mod 3329). */
  p11 = mulmod(mulmod(a[1], b[1]), 169);
  want0 = modq(mulmod(mulmod(p11, z), 169) +
               mulmod(mulmod(a[0], b[0]), 169));
  want1 = mulmod(modq(mulmod(a[0], b[1]) + mulmod(a[1], b[0])), 169);
  require(modq(r[0]) == want0 && modq(r[1]) == want1, "basemul oracle");
  require(barrett_reduce(r[0]) == barrett_reduce(ref[0]) &&
          barrett_reduce(r[1]) == barrett_reduce(ref[1]), "basemul vs B0");
#ifdef TEST_A
  require(r[0] == ref[0] && r[1] == ref[1], "A leaves basemul unchanged");
#else
  require(r[0] >= -2177 && r[0] <= 2177 &&
          r[1] >= -2177 && r[1] <= 2177, "batched output range");
#endif
}

static void check_basemul(void)
{
  static const int16_t edge[9] = {-4095, -3329, -1664, -1, 0,
                                 1, 1664, 3329, 4095};
  unsigned int zi, i, j, k, l;
  int16_t a[2], b[2], z;

  for(zi = 0; zi < 131; zi++) {
    if(zi < 128)
      z = zi < 64 ? zetas[64 + zi] : -zetas[zi];
    else
      z = zi == 128 ? -1664 : (zi == 129 ? 0 : 1664);
    for(i = 0; i < 9; i++)
      for(j = 0; j < 9; j++)
        for(k = 0; k < 9; k++)
          for(l = 0; l < 9; l++) {
            a[0] = edge[i]; a[1] = edge[j];
            b[0] = edge[k]; b[1] = edge[l];
            check_pair(a, b, z);
          }
  }
  for(i = 0; i < 100000; i++) {
    a[0] = sample(); a[1] = sample();
    b[0] = sample(); b[1] = sample();
    z = (int16_t)((int32_t)(next_u32() % 3329U) - 1664);
    check_pair(a, b, z);
  }
  puts("PASS 859491 boundary tuples and 100000 deterministic random basemuls");
}

static void check_transforms(void)
{
  unsigned int n, i;
  int16_t a[256], b[256], original[256];
  static const int16_t edge[9] = {-4095, -3329, -1664, -1, 0,
                                 1, 1664, 3329, 4095};

  for(n = 0; n < 1024; n++) {
    for(i = 0; i < 256; i++) {
      a[i] = n < 9 ? edge[n] : sample();
      b[i] = a[i]; original[i] = a[i];
    }
    ntt(a); b0_ntt(b);
    for(i = 0; i < 256; i++) {
      require(a[i] == b[i], "raw forward NTT vs B0");
      a[i] = barrett_reduce(a[i]);
      b[i] = barrett_reduce(b[i]);
    }
    invntt(a); b0_invntt(b);
    for(i = 0; i < 256; i++) {
      require(a[i] == b[i], "inverse NTT vs B0");
      require(modq(a[i]) == mulmod(original[i], 2285), "NTT round trip");
      a[i] = n < 9 ? edge[n] : sample();
      b[i] = a[i];
    }
    invntt(a); b0_invntt(b);
    for(i = 0; i < 256; i++)
      require(a[i] == b[i], "direct inverse NTT vs B0");
  }
  puts("PASS 1024 forward/round-trip and 1024 direct inverse NTT cases");
}

static void check_dot_products(void)
{
  unsigned int n, k, j;
  int16_t a[2], b[2], r[2], ref[2], z;
  int32_t acc[2], refacc[2];

  for(n = 0; n < 10000; n++) {
    z = zetas[64 + (n % 64)];
    if(n & 1) z = -z;
    acc[0] = acc[1] = refacc[0] = refacc[1] = 0;
    for(k = 0; k < KYBER_K; k++) {
      for(j = 0; j < 2; j++) {
        a[j] = sample(); b[j] = sample();
      }
      basemul(r, a, b, z); b0_basemul(ref, a, b, z);
      for(j = 0; j < 2; j++) {
        acc[j] += r[j]; refacc[j] += ref[j];
        require(acc[j] >= -32768 && acc[j] <= 32767,
                "existing int16_t vector accumulation range");
        require(refacc[j] >= -32768 && refacc[j] <= 32767,
                "B0 vector accumulation range");
      }
    }
    for(j = 0; j < 2; j++)
      require(barrett_reduce((int16_t)acc[j]) ==
              barrett_reduce((int16_t)refacc[j]), "normalized vector dot product");
  }
  puts("PASS 10000 KYBER_K-term normalized dot products");
}

static void check_polynomial_products(void)
{
  unsigned int n, i, j, d;
  int16_t a[256], b[256], x[256], y[256], r[256];
  int32_t want[256], term;

  for(n = 0; n < 32; n++) {
    for(i = 0; i < 256; i++) {
      a[i] = n == 0 ? 0 : (n == 1 ? (i == 0) : sample());
      b[i] = sample();
      x[i] = a[i]; y[i] = b[i]; want[i] = 0;
    }
    ntt(x); ntt(y);
    for(i = 0; i < 256; i++) {
      x[i] = barrett_reduce(x[i]); y[i] = barrett_reduce(y[i]);
    }
    for(i = 0; i < 64; i++) {
      basemul(r + 4*i, x + 4*i, y + 4*i, zetas[64+i]);
      basemul(r + 4*i+2, x + 4*i+2, y + 4*i+2, -zetas[64+i]);
    }
    for(i = 0; i < 256; i++) r[i] = barrett_reduce(r[i]);
    invntt(r);
    for(i = 0; i < 256; i++)
      for(j = 0; j < 256; j++) {
        d = i + j;
        term = (int32_t)a[i] * b[j];
        if(d >= 256) { d -= 256; term = -term; }
        want[d] = modq(want[d] + term);
      }
    for(i = 0; i < 256; i++)
      require(modq(r[i]) == want[i], "negacyclic schoolbook product oracle");
  }
  puts("PASS 32 complete polynomial products vs schoolbook oracle");
}

int main(void)
{
  require((int16_t)32768L == -32768 && (-1 >> 1) == -1,
          "B0 signed narrowing/arithmetic-shift target assumptions");
#ifdef TEST_A
  check_constants();
#endif
  check_basemul();
  check_transforms();
  check_dot_products();
  check_polynomial_products();
  printf("PASS all arithmetic checks for KYBER_K=%d\n", KYBER_K);
  return 0;
}
