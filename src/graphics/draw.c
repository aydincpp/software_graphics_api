#include "graphics/draw.h"
#include "algorithm/bresenham.h"
#include "math/utils.h"
#include <math.h>
#include <stdlib.h>

void
draw_pixel (Framebuffer *fb, Pixel_t p)
{
  set_pixel (fb, p.pos, p.color);
}

void
draw_line (Framebuffer *fb, Pixel_t p0, Pixel_t p1)
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
draw_triangle_wireframe (Framebuffer *fb, Pixel_t p0, Pixel_t p1, Pixel_t p2)
{
  draw_line (fb, p0, p1);
  draw_line (fb, p1, p2);
  draw_line (fb, p2, p0);
}

static inline float
edge_func (Vec2i_t a, Vec2i_t b, Vec2i_t c)
{
  Vec2i_t ab = vec2i_delta (a, b);
  Vec2i_t ac = vec2i_delta (a, c);
  return vec2i_cross (ab, ac);
}

void
draw_triangle_fill (Framebuffer *fb, Pixel_t v0, Pixel_t v1, Pixel_t v2)
{
  int xmin = (int)fminf (fminf (v0.pos.x, v1.pos.x), v2.pos.x);
  int xmax = (int)fmaxf (fmaxf (v0.pos.x, v1.pos.x), v2.pos.x);
  int ymin = (int)fminf (fminf (v0.pos.y, v1.pos.y), v2.pos.y);
  int ymax = (int)fmaxf (fmaxf (v0.pos.y, v1.pos.y), v2.pos.y);

  Pixel_t p;
  float area = edge_func (v0.pos, v1.pos, v2.pos);

  for (int y = ymin; y < ymax; y++)
    {
      for (int x = xmin; x < xmax; x++)
        {
          p.pos.x = x;
          p.pos.y = y;

          /* area of each sub-triangle */
          float w0 = edge_func (v0.pos, v1.pos, p.pos);
          float w1 = edge_func (v1.pos, v2.pos, p.pos);
          float w2 = edge_func (v2.pos, v0.pos, p.pos);

          /* areal coordinates or barycentric coordinates */
          float b0 = w1 / area;
          float b1 = w2 / area;
          float b2 = w0 / area;

          Color8_t c0 = v0.color;
          Color8_t c1 = v1.color;
          Color8_t c2 = v2.color;

          if ((w0 >= 0 && w1 >= 0 && w2 >= 0)
              || (w0 <= 0 && w1 <= 0 && w2 <= 0))
            {
              p.color.r = b0 * c0.r + b1 * c1.r + b2 * c2.r;
              p.color.g = b0 * c0.g + b1 * c1.g + b2 * c2.g;
              p.color.b = b0 * c0.b + b1 * c1.b + b2 * c2.b;
              p.color.a = b0 * c0.a + b1 * c1.a + b2 * c2.a;
              draw_pixel (fb, p);
            }
        }
    }
}
