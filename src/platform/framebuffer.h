#ifndef FB_H
#define FB_H

#include <linux/fb.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

typedef struct
{
  int fd;
  struct fb_fix_screeninfo finfo;
  struct fb_var_screeninfo vinfo;
  size_t screensize;
  uint8_t *fbp;
} Framebuffer;

/* clang-format off */
bool  fb_open         (Framebuffer *fb, const char *path);
void  fb_close        (Framebuffer *fb);
bool  fb_get_info     (Framebuffer *fb);
bool  fb_put_info     (Framebuffer *fb);
bool  fb_map          (Framebuffer *fb);
bool  fb_unmap        (Framebuffer *fb);
void  fb_clear_color  (Framebuffer *fb, uint8_t r, uint8_t g, uint8_t b);
/* clang-format on */

#endif /* FB_H */
