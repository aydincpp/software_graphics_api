#include "platform/framebuffer.h"
#include "platform/input.h"
#include "graphics/buffer.h"
#include "graphics/draw.h"
#include "math/matrix.h"
#include "math/utils.h"

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define COUNT_OF_ARRAY(array) (sizeof(array) / sizeof(array[0]))

typedef struct {
  Vec3f_t pos;
  Vec4f_t col;
} Vertex;

typedef struct {
  Mat4x4f_t model;
  Mat4x4f_t view;
  Mat4x4f_t proj;
} TransformMatrices;

typedef struct {
  Vec3f_t eye;
  Vec3f_t target;
  Vec3f_t up;
  float fov;
  float aspect;
  float near;
  float far;
  float yaw;
  float pitch;
  float move_speed;
  float rotate_speed;
} Camera;

Vec3f_t camera_get_direction(const Camera* cam) {
  Vec3f_t dir;
  dir.x = cosf(cam->pitch) * sinf(cam->yaw);
  dir.y = sinf(cam->pitch);
  dir.z = cosf(cam->pitch) * cosf(cam->yaw);
  return vec3f_normalize(dir);
}

void handle_input(Input* input, Camera* camera)
{
  // exit on q press
  if (input->pressed['q']) {
    exit(0);
  }

  Vec3f_t forward = camera_get_direction(camera);
  camera->target  = vec3f_add(camera->eye, forward);
  Vec3f_t right   = { cosf(camera->yaw), 0, -sin(camera->yaw) };

  // movement
  if (input->keys['w']) camera->eye = vec3f_sub(camera->eye, vec3f_mul_scalar(forward,  camera->move_speed));
  if (input->keys['s']) camera->eye = vec3f_add(camera->eye, vec3f_mul_scalar(forward,  camera->move_speed));
  if (input->keys['d']) camera->eye = vec3f_add(camera->eye, vec3f_mul_scalar(right,    camera->move_speed));
  if (input->keys['a']) camera->eye = vec3f_sub(camera->eye, vec3f_mul_scalar(right,    camera->move_speed));

  // rotation
  if (input->keys['h']) camera->yaw -= camera->rotate_speed;
  if (input->keys['l']) camera->yaw += camera->rotate_speed;

  if (input->keys['k']) {
    camera->pitch += camera->rotate_speed;
    if (camera->pitch > PI / 2.0f - 0.01f) camera->pitch = PI / 2.0f - 0.01f;
  }
  if (input->keys['j']) {
    camera->pitch -= camera->rotate_speed;
    if (camera->pitch < -PI / 2.0f + 0.01f) camera->pitch = -PI / 2.0f + 0.01f;
  }
}

int main (void)
{
  // initialize framebuffer
  Framebuffer fb;
  if (!fb_init(&fb, "/dev/fb0"))
    return EXIT_FAILURE;

  // initialize input
  Input input;
  input_init(&input);

  // define vertices
  const Vertex vertices[] = {
    { .pos = {-0.5f, -0.5f, 0.0f }, .col = { 1.0f, 0.0f, 0.0f, 1.0f } },
    { .pos = { 0.5f, -0.5f, 0.0f }, .col = { 0.0f, 1.0f, 0.0f, 1.0f } },
    { .pos = { 0.0f,  0.5f, 0.0f }, .col = { 0.0f, 0.0f, 1.0f, 1.0f } }
  };

  // define indices
  const unsigned int indices[] = {
    0, 1, 2
  };

  // define attributes
  VertexAttribute attributes[] = {
    { ATTR_POSITION, offsetof(Vertex, pos), sizeof(float), 3 },
    { ATTR_COLOR,    offsetof(Vertex, col), sizeof(float), 4 }
  };
  VertexLayout layout = vertex_layout_create(attributes, COUNT_OF_ARRAY(attributes), sizeof(Vertex));
  VertexBuffer vb     = vertex_buffer_create(vertices, layout, COUNT_OF_ARRAY(vertices));
  IndexBuffer  ib     = index_buffer_create(indices, COUNT_OF_ARRAY(indices));

  float angle = 0.0f;
  TransformMatrices transformMatrices;

  // initialize camera
  Camera camera = {
    .eye          = { 0.0f,  0.0f,  10.0f },
    .target       = { 0.0f,  0.0f,  0.0f },
    .up           = { 0.0f, -1.0f,  0.0f },
    .fov          = deg_to_rad(60),
    .aspect       = fb.aspect,
    .near         = 0.1f,
    .far          = 1000.0f,
    .yaw          = 0.0f,
    .pitch        = 0.0f,
    .move_speed   = 0.1f,
    .rotate_speed = 0.03f
  };

  // control main loop
  bool running = true;

  // main loop
  while (running)
  {
    // process input
    input_update(&input);
    handle_input(&input, &camera);

    // model matrix
    Vec3f_t rotation = { angle, angle, angle };
    transformMatrices.model = mat4x4f_identity();
    transformMatrices.model = mat4x4f_rotation(&transformMatrices.model, rotation);

    // view matrix
    transformMatrices.view    = mat4x4f_lookat(camera.eye, camera.target, camera.up);

    // projection matrix
    transformMatrices.proj    = mat4x4f_perspective(camera.fov, camera.aspect, camera.near, camera.far);

    // combine to MVP matrix
    Mat4x4f_t mvp = mat4x4f_mul(&transformMatrices.view, &transformMatrices.model);
    mvp           = mat4x4f_mul(&transformMatrices.proj, &mvp);

    // update vertices
    for (size_t i = 0; i < vb.vertex_count; ++i) {
      // get the position attribute for the vertex
      Vec3f_t* position = get_attribute_pointer(&vb, i, ATTR_POSITION);

      // convert to homogeneous coordinates
      Vec4f_t vertex_position = { vertices[i].pos.x, vertices[i].pos.y, vertices[i].pos.z, 1.0f };

      // apply the MVP matrix
      Vec4f_t transformed_position = mat4x4f_mul_vec4f(&mvp, &vertex_position);

      // apply perspective division
      if (transformed_position.w != 0.0f) {
        transformed_position.x /= transformed_position.w;
        transformed_position.y /= transformed_position.w;
        transformed_position.z /= transformed_position.w;
      }

      // update vertex position
      position->x = transformed_position.x;
      position->y = transformed_position.y;
      position->z = transformed_position.z;
    }

    fb_clear(&fb);
    draw_index_buffer(&fb, &ib, &vb, PRIM_TRIANGLES);
    fb_present(&fb);

    // update angle
    angle += 0.01f;
    if (angle > PI)
      angle -= 2 * PI;

    // 60 fps
    usleep(16000);
  }

  // cleanup and exit
  vertex_buffer_destroy(&vb);
  index_buffer_destroy(&ib);
  input_shutdown();
  fb_close(&fb);

  return EXIT_SUCCESS;
}
