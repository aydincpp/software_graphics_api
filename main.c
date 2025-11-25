#include "graphics/buffer.h"
#include "graphics/draw.h"
#include "math/matrix.h"
#include "platform/framebuffer.h"
#include <linux/fb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

typedef struct
{
  uint8_t *pixels;
  int width;
  int height;
  int bpp;
  int pitch;
} Surface;

typedef struct
{
  float x, y, z;
  float r, g, b, a;
} Vertex;

int
main (void)
{
  Framebuffer fb;
  if (!fb_open (&fb, "/dev/fb0"))
    return EXIT_FAILURE;
  if (!fb_get_info (&fb))
    return EXIT_FAILURE;
  if (!fb_map (&fb))
    return EXIT_FAILURE;

  size_t fb_size
      = fb.vinfo.xres * fb.vinfo.yres * (fb.vinfo.bits_per_pixel / 8);
  uint8_t *back_buffer = malloc (fb_size);
  if (!back_buffer)
    {
      fprintf (stderr, "failed to allocate memory for back_buffer\n");
    }

  fb.back_buffer = back_buffer;

  /* vertex data */
  Vertex vertices[] = {
    { -0.5f, -0.5f, 0.0f, 1, 0, 0, 1 }, /* bottom-left,  red */
    { 0.5f, -0.5f, 0.0f, 0, 1, 0, 1 },  /* bottom-right, green */
    { 0.0f, 0.5f, 0.0f, 0, 0, 1, 1 }    /* top-center,   blue */
  };

  /* define attributes */
  VertexAttribute attributes[]
      = { { ATTR_POSITION, 0, sizeof (float) * 3, 3 },
          { ATTR_COLOR, sizeof (float) * 3, sizeof (float) * 4, 4 } };
  VertexLayout layout = vertex_layout_create (attributes, 2, sizeof (Vertex));
  VertexBuffer vb = vertex_buffer_create (vertices, layout, 3);

  /* store original positions */
  float *original_positions = malloc (sizeof (float) * 3 * vb.vertex_count);
  if (!original_positions)
    {
      fprintf (stderr, "Failed to allocate memory\n");
      vertex_buffer_destroy (&vb);
      fb_close (&fb);
      return EXIT_FAILURE;
    }
  /* extract original positions from vertices */
  for (uint32_t i = 0; i < vb.vertex_count; ++i)
    {
      float *pos_ptr = (float *)vertex_buffer_get_attribute_pointer (
          &vb, i, ATTR_POSITION);
      memcpy (&original_positions[i * 3], pos_ptr, sizeof (float) * 3);
    }

  float angle = 0.0f;

  while (1)
    {
      /* clear backbuffer */
      memset(back_buffer, 0, fb_size);

      /* compose rotation matrix */
      Mat4x4f_t model = mat4x4f_identity ();
      model = mat4x4f_rotation (&model, (Vec3f_t){ angle, angle, angle });

      /* update position attribute for each vertex */
      for (uint32_t i = 0; i < vb.vertex_count; ++i)
        {
          float *pos_ptr = (float *)vertex_buffer_get_attribute_pointer (
              &vb, i, ATTR_POSITION);
          Vec4f_t orig
              = { original_positions[i * 3 + 0], original_positions[i * 3 + 1],
                  original_positions[i * 3 + 2], 1.0f };
          Vec4f_t transformed = mat4x4f_mul_vec4f (&model, &orig);
          pos_ptr[0] = transformed.x;
          pos_ptr[1] = transformed.y;
          pos_ptr[2] = transformed.z;
        }

      /* draw the vertex buffer */
      draw_vertex_buffer (&fb, &vb, PRIM_TRIANGLES);

      /* copy finished frame into the real framebuffer */
      memcpy(fb.fbp, back_buffer, fb_size);

      /* update angle */
      angle += 0.01f;
      if (angle > 2.0f * 3.14159265f)
        angle -= 2.0f * 3.14159265f;

      /* 60 fps */
      usleep (16000);
    }

  /* cleanup */
  free (original_positions);
  vertex_buffer_destroy (&vb);
  fb_close (&fb);

  return EXIT_SUCCESS;
}
