#include "graphics/draw.h"
#include "algorithm/bresenham.h"
#include "math/utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Convert normalized device coordinates (NDC) in [-1, 1] range to framebuffer pixel coordinates.
 *
 * NDC's x and y are mapped to framebuffer resolution with origin at top-left.
 *
 * @param fb  Pointer to the framebuffer with resolution info.
 * @param pos Pointer to 2D or 3D position in NDC space.
 * @return Coordinates as Vec2i_t corresponding to framebuffer pixels.
 */
static inline Vec2i_t
ndc_to_framebuffer_coords (const Framebuffer *fb,
                           const float *pos)
{
  return (Vec2i_t){
    .x = (int)((+pos[0] * 0.5f + 0.5f) * fb->vinfo.xres),
    .y = (int)((-pos[1] * 0.5f + 0.5f) * fb->vinfo.yres),
  };
}

/**
 * Convert a vertex at a given index in a vertex buffer to a Pixel_t suitable for drawing.
 *
 * This function extracts necessary vertex data, performs coordinate transformation,
 * and prepares the pixel data for rendering.
 *
 * @param fb    Pointer to the framebuffer.
 * @param vb    Pointer to the vertex buffer.
 * @param index Index of the vertex in the buffer.
 * @param out   Output pointer to Pixel_t that will be filled with transformed vertex information.
 * @return True if conversion succeeded, false if required data was missing or invalid.
 */
static inline bool
vertex_to_pixel (const Framebuffer *fb,
                 const VertexBuffer *vb,
                 uint32_t index,
                 Pixel_t *out)
{
  if (!fb || !vb)
    return false;

  float *pos = vertex_buffer_get_attribute_pointer (vb, index, ATTR_POSITION);
  float *col = vertex_buffer_get_attribute_pointer (vb, index, ATTR_COLOR);

  if (!pos || !col)
    {
      *out = (Pixel_t){ .pos = { 0, 0 }, .color = { 0, 0, 0, 255 } };
      return false;
    }

  out->pos    = ndc_to_framebuffer_coords (fb, pos);
  out->color  = float4_to_color8 (col);

  return true;
}

void
draw_vertex_buffer (Framebuffer *fb,
                    const VertexBuffer *vb,
                    PrimitiveType prim) {
  switch (prim)
    {
    case PRIM_POINTS:
      for (uint32_t i = 0; i < vb->vertex_count; ++i)
        {
          Pixel_t p;
          vertex_to_pixel (fb, vb, i, &p);
          draw_pixel (fb, p);
        } break;

    case PRIM_LINES:
      for (uint32_t i = 0; i < vb->vertex_count; i += 2)
        {
          Pixel_t p0;
          Pixel_t p1;
          vertex_to_pixel (fb, vb, i, &p0);
          vertex_to_pixel (fb, vb, i + 1, &p1);
          draw_line (fb, p0, p1);
        }
      break;

    case PRIM_TRIANGLES:
      for (uint32_t i = 0; i < vb->vertex_count; i += 3)
        {
          Pixel_t p0;
          Pixel_t p1;
          Pixel_t p2;
          vertex_to_pixel (fb, vb, i, &p0);
          vertex_to_pixel (fb, vb, i + 1, &p1);
          vertex_to_pixel (fb, vb, i + 2, &p2);
          draw_triangle_fill (fb, p0, p1, p2);
        }
      break;

    default:
      fprintf (stderr, "INVALID_PRIMITIVE_TYPE\n");
      break;
    }
}

void
draw_pixel (Framebuffer *fb,
            Pixel_t p)
{
  int x = p.pos.x;
  int y = p.pos.y;

  if (x < 0 || x >= (int)fb->vinfo.xres) return;
  if (y < 0 || y >= (int)fb->vinfo.yres) return;

  set_pixel (fb, p.pos, p.color);
}

