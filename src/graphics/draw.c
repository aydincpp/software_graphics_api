#include "graphics/draw.h"
#include "algorithm/bresenham.h"
#include "math/utils.h"
#include <stdio.h>
#include <stdlib.h>

/* 16.16 fixed-point format */
#define FIXED_SHIFT 8
#define FIXED_ONE (1 << FIXED_SHIFT)
#define TO_FIXED(x) ((x) << FIXED_SHIFT)
#define FIXED_MUL(a, b) (((int64_t)(a) * (b)) >> FIXED_SHIFT)
#define FIXED_DIV(a, b) (((int64_t)(a) << FIXED_SHIFT) / (b))

/**
 * Convert normalized device coordinates (NDC) in [-1, 1] range to framebuffer
 * pixel coordinates.
 *
 * NDC's x and y are mapped to framebuffer resolution with origin at top-left.
 *
 * @param fb  Pointer to the framebuffer with resolution info.
 * @param pos Pointer to 2D or 3D position in NDC space.
 * @return Coordinates as Vec2i_t corresponding to framebuffer pixels.
 */
static inline Vec2i_t
ndc_to_framebuffer_coords (const Framebuffer *fb, const float *pos)
{
  return (Vec2i_t){
    .x = (int)((+pos[0] * 0.5f + 0.5f) * fb->vinfo.xres),
    .y = (int)((-pos[1] * 0.5f + 0.5f) * fb->vinfo.yres),
  };
}

/**
 * Convert a vertex at a given index in a vertex buffer to a Pixel_t suitable
 * for drawing.
 *
 * This function extracts necessary vertex data, performs coordinate
 * transformation, and prepares the pixel data for rendering.
 *
 * @param fb    Pointer to the framebuffer.
 * @param vb    Pointer to the vertex buffer.
 * @param index Index of the vertex in the buffer.
 * @param out   Output pointer to Pixel_t that will be filled with transformed
 * vertex information.
 * @return True if conversion succeeded, false if required data was missing or
 * invalid.
 */
static inline bool
vertex_to_pixel (const Framebuffer *fb, const VertexBuffer *vb, uint32_t index,
                 Pixel_t *out)
{
  /* reject missing framebuffer or vertex buffer */
  if (!fb || !vb)
    return false;


  /* fetch position and color attributes */
  float *pos = get_attribute_pointer (vb, index, ATTR_POSITION);

  float *col = get_attribute_pointer (vb, index, ATTR_COLOR);

  /* fallback if attributes are missing */
  if (!pos || !col)
    {
      *out = (Pixel_t){
        .pos    = { 0, 0 },
        .color  = { 0, 0, 0, 255 },
        .depth  = 1.0f
      };
      return false;
    }

  /* convert NDC xy to framebuffer coords */
  out->pos = ndc_to_framebuffer_coords (fb, pos);

  /* convert float RGBA to 0-255 */
  out->color = float4_to_color8 (col);

  /* map NDC z from [-1,1] to depth [0,1] */
  float ndc_z = pos[2];
  float depth = ndc_z * 0.5f + 0.5f;

  /* clamp depth to valid range */
  clampi(depth, 0.0f, 1.0f);
  out->depth = depth;

  return true;
}

