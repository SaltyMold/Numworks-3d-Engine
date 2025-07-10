#ifndef ENGINE_H
#define ENGINE_H

#include "eadk.h"

typedef struct {
  float x, y, z;
} vec3;

typedef struct {
  vec3 position;
  vec3 target;
  vec3 up;
  float fov;
  float aspect;
  float near;
  float far;
} camera_t;

extern const vec3 cube_vertices[8];
extern const int cube_edges[12][2];
void project(vec3 v, int *x, int *y, camera_t *cam);
void draw_line(int x0, int y0, int x1, int y1, eadk_color_t color);
vec3 apply_view_projection(vec3 v, camera_t *cam);

#endif