void
draw_line (Framebuffer *fb,
           Pixel_t p0,
           Pixel_t p1)
{
  /* starting and ending coordinates */
  int x1 = p0.pos.x;
  int y1 = p0.pos.y;
  int x2 = p1.pos.x;
  int y2 = p1.pos.y;

  /* variables used by bresenham's algorithm */
  int dx, dy, sx, sy, err;

  /* initialize bresenham algorithm state */
  bresenham_init (x1, y1, x2, y2, &dx, &dy, &sx, &sy, &err);

  /* euclidean distance between start and end points */
  float length = vec2i_distance (p0.pos, p1.pos);

  /* special case where start and end are the same pixel */
  if (length == 0.0f)
    {
      draw_pixel (fb, p0);
      return;
    }

  /* number of steps which is the length of the dominant axis */
  int steps = abs (x2 - x1) > abs (y2 - y1) ? abs (x2 - x1) : abs (y2 - y1);

  /* interpolation increment per pixel step */
  float dt = steps > 0 ? 1.0f / steps : 1.0f;

  /* interpolation value */
  float t = 0.0f;

  /* loop through each pixel position on the line */
  for (;;)
    {
      Pixel_t p;
      p.pos.x = x1;
      p.pos.y = y1;

      /* interpolate each color channel base on the current t */
      p.color.r = math_lerpi (p0.color.r, p1.color.r, t);
      p.color.g = math_lerpi (p0.color.g, p1.color.g, t);
      p.color.b = math_lerpi (p0.color.b, p1.color.b, t);
      p.color.a = math_lerpi (p0.color.a, p1.color.a, t);

      /* draw the pixel */
      draw_pixel (fb, p);

      /* advance to the next pixel and break if it reached end */
      if (!bresenham_step (&x1, &y1, x2, y2, &err, dx, dy, sx, sy))
        break;

      t += dt;
      if (t > 1.0f)
        t = 1.0f;
    }
}

void
draw_triangle_wireframe (Framebuffer *fb,
                         Pixel_t p0,
                         Pixel_t p1,
                         Pixel_t p2)
{
  draw_line (fb, p0, p1);
  draw_line (fb, p1, p2);
  draw_line (fb, p2, p0);
}

/*
 * returns the 2d cross product which is twice the area of the triangle abc.
 * the sign tells which side of edge ab the point c lies on.
 * if the 2d cross product is 0 then it means the point lies on the line.
 * */
static inline float
edge_func (Vec2i_t a, Vec2i_t b, Vec2i_t c)
{
  Vec2i_t ab = vec2i_delta (a, b);
  Vec2i_t ac = vec2i_delta (a, c);
  return vec2i_cross (ab, ac);
}

void
draw_triangle_fill (Framebuffer *fb,
                    Pixel_t v0,
                    Pixel_t v1,
                    Pixel_t v2)
{
  /* triangle's bounding box */
  int xmin = fmaxf(0, (int)floorf(math_fmin3f(v0.pos.x, v1.pos.x, v2.pos.x)));
  int xmax = fminf(fb->vinfo.xres - 1, (int)ceilf(math_fmax3f(v0.pos.x, v1.pos.x, v2.pos.x)));
  int ymin = fmaxf(0, (int)floorf(math_fmin3f(v0.pos.y, v1.pos.y, v2.pos.y)));
  int ymax = fminf(fb->vinfo.yres - 1, (int)ceilf(math_fmax3f(v0.pos.y, v1.pos.y, v2.pos.y)));

  /* 2x area of the triangle */
  float area = edge_func (v0.pos, v1.pos, v2.pos);

  /* iterate over all the pixels in the bounding box of the triangle */
  for (int y = ymin; y < ymax; y++)
    {
      for (int x = xmin; x < xmax; x++)
        {
          /* position of the pixel */
          Vec2i_t p = { x, y };

          /* 2x area of each sub-triangle */
          float w0 = edge_func (v0.pos, v1.pos, p);
          float w1 = edge_func (v1.pos, v2.pos, p);
          float w2 = edge_func (v2.pos, v0.pos, p);

          /* check if the pixel is inside the the triangle */
          if (!((w0 >= 0 && w1 >= 0 && w2 >= 0)
                || (w0 <= 0 && w1 <= 0 && w2 <= 0)))
            continue;

          /* barycentric coordinates */
          float b0 = w1 / area;
          float b1 = w2 / area;
          float b2 = w0 / area;

          /* interpolate color for the out pixel */
          Color8_t c;
          c.r = b0 * v0.color.r + b1 * v1.color.r + b2 * v2.color.r;
          c.g = b0 * v0.color.g + b1 * v1.color.g + b2 * v2.color.g;
          c.b = b0 * v0.color.b + b1 * v1.color.b + b2 * v2.color.b;
          c.a = b0 * v0.color.a + b1 * v1.color.a + b2 * v2.color.a;

          Pixel_t out = { p, c };
          draw_pixel (fb, out);
        }
    }
}
