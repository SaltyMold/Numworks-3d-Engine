#include "eadk.h"
#include "engine.h"
#include "start.h"
#include "shape.h"

const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "3D-ENGINE";
const uint32_t eadk_api_level  __attribute__((section(".rodata.eadk_api_level"))) = 0;

int main() {
  int shape_index = start();

  float angle = 0.7f;
  camera_t cam = {
    .position = {0, 0, 4},
    .target = {0, 0, 0},
    .up = {0, 1, 0},
    .fov = 1.0f, 
    .aspect = (float)EADK_SCREEN_WIDTH / (float)EADK_SCREEN_HEIGHT,
    .near = 0.1f,
    .far = 100.0f
  };

  uint32_t elapsed = 0;

  int sleep_time = 3; 

  char buffer[64];

  while (1) {
    uint32_t start = (uint32_t)eadk_timing_millis();

    angle += 0.01f;
    eadk_display_push_rect_uniform(eadk_screen_rect, eadk_color_white);

    float ca = cosf(angle), sa = sinf(angle);
    float cb = cosf(angle * 0.7f), sb = sinf(angle * 0.7f);
    vec3 rotated[8];
    for (int i=0; i<8; i++) {
      // Rotation Y
      float x = 0, y = 0, z = 0;
      if (shape_index == 0) {
        // Cube
        x = cube_vertices[i].x * ca - cube_vertices[i].z * sa;
        z = cube_vertices[i].x * sa + cube_vertices[i].z * ca;
        y = cube_vertices[i].y;
        rotated[i] = (vec3){x, y, z};
      } else {
        // Pyramid
        x = pyramid_vertices[i].x * ca - pyramid_vertices[i].z * sa;
        z = pyramid_vertices[i].x * sa + pyramid_vertices[i].z * ca;
        y = pyramid_vertices[i].y;
        rotated[i] = (vec3){x, y, z};
      }

      // Rotation X
      float y2 = y * cb - z * sb;
      float z2 = y * sb + z * cb;
      rotated[i].x = x;
      rotated[i].y = y2;
      rotated[i].z = z2;
    }

    if (shape_index == 0) {
      for (int i=0; i<12; i++) {
      int a = cube_edges[i][0], b = cube_edges[i][1];
      int x0, y0, x1, y1;
      project(rotated[a], &x0, &y0, &cam);
      project(rotated[b], &x1, &y1, &cam);
      draw_line(x0, y0, x1, y1, eadk_color_black);
    }
    } else {
      for (int i=0; i<8; i++) {
        int a = pyramid_edges[i][0], b = pyramid_edges[i][1];
        int x0, y0, x1, y1;
        project(rotated[a], &x0, &y0, &cam);
        project(rotated[b], &x1, &y1, &cam);
        draw_line(x0, y0, x1, y1, eadk_color_black);
      }
    }

    angle += 0.01f;

    uint32_t end = (uint32_t)eadk_timing_millis();
    elapsed = end - start;

    eadk_timing_msleep(sleep_time); 

    snprintf(buffer, sizeof(buffer),
      "Framerate: %u ms | Sleep: %u ms",
      elapsed, sleep_time
    );
    eadk_display_draw_string(buffer, (eadk_point_t){0, 0}, false, eadk_color_black, eadk_color_white);
    
  }

  return 0;
}