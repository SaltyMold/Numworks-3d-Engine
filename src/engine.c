#include "engine.h"
#include "eadk.h"

static vec3 cross(vec3 a, vec3 b) {
  return (vec3){
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x
  };
}

static vec3 normalize(vec3 v) {
  float len = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
  if (len == 0) return v;
  return (vec3){v.x/len, v.y/len, v.z/len};
}

static vec3 sub(vec3 a, vec3 b) {
  return (vec3){a.x-b.x, a.y-b.y, a.z-b.z};
}

static float dot(vec3 a, vec3 b) {
  return a.x*b.x + a.y*b.y + a.z*b.z;
}

vec3 apply_view_projection(vec3 v, camera_t *cam) {
  vec3 zaxis = normalize(sub(cam->position, cam->target));
  vec3 xaxis = normalize(cross(cam->up, zaxis));
  vec3 yaxis = cross(zaxis, xaxis);
  vec3 t = sub(v, cam->position);
  float vx = dot(t, xaxis);
  float vy = dot(t, yaxis);
  float vz = dot(t, zaxis);

  float f = 1.0f / tanf(cam->fov * 0.5f);
  float aspect = cam->aspect;
  float z = -vz;
  if (z == 0) z = 0.0001f;
  float px = vx * f / aspect / z;
  float py = vy * f / z;
  float pz = (z - cam->near) / (cam->far - cam->near);
  return (vec3){px, py, pz};
}

void project(vec3 v, int *x, int *y, camera_t *cam) {
  vec3 p = apply_view_projection(v, cam);
  *x = (int)(EADK_SCREEN_WIDTH/2 + p.x * (EADK_SCREEN_WIDTH/2));
  *y = (int)(EADK_SCREEN_HEIGHT/2 - p.y * (EADK_SCREEN_HEIGHT/2));
}

void draw_line(int x0, int y0, int x1, int y1, eadk_color_t color) {
  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = dx + dy, e2;
  while (1) {
    eadk_display_push_rect_uniform((eadk_rect_t){x0, y0, 1, 1}, color);
    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    if (e2 >= dy) { err += dy; x0 += sx; }
    if (e2 <= dx) { err += dx; y0 += sy; }
  }
}