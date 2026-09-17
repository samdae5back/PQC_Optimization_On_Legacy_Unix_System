#include <stdint.h>
#include "params.h"
#include "ntt.h"
#include "reduce.h"

/* Code to generate zetas and zetas_inv used in the number-theoretic transform:

#define KYBER_ROOT_OF_UNITY 17

static const uint8_t tree[128] = {
  0, 64, 32, 96, 16, 80, 48, 112, 8, 72, 40, 104, 24, 88, 56, 120,
  4, 68, 36, 100, 20, 84, 52, 116, 12, 76, 44, 108, 28, 92, 60, 124,
  2, 66, 34, 98, 18, 82, 50, 114, 10, 74, 42, 106, 26, 90, 58, 122,
  6, 70, 38, 102, 22, 86, 54, 118, 14, 78, 46, 110, 30, 94, 62, 126,
  1, 65, 33, 97, 17, 81, 49, 113, 9, 73, 41, 105, 25, 89, 57, 121,
  5, 69, 37, 101, 21, 85, 53, 117, 13, 77, 45, 109, 29, 93, 61, 125,
  3, 67, 35, 99, 19, 83, 51, 115, 11, 75, 43, 107, 27, 91, 59, 123,
  7, 71, 39, 103, 23, 87, 55, 119, 15, 79, 47, 111, 31, 95, 63, 127
};

void init_ntt() {
  unsigned int i;
  int16_t tmp[128];

  tmp[0] = MONT;
  for(i=1;i<128;i++)
    tmp[i] = fqmul(tmp[i-1],MONT*KYBER_ROOT_OF_UNITY % KYBER_Q);

  for(i=0;i<128;i++) {
    zetas[i] = tmp[tree[i]];
    if(zetas[i] > KYBER_Q/2)
      zetas[i] -= KYBER_Q;
    if(zetas[i] < -KYBER_Q/2)
      zetas[i] += KYBER_Q;
  }
}
*/

const int16_t zetas[128] = {
  -1044,  -758,  -359, -1517,  1493,  1422,   287,   202,
   -171,   622,  1577,   182,   962, -1202, -1474,  1468,
    573, -1325,   264,   383,  -829,  1458, -1602,  -130,
   -681,  1017,   732,   608, -1542,   411,  -205, -1571,
   1223,   652,  -552,  1015, -1293,  1491,  -282, -1544,
    516,    -8,  -320,  -666, -1618, -1162,   126,  1469,
   -853,   -90,  -271,   830,   107, -1421,  -247,  -951,
   -398,   961, -1508,  -725,   448, -1065,   677, -1275,
  -1103,   430,   555,   843, -1251,   871,  1550,   105,
    422,   587,   177,  -235,  -291,  -460,  1574,  1653,
   -246,   778,  1159,  -147,  -777,  1483,  -602,  1119,
  -1590,   644,  -872,   349,   418,   329,  -156,   -75,
    817,  1097,   603,   610,  1322, -1285, -1465,   384,
  -1215,  -136,  1218, -1335,  -874,   220, -1187, -1659,
  -1185, -1530, -1278,   794, -1510,  -854,  -870,   478,
   -108,  -308,   996,   991,   958, -1460,  1522,  1628
};

/* A: signed low 16 bits of zetas[i] * QINV, computed offline.
 * Indices depend only on the public NTT schedule, never on coefficients.
 * This adds 256 bytes of read-only precomputation; reduction timing is
 * unchanged. See README.md for the identity, bounds, and test commands.
 */
static const int16_t zetas_qinv[128] = {
     -20,  31498,  14745,    787,  13525, -12402,  28191, -16694,
  -20907,  27758,  -3799, -15690,  10690,   1358, -11202,  31164,
   -5827,  17363, -26360, -29057,   5571,  -1102,  21438, -26242,
  -28073,  24313, -10532,   8800,  18426,   8859,  26675, -16163,
   -5689,  -6516,   1496,  30967, -23565,  20179,  20710,  25080,
  -12796,  26616,  16064, -12442,   9134,   -650, -25986,  27837,
   19883, -28250, -15887,  -8898, -28309,   9075, -30199,  18249,
   13426,  14017, -29156, -12757,  16832,   4311, -24155, -17915,
    -335,  11182, -11477,  13387, -32227, -14233,  20494, -21655,
  -27738,  13131,    945,  -4587, -14883,  23092,   6182,   5493,
   32010, -32502,  10631,  30317,  29175, -18741, -28762,  12639,
  -18486,  20100,  17560,  18525, -14430,  19529,  -5276, -12619,
  -31183,  20297,  25435,   2146,  -7382,  15355,  24391, -32384,
  -20927,  -6280,  10946, -14903,  24214, -11044,  16989,  14469,
   10335, -21498,  -7934, -20198, -22502,  23210,  10906, -17442,
   31636, -23860,  28644, -20257,  23998,   7756, -17422,  23132
};

