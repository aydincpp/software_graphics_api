#include "graphics/color.h"
#include "utils/bit.h"

uint32_t
scale_channel (uint8_t value, int length)
{
  if (length <= 0)
    return 0;
  uint32_t max_value = (1u << length) - 1;
  return (uint32_t)((value / 255.0f) * max_value + 0.5f);
}

uint8_t
expand_channel (uint32_t value, int length)
{
  if (length <= 0)
    return 0;
  uint32_t max_value = (1u << length) - 1;
  return (uint8_t)((value * 255u) / max_value);
}

/* clang-format off */
uint32_t
pack_fb_color(Color8 c,
              int r_offset, int r_length,
              int g_offset, int g_length,
              int b_offset, int b_length,
              int a_offset, int a_length)
{
  uint32_t r = scale_channel(c.r, r_length) << r_offset;
  uint32_t g = scale_channel(c.g, g_length) << g_offset;
  uint32_t b = scale_channel(c.b, b_length) << b_offset;
  uint32_t a = scale_channel(c.a, a_length) << a_offset;

  return r | g | b | a;
}

Color8
unpack_fb_color(uint32_t value,
                int r_offset, int r_length,
                int g_offset, int g_length,
                int b_offset, int b_length,
                int a_offset, int a_length) {
  Color8 c;

  uint32_t r_raw = extract_bits_32(value, r_offset, r_length);
  uint32_t g_raw = extract_bits_32(value, g_offset, g_length);
  uint32_t b_raw = extract_bits_32(value, b_offset, b_length);
  uint32_t a_raw = extract_bits_32(value, a_offset, a_length);

  c.r = expand_channel(r_raw, r_length);
  c.g = expand_channel(g_raw, g_length);
  c.b = expand_channel(b_raw, b_length);
  c.a = expand_channel(a_raw, a_length);

  return c;
}

/* clang-format on */
