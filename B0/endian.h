#ifndef PQC_LEGACY_ENDIAN_H
#define PQC_LEGACY_ENDIAN_H

#include <stdint.h>

/*
 * Explicit little-endian loads for byte-oriented ML-KEM inputs.
 * These helpers avoid dependence on host byte order and alignment.
 */
static uint32_t load24_le(const uint8_t x[3])
{
  return ((uint32_t)x[0]) |
         ((uint32_t)x[1] << 8) |
         ((uint32_t)x[2] << 16);
}

static uint32_t load32_le(const uint8_t x[4])
{
  return ((uint32_t)x[0]) |
         ((uint32_t)x[1] << 8) |
         ((uint32_t)x[2] << 16) |
         ((uint32_t)x[3] << 24);
}

#endif
