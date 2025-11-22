#include "graphics/pixel.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

// Architecture detection
#if defined(__x86_64__) || defined(__i386__)
#define CPU_X86 1
#else
#define CPU_X86 0
#endif

#if defined(__arm__) || defined(__aarch64__)
#define CPU_ARM 1
#else
#define CPU_ARM 0
#endif

static inline bool
in_bounds (Framebuffer *fb, Vec2i_t pos)
{
  return (pos.x < 0 || pos.x >= (int)fb->vinfo.xres || pos.y < 0
          || pos.y >= (int)fb->vinfo.yres);
}

static inline size_t
byte_offset (Framebuffer *fb, Vec2i_t pos)
{
  int bytes = fb->vinfo.bits_per_pixel / 8;
  return pos.y * fb->finfo.line_length + pos.x * bytes;
}

void
set_pixel (Framebuffer *fb, Vec2i_t pos, Color8_t color)
{
  if (!in_bounds (fb, pos))
    return;

  // Calculate the byte offset in the framebuffer memory
  size_t offset = byte_offset (fb, pos);

  // Pointer to pixel position in the framebuffer memory
  uint8_t *ptr = fb->fbp + offset;

  // Pack color into a 32-bit unsigned integer
  // clang-format off
  uint32_t packed_color = pack_fb_color (
      color,
      fb->vinfo.red.offset,    fb->vinfo.red.length,
      fb->vinfo.green.offset,  fb->vinfo.green.length,
      fb->vinfo.blue.offset,   fb->vinfo.blue.length,
      fb->vinfo.transp.offset, fb->vinfo.transp.length);
  // clang-format on

  // Guess what
  int bytes_per_pixel = fb->vinfo.bits_per_pixel / 8;

  // Handle different bytes per pixel and write the pixel to framebuffer memory
  if (bytes_per_pixel == 4)
    {
// 32-bit framebuffer
#if CPU_X86
      *(uint32_t *)ptr = packed_color;
#else
      memcpy (ptr, &packed_color, 4);
#endif
    }
  else if (bytes_per_pixel == 3)
    {
      // 24-bit framebuffer
      ptr[0] = (packed_color) & 0xFF;
      ptr[1] = (packed_color >> 8) & 0xFF;
      ptr[2] = (packed_color >> 16) & 0xFF;
    }
  else if (bytes_per_pixel == 2)
    {
      // 16-bit framebuffer
      uint16_t color16 = (uint16_t)packed_color;
#if CPU_X86
      *(uint16_t *)ptr = color16;
#else
      memcpy (ptr, &color16, 2);
#endif
    }
  else
    {
      // Unsupported platform
      fprintf (stderr, "Unsupported platform\n");
      return;
    }
}

Color8_t
get_pixel (Framebuffer *fb, Vec2i_t pos)
{
  Color8_t c = { 0 };

  if (!in_bounds (fb, pos))
    return c;

  // Calculate the byte offset in the framebuffer memory
  size_t offset = byte_offset (fb, pos);

  // Pointer to pixel position in the framebuffer memory
  uint8_t *ptr = fb->fbp + offset;
  uint32_t raw = 0;

  // Guess what
  int bytes_per_pixel = fb->vinfo.bits_per_pixel / 8;

  // Handle different bytes per pixel and store pixel's bytes in raw
  if (bytes_per_pixel == 4)
    {
    // 32-bit framebuffer
#if CPU_X86
      raw = *(uint32_t *)ptr;
#else
      memcpy (&raw, ptr, 4);
#endif
    }
  else if (bytes_per_pixel == 3)
    {
      // 24-bit framebuffer
      raw = ptr[0] | (ptr[1] << 8) | (ptr[2] << 16);
    }
  else if (bytes_per_pixel == 2)
    {
    // 16-bit framebuffer
#if CPU_X86
      raw = *(uint16_t *)ptr;
#else
      memcpy (&raw, ptr, 2);
#endif
    }
  else
    {
      // Unsupported platform
      fprintf (stderr, "Unsupported platform\n");
      return c;
    }

  // Convert color bytes to Color8_t
  // clang-format off
  return unpack_fb_color(
    raw,
    fb->vinfo.red.offset,    fb->vinfo.red.length,
    fb->vinfo.green.offset,  fb->vinfo.green.length,
    fb->vinfo.blue.offset,   fb->vinfo.blue.length,
    fb->vinfo.transp.offset, fb->vinfo.transp.length
  );
  // clang-format on
}
