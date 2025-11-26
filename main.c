#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "graphics/buffer.h"
#include "graphics/draw.h"
#include "math/matrix.h"
#include "platform/framebuffer.h"
#include "math/utils.h"

typedef struct
{
  float x, y, z;
  float r, g, b, a;
} Vertex;

static struct termios orig_termios;

void disable_raw_mode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disable_raw_mode);

  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON); // disable echo and canonical mode
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

/* none-blocking input check */
int kbhit() {
  struct timeval tv = { 0L, 0L };
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  return select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
}

int main(void) {
  enable_raw_mode();

  Framebuffer fb;
  if (!fb_open(&fb, "/dev/fb0")) return EXIT_FAILURE;
  if (!fb_get_info(&fb)) return EXIT_FAILURE;
  if (!fb_map(&fb)) return EXIT_FAILURE;

  /* allocate memory for back buffer */
  size_t fb_size = fb.vinfo.xres * fb.vinfo.yres * (fb.vinfo.bits_per_pixel / 8);
  uint8_t *back_buffer = malloc(fb_size);
  if (!back_buffer) {
    fprintf(stderr, "failed to allocate memory for back_buffer\n");
    return EXIT_FAILURE;
  }
  fb.back_buffer = back_buffer;

  Vertex cube_vertices[] = {
    { -0.5f, -0.5f,  0.5f, 1.0f, 0.3f, 0.3f, 1.0f }, /* 0 front-bottom-left:  warm red */
    {  0.5f, -0.5f,  0.5f, 1.0f, 0.6f, 0.2f, 1.0f }, /* 1 front-bottom-right: orange */
    {  0.5f,  0.5f,  0.5f, 1.0f, 0.9f, 0.4f, 1.0f }, /* 2 front-top-right:    light orange */
    { -0.5f,  0.5f,  0.5f, 0.9f, 1.0f, 0.5f, 1.0f }, /* 3 front-top-left:     yellowish */

    { -0.5f, -0.5f, -0.5f, 0.3f, 1.0f, 0.4f, 1.0f }, /* 4 back-bottom-left:   light green */
    {  0.5f, -0.5f, -0.5f, 0.2f, 0.8f, 1.0f, 1.0f }, /* 5 back-bottom-right:  cyan */
    {  0.5f,  0.5f, -0.5f, 0.1f, 0.5f, 1.0f, 1.0f }, /* 6 back-top-right:     blue */
    { -0.5f,  0.5f, -0.5f, 0.4f, 0.2f, 1.0f, 1.0f }  /* 7 back-top-left:      violet */
  };

  unsigned int cube_indices[] = {
    /* front face */
    0, 1, 2,
    0, 2, 3,

    /* back face */
    5, 4, 7,
    5, 7, 6,

    /* left face */
    4, 0, 3,
    4, 3, 7,

    /* right face */
    1, 5, 6,
    1, 6, 2,

    /* top face */
    3, 2, 6,
    3, 6, 7,

    /* bottom face */
    4, 5, 1,
    4, 1, 0
  };

  /* define attributes */
  VertexAttribute attributes[] = {
    { ATTR_POSITION, 0, sizeof(float)*3, 3 },
    { ATTR_COLOR, sizeof(float)*3, sizeof(float)*4, 4 }
  };
  VertexLayout layout = vertex_layout_create(attributes, 2, sizeof(Vertex));
  VertexBuffer vb = vertex_buffer_create  (cube_vertices, layout, 36);
  IndexBuffer  ib = index_buffer_create   (cube_indices, sizeof(cube_indices) / sizeof(cube_indices[0]));

  float angle = 0.0f;

  /* camera position */
  Vec3f_t eye     = { 0.0f, 0.0f, 20.0f };
  Vec3f_t target  = { 0.0f, 0.0f, 0.0f };
  Vec3f_t up      = { 0.0f, 1.0f, 0.0f };

  float camera_speed = 5.0f;

  /* store original positions */
  float *original_positions = malloc(sizeof(float)*3*vb.vertex_count);
  if (!original_positions) {
    fprintf(stderr, "Failed to allocate memory\n");
    vertex_buffer_destroy(&vb);
    fb_close(&fb);
    return EXIT_FAILURE;
  }
  for (uint32_t i = 0; i < vb.vertex_count; ++i) {
    float *pos_ptr = (float *)vertex_buffer_get_attribute_pointer(&vb, i, ATTR_POSITION);
    memcpy(&original_positions[i * 3], pos_ptr, sizeof(float)*3);
  }

  while (1) {
    /* input handling */
    if (kbhit()) {
      char c;
      read(STDIN_FILENO, &c, 1);
      if (c == 'q') break; // quit
      else if (c == 'w') eye.z -= camera_speed;
      else if (c == 's') eye.z += camera_speed;
      else if (c == 'a') eye.x -= camera_speed;
      else if (c == 'd') eye.x += camera_speed;
      else if (c == 'r') eye.y += camera_speed;
      else if (c == 'f') eye.y -= camera_speed;
    }

    /* clear back buffer */
    memset(back_buffer, 0, fb_size);

    /* model matrix */
    Mat4x4f_t model = mat4x4f_identity();
    model = mat4x4f_rotation(&model, (Vec3f_t){ angle , angle, angle });

    /* view matrix */
    Mat4x4f_t view = mat4x4f_lookat(eye, target, up);

    /* projection matrix */
    float aspect = (float)fb.vinfo.xres / fb.vinfo.yres;
    float fov = math_deg_to_rad(45);
    float near = 0.1f;
    float far = 10.0f;

    Mat4x4f_t proj = mat4x4f_perspective(fov, aspect, near, far);

    /* combine to MVP matrix */
    Mat4x4f_t mv = mat4x4f_mul(&view, &model);
    Mat4x4f_t mvp = mat4x4f_mul(&proj, &mv);

    /* transform cube vertices */
    Vertex transformed_cube_vertices[vb.vertex_count];
    for (uint32_t i = 0; i < vb.vertex_count; ++i) {
      Vec4f_t orig = {
        original_positions[i * 3 + 0],
        original_positions[i * 3 + 1],
        original_positions[i * 3 + 2],
        1.0f
      };
      Vec4f_t transformed = mat4x4f_mul_vec4f(&mvp, &orig);

      if (transformed.w != 0.0f) {
        transformed.x /= transformed.w;
        transformed.y /= transformed.w;
        transformed.z /= transformed.w;
      }

      transformed_cube_vertices[i].x = transformed.x;
      transformed_cube_vertices[i].y = transformed.y;
      transformed_cube_vertices[i].z = transformed.z;

      float *col_ptr = (float *)vertex_buffer_get_attribute_pointer(&vb, i, ATTR_COLOR);
      if (col_ptr) {
        transformed_cube_vertices[i].r = col_ptr[0];
        transformed_cube_vertices[i].g = col_ptr[1];
        transformed_cube_vertices[i].b = col_ptr[2];
        transformed_cube_vertices[i].a = col_ptr[3];
      } else {
        transformed_cube_vertices[i].r = 1.0f;
        transformed_cube_vertices[i].g = 1.0f;
        transformed_cube_vertices[i].b = 1.0f;
        transformed_cube_vertices[i].a = 1.0f;
      }
    }

    vertex_buffer_update(&vb, transformed_cube_vertices, sizeof(Vertex)*vb.vertex_count);

    /* draw cube */
    draw_index_buffer(&fb, &ib, &vb, PRIM_TRIANGLES);

    memcpy(fb.fbp, back_buffer, fb_size);

    angle += 0.01f;
    if (angle > 2.0f * M_PI) angle -= 2.0f * M_PI;

    /* 60 fps delay */
    usleep(16000);
  }

  /* cleanup */
  free(original_positions);
  vertex_buffer_destroy(&vb);
  fb_close(&fb);

  return 0;
}