/*************************************************
* Name:        fqmul
*
* Description: Multiplication followed by Montgomery reduction
*
* Arguments:   - int16_t a: first factor
*              - int16_t b: second factor
*
* Returns 16-bit integer congruent to a*b*R^{-1} mod q
**************************************************/
static int16_t fqmul(int16_t a, int16_t b) {
  return montgomery_reduce((int32_t)a*b);
}

/* A: fixed-public-factor Montgomery multiplication. b_qinv is the
 * signed low 16 bits of b * QINV. Associativity modulo 2^16 makes t
 * identical to B0, but its multiplication is independent of a * b.
 * For the twiddles and b=1441, all intermediates fit int32_t for every
 * int16_t a. The narrowing and signed shift have B0's target assumptions.
 */
static int16_t fqmul_const(int16_t a, int16_t b, int16_t b_qinv)
{
  int16_t t;

  t = (int16_t)((int32_t)a * b_qinv);
  return (int16_t)(((int32_t)a * b - (int32_t)t * KYBER_Q) >> 16);
}

/*************************************************
* Name:        ntt
*
* Description: Inplace number-theoretic transform (NTT) in Rq.
*              input is in standard order, output is in bitreversed order
*
* Arguments:   - int16_t r[256]: pointer to input/output vector of elements of Zq
**************************************************/
void ntt(int16_t r[256]) {
  unsigned int len, start, j, k;
  int16_t t, zeta, zeta_qinv;

  k = 1;
  for(len = 128; len >= 2; len >>= 1) {
    for(start = 0; start < 256; start = j + len) {
      zeta = zetas[k];
      zeta_qinv = zetas_qinv[k++];
      for(j = start; j < start + len; j++) {
        t = fqmul_const(r[j + len], zeta, zeta_qinv);
        r[j + len] = r[j] - t;
        r[j] = r[j] + t;
      }
    }
  }
}

/*************************************************
* Name:        invntt_tomont
*
* Description: Inplace inverse number-theoretic transform in Rq and
*              multiplication by Montgomery factor 2^16.
*              Input is in bitreversed order, output is in standard order
*
* Arguments:   - int16_t r[256]: pointer to input/output vector of elements of Zq
**************************************************/
void invntt(int16_t r[256]) {
  unsigned int start, len, j, k;
  int16_t t, zeta, zeta_qinv;
  const int16_t f = 1441; /* mont^2/128 */

  k = 127;
  for(len = 2; len <= 128; len <<= 1) {
    for(start = 0; start < 256; start = j + len) {
      zeta = zetas[k];
      zeta_qinv = zetas_qinv[k--];
      for(j = start; j < start + len; j++) {
        t = r[j];
        r[j] = barrett_reduce(t + r[j + len]);
        r[j + len] = r[j + len] - t;
        r[j + len] = fqmul_const(r[j + len], zeta, zeta_qinv);
      }
    }
  }

  for(j = 0; j < 256; j++)
    r[j] = fqmul_const(r[j], f, -10079); /* low16(1441 * QINV) */
}

/*************************************************
* Name:        basemul
*
* Description: Multiplication of polynomials in Zq[X]/(X^2-zeta)
*              used for multiplication of elements in Rq in NTT domain
*
* Arguments:   - int16_t r[2]: pointer to the output polynomial
*              - const int16_t a[2]: pointer to the first factor
*              - const int16_t b[2]: pointer to the second factor
*              - int16_t zeta: integer defining the reduction polynomial
**************************************************/
void basemul(int16_t r[2], const int16_t a[2], const int16_t b[2], int16_t zeta)
{
  r[0]  = fqmul(a[1], b[1]);
  r[0]  = fqmul(r[0], zeta);
  r[0] += fqmul(a[0], b[0]);
  r[1]  = fqmul(a[0], b[1]);
  r[1] += fqmul(a[1], b[0]);
}
