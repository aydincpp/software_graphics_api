#include "platform/framebuffer.h"
#include "graphics/buffer.h"
#include "graphics/draw.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define COUNT_OF_ARRAY(array) (sizeof(array) / sizeof(array[0]))

typedef struct {
  float x, y, z;
  float r, g, b, a;
} Vertex;

int main (void)
{
  // initialize framebuffer
  Framebuffer fb;
  if (!fb_init(&fb, "/dev/fb0"))
    return EXIT_FAILURE;

  // define vertices
  const Vertex vertices[] = {
    { -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
    {  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },
    {  0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f }
  };

  // define indices
  const unsigned int indices[] = {
    0, 1, 2
  };

  // define attributes
  VertexAttribute attributes[] = {
    { ATTR_POSITION,  offsetof(Vertex, x), sizeof(float), 3 },
    { ATTR_COLOR,     offsetof(Vertex, r), sizeof(float), 4 }
  };
  VertexLayout layout = vertex_layout_create(attributes, COUNT_OF_ARRAY(attributes), sizeof(Vertex));
  VertexBuffer vb     = vertex_buffer_create(vertices, layout, COUNT_OF_ARRAY(vertices));
  IndexBuffer  ib     = index_buffer_create(indices, COUNT_OF_ARRAY(indices));

  // main loop
  while (true)
  {
    fb_clear(&fb);
    draw_index_buffer(&fb, &ib, &vb, PRIM_TRIANGLES);
    fb_present(&fb);
  }

  // cleanup and exit
  return EXIT_SUCCESS;
}
