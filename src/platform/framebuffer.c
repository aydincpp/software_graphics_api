#include "platform/framebuffer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

bool
fb_init (Framebuffer* fb,
         const char* path) {
  if (!fb_open(fb, path)) return false;
  if (!fb_get_info(fb))   return false;
  if (!fb_map(fb))        return false;

  fb->size = fb->vinfo.xres * fb->vinfo.yres * (fb->vinfo.bits_per_pixel / 8);

  uint8_t *back_buffer = (uint8_t*)malloc(fb->size);
  if (!back_buffer) { return false; }
  fb->back_buffer = back_buffer;

  float *depth_buffer = (float*)malloc(fb->size);
  if (!depth_buffer) { return false; }
  fb->depth_buffer = depth_buffer;

  fb->aspect = (float)fb->vinfo.xres / fb->vinfo.yres;
  if (!fb->back_buffer) return false;

  return true;
}

void
fb_shutdown(Framebuffer* fb) {
  if (!fb) return;

  if (fb->back_buffer) {
    free(fb->back_buffer);
  }

  if (fb->depth_buffer) {
    free(fb->depth_buffer);
  }

  fb_close(fb);
}

void
fb_clear (Framebuffer* fb)
{
  memset(fb->back_buffer, 0, fb->size);

  size_t n = (float)fb->vinfo.xres * fb->vinfo.yres;
  for (size_t i = 0; i < n; ++i) {
    fb->depth_buffer[i] = 1.0f;
  }
}

void
fb_present (Framebuffer* fb)
{
  memcpy(fb->fbp, fb->back_buffer, fb->size);
}

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
  fb->fbp = mmap (0, fb->finfo.smem_len,
                  PROT_READ | PROT_WRITE,
                  MAP_SHARED,
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

  munmap (fb->fbp, fb->finfo.smem_len);
  fb->fbp = NULL;
  return true;
}
