#ifndef BIT_H
#define BIT_H

#include <stdint.h>

uint32_t
extract_bits_32 (uint32_t value, int offset, int length)
{
  if (length <= 0)
    return 0;
  uint32_t mask = (1u << length) - 1u;
  return (value >> offset) & mask;
}

#endif /* BIT_H */
