#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

typedef struct
{
  uint8_t r, g, b, a;
} Color8;

/* clang-format off */
uint32_t
scale_channel(uint8_t value, int length);

uint8_t
expand_channel(uint32_t value, int length);

uint32_t
pack_fb_color(Color8 c,
              int r_offset, int r_length,
              int g_offset, int g_length,
              int b_offset, int b_length,
              int a_offset, int a_length);

Color8
unpack_fb_color(uint32_t value,
                int r_offset, int r_length,
                int g_offset, int g_length,
                int b_offset, int b_length,
                int a_offset, int a_length);
/* clang-format on */

#endif /* COLOR_H */
