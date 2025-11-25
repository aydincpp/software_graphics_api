#include "graphics/buffer.h"
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

  /* clang-format off */
  float vertices[] = {
    /* triangle 1 */
    /* position */        /* color */
    -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,  /* bottom-left,  red */
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,  /* bottom-right, green */
     0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,  /* top-right,    blue */

    /* triangle 2 */
    /* position */       /* color */
    -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,   /* bottom-left, red */
     0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f,   /* top-right,   blue */
    -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f, 1.0f,   /* top-left,    yellow */
  };

  /* define vertex attributes */
  VertexAttribute attributes[] = {
    {ATTR_POSITION, 0,                  sizeof(float) * 3, 3},
    {ATTR_COLOR,    sizeof(float) * 3,  sizeof(float) * 4, 4}
  };
  /* clang-format on */

  VertexLayout layout = vertex_layout_create(attributes, 2, sizeof(float) * 7);

  /* create a vertex buffer initialized with the vertex data */
  VertexBuffer vertex_buffer = vertex_buffer_create(vertices, layout, 6);

  /* access and print each vertex's position and color */
  for (uint32_t i = 0; i < vertex_buffer.vertex_count; i++)
  {
    float *pos = (float *)vertex_buffer_get_attribute_pointer(&vertex_buffer, i, ATTR_POSITION);
    float *col = (float *)vertex_buffer_get_attribute_pointer(&vertex_buffer, i, ATTR_COLOR);

    if (pos && col)
    {
      printf("Vertex %u Position: (%f, %f, %f) Color: (%f, %f, %f, %f)\n",
             i, pos[0], pos[1], pos[2], col[0], col[1], col[2], col[3]);
    }
  }

  /* destroy vertex buffer */
  vertex_buffer_destroy(&vertex_buffer);

  fb_close (&fb);
  return EXIT_SUCCESS;
}
