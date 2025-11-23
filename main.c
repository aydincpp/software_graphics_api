#include "graphics/draw.h"
#include "platform/framebuffer.h"
#include <stdio.h>
#include <stdlib.h>

int
main (void)
{
  Framebuffer fb;
  if (!fb_open (&fb, "/dev/fb0"))
    {
      return EXIT_FAILURE;
    }

  if (!fb_get_info (&fb))
    {
      return EXIT_FAILURE;
    }

  if (!fb_map (&fb))
    {
      return EXIT_FAILURE;
    }

  /* triangle vertices */
  Pixel_t p0
      = { .pos = { .x = fb.vinfo.xres / 4, .y = fb.vinfo.yres / 4 * 3 },
          .color
          = { .r = 255, .g = 0, .b = 0, .a = 255 } }; /* left-bottom red */

  Pixel_t p1
      = { .pos = { .x = fb.vinfo.xres / 4 * 3, .y = fb.vinfo.yres / 4 * 3 },
          .color
          = { .r = 0, .g = 255, .b = 0, .a = 255 } }; /* right-bottom green */

  Pixel_t p2
      = { .pos = { .x = fb.vinfo.xres / 2, .y = fb.vinfo.yres / 4 },
          .color
          = { .r = 0, .g = 0, .b = 255, .a = 255 } }; /* middle-top blue */

  /* draw fill mode */
  draw_triangle_fill (&fb, p0, p1, p2);

  printf ("Press Enter to see wireframe mode.\n");
  getchar ();

  /* clear framebuffer */
  fb_clear_color (&fb, 0, 0, 0);

  /* draw wireframe mode */
  draw_triangle_wireframe (&fb, p0, p1, p2);

  printf ("Press Enter to exit.\n");
  getchar ();

  fb_close (&fb);
  return EXIT_SUCCESS;
}