void
draw_vertex_buffer (Framebuffer *fb, const VertexBuffer *vb,
                    PrimitiveType prim)
{
  switch (prim)
    {
    case PRIM_POINTS:
      for (uint32_t i = 0; i < vb->vertex_count; ++i)
        {
          Pixel_t p;
          vertex_to_pixel (fb, vb, i, &p);
          draw_pixel (fb, p);
        }
      break;

    case PRIM_LINES:
      for (uint32_t i = 0; i + 1 < vb->vertex_count; i += 2)
        {
          Pixel_t p0;
          Pixel_t p1;
          vertex_to_pixel (fb, vb, i, &p0);
          vertex_to_pixel (fb, vb, i + 1, &p1);
          draw_line (fb, p0, p1);
        }
      break;

    case PRIM_TRIANGLES:
      for (uint32_t i = 0; i + 2 < vb->vertex_count; i += 3)
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
draw_index_buffer (Framebuffer *fb, const IndexBuffer *ib,
                   const VertexBuffer *vb, PrimitiveType prim)
{
  switch (prim)
    {
    case PRIM_POINTS:
      for (uint32_t i = 0; i < ib->count; ++i)
        {
          uint32_t vertex_index = ib->data[i];
          if (vertex_index >= vb->vertex_count)
            continue;

          Pixel_t p;
          vertex_to_pixel (fb, vb, vertex_index, &p);
          draw_pixel (fb, p);
        }
      break;

    case PRIM_LINES:
      for (uint32_t i = 0; i + 1 < ib->count; i += 2)
        {
          uint32_t idx0 = ib->data[i];
          uint32_t idx1 = ib->data[i + 1];

          if (idx0 >= vb->vertex_count || idx1 >= vb->vertex_count)
            continue;

          Pixel_t p0;
          Pixel_t p1;
          vertex_to_pixel (fb, vb, idx0, &p0);
          vertex_to_pixel (fb, vb, idx1, &p1);
          draw_line (fb, p0, p1);
        }
      break;

    case PRIM_TRIANGLES:
      for (uint32_t i = 0; i + 2 < ib->count; i += 3)
        {
          uint32_t idx0 = ib->data[i];
          uint32_t idx1 = ib->data[i + 1];
          uint32_t idx2 = ib->data[i + 2];

          if (idx0 >= vb->vertex_count ||
              idx1 >= vb->vertex_count ||
              idx2 >= vb->vertex_count)
            continue;

          Pixel_t p0;
          Pixel_t p1;
          Pixel_t p2;
          vertex_to_pixel (fb, vb, idx0, &p0);
          vertex_to_pixel (fb, vb, idx1, &p1);
          vertex_to_pixel (fb, vb, idx2, &p2);
          draw_triangle_fill (fb, p0, p1, p2);
        }
      break;

    default:
      fprintf (stderr, "INVALID_PRIMITIVE_TYPE\n");
      break;
    }
}

void
draw_pixel (Framebuffer *fb, Pixel_t p)
{
  /* extract coordinates */
  int x = p.pos.x;
  int y = p.pos.y;

  /* reject pixels outside framebuffer */
  if (x < 0 || x >= (int)fb->vinfo.xres)
    return;
  if (y < 0 || y >= (int)fb->vinfo.yres)
    return;

  /* if no depth buffer, just write the pixel */
  if (!fb->depth_buffer)
    {
      set_pixel (fb, p.pos, p.color);
      return;
    }

  /* compute index into depth buffer */
  uint32_t w = fb->vinfo.xres;
  size_t idx = (size_t)y * w + (size_t)x;

  /* read stored depth */
  float current_depth = fb->depth_buffer[idx];

  /* depth test */
  if (p.depth < current_depth)
    {
      fb->depth_buffer[idx] = p.depth;
      set_pixel (fb, p.pos, p.color);
    }
}

/* lerp integer with fixed-point t_fixed */
static inline uint8_t
lerp_fixed (uint8_t c0, uint8_t c1, int t_fixed)
{
  int diff = c1 - c0;
  return c0 + ((diff * t_fixed) >> FIXED_SHIFT);
}

void
draw_line (Framebuffer *fb, Pixel_t p0, Pixel_t p1)
{
  /* starting pixel coords */
  int x1 = p0.pos.x, y1 = p0.pos.y;
  int x2 = p1.pos.x, y2 = p1.pos.y;

  /* Bresenham setup values */
  int dx, dy, sx, sy, err;
  bresenham_init (x1, y1, x2, y2, &dx, &dy, &sx, &sy, &err);

  /* total number of steps for interpolation */
  int steps = abs (x2 - x1) > abs (y2 - y1) ? abs (x2 - x1) : abs (y2 - y1);

  /* If both endpoints are the same pixel, just draw it */
  if (steps == 0)
    {
      /* degenerate line, draw single pixel */
      draw_pixel (fb, p0);
      return;
    }

  /* fixed point t = 0 */
  int t_fixed = 0;

  /* fixed point dt = 1 / steps */
  int dt_fixed = FIXED_DIV (FIXED_ONE, steps);

  for (;;)
    {
      Pixel_t p;

      /* current pixel coords */
      p.pos.x = x1;
      p.pos.y = y1;

      /* interpolate color in fixed point */
      p.color.r = lerp_fixed (p0.color.r, p1.color.r, t_fixed);
      p.color.g = lerp_fixed (p0.color.g, p1.color.g, t_fixed);
      p.color.b = lerp_fixed (p0.color.b, p1.color.b, t_fixed);
      p.color.a = lerp_fixed (p0.color.a, p1.color.a, t_fixed);

      /* convert fixed-point t to float */
      float t = (float)t_fixed / (float)FIXED_ONE;
      /* interpolate depth */
      p.depth = p0.depth * (1.0f - t) + p1.depth * t;

      /* draw this pixel with depth test */
      draw_pixel (fb, p);

      /* step Bresenham, break when done */
      if (!bresenham_step (&x1, &y1, x2, y2, &err, dx, dy, sx, sy))
        break;

      /* advance t in fixed point */
      t_fixed += dt_fixed;
      if (t_fixed > FIXED_ONE)
        t_fixed = FIXED_ONE;
    }
}

void
draw_triangle_wireframe (Framebuffer *fb, Pixel_t p0, Pixel_t p1, Pixel_t p2)
{
  /* draw edges of the triangle */
  draw_line (fb, p0, p1);
  draw_line (fb, p1, p2);
  draw_line (fb, p2, p0);
}

/*
 * returns the 2d cross product which is twice the area of the triangle abc.
 * the sign tells which side of edge ab the point c lies on.
 * if the 2d cross product is 0 then it means the point lies on the line.
 * */
static inline int
edge_func (Vec2i_t a, Vec2i_t b, Vec2i_t c)
{
  Vec2i_t ab = vec2i_delta (a, b);
  Vec2i_t ac = vec2i_delta (a, c);
  return ab.x * ac.y - ab.y * ac.x;
}

void
draw_triangle_fill (Framebuffer *fb, Pixel_t v0, Pixel_t v1, Pixel_t v2)
{
  /* clamp triangle bounds to framebuffer */
  int xmin = max2i (0, min3i (v0.pos.x, v1.pos.x, v2.pos.x));
  int xmax = min2i (fb->vinfo.xres - 1, max3i (v0.pos.x, v1.pos.x, v2.pos.x));
  int ymin = max2i (0, min3i (v0.pos.y, v1.pos.y, v2.pos.y));
  int ymax = min2i (fb->vinfo.yres - 1, max3i (v0.pos.y, v1.pos.y, v2.pos.y));

  /* signed area of the triangle */
  int area = edge_func (v0.pos, v1.pos, v2.pos);
  if (area == 0)
    return;

  for (int y = ymin; y <= ymax; y++)
    {
      for (int x = xmin; x <= xmax; x++)
        {
          /* current pixel position */
          Vec2i_t p = { x, y };

          /* edge function tests */
          int w0 = edge_func (v0.pos, v1.pos, p);
          int w1 = edge_func (v1.pos, v2.pos, p);
          int w2 = edge_func (v2.pos, v0.pos, p);

          /* skip if point is outside triangle */
          if (!((w0 >= 0 && w1 >= 0 && w2 >= 0)
                || (w0 <= 0 && w1 <= 0 && w2 <= 0)))
            continue;

          /* barycentrics in fixed-point */
          int32_t b0 = FIXED_DIV (w1, area);
          int32_t b1 = FIXED_DIV (w2, area);
          int32_t b2 = FIXED_DIV (w0, area);

          /* convert to float barycentrics */
          float fb0 = (float)b0 / (float)FIXED_ONE;
          float fb1 = (float)b1 / (float)FIXED_ONE;
          float fb2 = (float)b2 / (float)FIXED_ONE;

          /* depth interpolation */
          float depth = fb0 * v0.depth + fb1 * v1.depth + fb2 * v2.depth;

          /* color interpolation in fixed-point */
          Color8_t c;
          c.r = (uint8_t)((b0 * v0.color.r + b1 * v1.color.r + b2 * v2.color.r)
                          >> FIXED_SHIFT);
          c.g = (uint8_t)((b0 * v0.color.g + b1 * v1.color.g + b2 * v2.color.g)
                          >> FIXED_SHIFT);
          c.b = (uint8_t)((b0 * v0.color.b + b1 * v1.color.b + b2 * v2.color.b)
                          >> FIXED_SHIFT);
          c.a = (uint8_t)((b0 * v0.color.a + b1 * v1.color.a + b2 * v2.color.a)
                          >> FIXED_SHIFT);

          /* prepare pixel for drawing */
          Pixel_t out = { p, c, depth };
          draw_pixel (fb, out);
        }
    }
}
