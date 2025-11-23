#include "platform/framebuffer.h"
#include "graphics/color.h"
#include "graphics/pixel.h"
#include <stdbool.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

bool
fb_open (Framebuffer *fb, const char *path)
{
  fb->fd = open (path, O_RDWR);
  if (fb->fd < 0)
    {
      perror ("open");
      return false;
    }
  return true;
}

void
fb_close (Framebuffer *fb)
{
  if (fb->fbp)
    {
      fb_unmap (fb);
    }

  if (fb->fd >= 0)
    {
      close (fb->fd);
    }
}

bool
fb_get_info (Framebuffer *fb)
{
  if (ioctl (fb->fd, FBIOGET_VSCREENINFO, &fb->vinfo) < 0)
    {
      perror ("FBIOGET_VSCREENINFO");
      return false;
    }
  if (ioctl (fb->fd, FBIOGET_FSCREENINFO, &fb->finfo) < 0)
    {
      perror ("FBIOGET_FSCREENINFO");
      return false;
    }
  fb->screensize = fb->finfo.smem_len;
  return true;
}

bool
fb_put_info (Framebuffer *fb)
{
  if (ioctl (fb->fd, FBIOPUT_VSCREENINFO, &fb->vinfo) < 0)
    {
      perror ("FBIOPUT_VSCREENINFO");
      return false;
    }
  return fb_get_info (fb);
}

bool
fb_map (Framebuffer *fb)
{
  fb->fbp = mmap (0, fb->screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
                  fb->fd, 0);
  if (fb->fbp == MAP_FAILED)
    {
      perror ("mmap");
      fb->fbp = NULL;
      return false;
    }

  return true;
}

bool
fb_unmap (Framebuffer *fb)
{
  if (!fb->fbp)
    {
      return false;
    }

  munmap (fb->fbp, fb->screensize);
  fb->fbp = NULL;
  return true;
}

void
fb_clear_color (Framebuffer *fb, uint8_t r, uint8_t g, uint8_t b)
{
  int width = fb->vinfo.xres;
  int height = fb->vinfo.yres;
  int pixels = width * height;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      set_pixel(fb, (Vec2i_t){x, y}, (Color8_t){r, g, b, 0});
    }
  }
}
