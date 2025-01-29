#include "tigr.h"
#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define map_width 24
#define map_height 24
#define screen_width 640
#define screen_height 480

typedef int32_t i32;
typedef uint32_t u32;
typedef uint8_t u8;
typedef float f32;
typedef double f64;

static inline i32 min(i32 x, i32 y) { return (x < y) ? x : y; }
static inline i32 max(i32 x, i32 y) { return (x > y) ? x : y; }
static inline void draw_verline(Tigr* bmp, i32 x, i32 y0, i32 y1,
                                TPixel color) {
  for (i32 dy = y0; dy <= y1; dy++) {
    i32 i = dy * bmp->w + x;
    bmp->pix[i].r += (u8)((color.r - bmp->pix[i].r));
    bmp->pix[i].g += (u8)((color.g - bmp->pix[i].g));
    bmp->pix[i].b += (u8)((color.b - bmp->pix[i].b));
    bmp->pix[i].a += (u8)((color.b - bmp->pix[i].a));
  }
}

i32 worldMap[map_width][map_height] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

TPixel red = {.r = 255, .g = 0, .b = 0, .a = 1};
TPixel green = {.r = 0, .g = 255, .b = 0, .a = 1};
TPixel blue = {.r = 0, .g = 0, .b = 255, .a = 1};
TPixel white = {.r = 255, .g = 255, .b = 255, .a = 1};
TPixel yellow = {.r = 255, .g = 255, .b = 0, .a = 1};

i32 main() {
  f64 pos_x = 22, pos_y = 12;
  f64 dir_x = -1, dir_y = 0;
  f64 plane_x = 0, plane_y = 0.66;
  // angle between direction and plane vectors is given by: f(x) = 2 *
  // atan(len(plane) / len(dir));

  f64 time = 0;
  f64 old_time = 0;

  Tigr* screen = tigrWindow(screen_width, screen_height, "Hello", 0);
  while (!tigrClosed(screen)) {
    tigrClear(screen, tigrRGB(0x0, 0x0, 0x0));

    old_time = time;
    time = tigrTime();
    f64 dt = fabs((time - old_time));
    f64 fps = (1.0 / dt);

    char buffer[256];
    sprintf(buffer, "fps: %f\n", fps);
    tigrPrint(screen, tfont, 200, 200, tigrRGB(0xff, 0xff, 0xff),
              "Hello, world.");

    for (i32 x = 0; x < screen_width; x++) {
      f64 camera_x = 2 * x / (f64)screen_width - 1;
      f64 raydir_x = dir_x + plane_x * camera_x;
      f64 raydir_y = dir_y + plane_y * camera_x;

      i32 map_x = (i32)pos_x, map_y = (i32)pos_y;

      // arbitrarily large 1e30 to avoid division by zero
      f64 dist_dx = raydir_x == 0 ? 1e30 : fabs(1.0 / raydir_x);
      f64 dist_dy = raydir_y == 0 ? 1e30 : fabs(1.0 / raydir_y);

      f64 side_dist_x, side_dist_y;
      f64 step_x, step_y, perpwall_dist;
      i32 hit = 0, side;  // hit x wall or y wall

      if (raydir_x < 0) {
        step_x = -1;
        side_dist_x = (pos_x - map_x) * dist_dx;
      } else {
        step_x = 1;
        side_dist_x = (map_x + 1.0 - pos_x) * dist_dx;
      }

      if (raydir_y < 0) {
        step_y = -1;
        side_dist_y = (pos_y - map_y) * dist_dy;
      } else {
        step_y = 1;
        side_dist_y = (map_y + 1.0 - pos_y) * dist_dy;
      }

      while (hit == 0) {
        if (side_dist_x < side_dist_y) {
          side_dist_x += dist_dx;
          map_x += step_x;
          side = 0;
        } else {
          side_dist_y += dist_dy;
          map_y += step_y;
          side = 1;
        }
        if (worldMap[map_x][map_y] > 0) hit = 1;
      }

      if (side == 0)
        perpwall_dist = (side_dist_x - dist_dx);
      else
        perpwall_dist = (side_dist_y - dist_dy);

      i32 line_height = (i32)(screen_height / perpwall_dist);
      i32 draw_start = max(-line_height / 2 + screen_height / 2, 0);
      i32 draw_end =
          min(line_height / 2 + screen_height / 2, screen_height - 1);

      TPixel color;
      switch (worldMap[map_x][map_y]) {
        case 1:
          color = red;
          break;
        case 2:
          color = green;
          break;
        case 3:
          color = blue;
          break;
        case 4:
          color = white;
          break;
        default:
          color = yellow;
          break;
      }

      if (side == 1) {
        color.r /= 2;
        color.g /= 2;
        color.b /= 2;
        color.a /= 2;
      }
      draw_verline(screen, x, draw_start, draw_end, color);
    }

    f64 move_speed = dt * 120.0;     // sq/second
    f64 rotation_speed = dt * 80.0;  // rad/second

    if (tigrKeyHeld(screen, 'W')) {
      if (worldMap[(u32)pos_x][(u32)(pos_y + dir_y * move_speed)] == 0)
        pos_y += dir_y * move_speed;
      if (worldMap[(u32)(pos_x + dir_x * move_speed)][(u32)(pos_y)] == 0)
        pos_x += dir_x * move_speed;
    }
    if (tigrKeyHeld(screen, 'S')) {
      if (worldMap[(u32)pos_x][(u32)(pos_y - dir_y * move_speed)] == 0)
        pos_y -= dir_y * move_speed;
      if (worldMap[(u32)(pos_x - dir_x * move_speed)][(u32)(pos_y)] == 0)
        pos_x -= dir_x * move_speed;
    }

    if (tigrKeyHeld(screen, 'A')) {
      f64 old_dir = dir_x;
      f64 old_plane = plane_x;

      dir_x = dir_x * cos(rotation_speed) - dir_y * sin(rotation_speed);
      dir_y = old_dir * sin(rotation_speed) + dir_y * cos(rotation_speed);
      plane_x = plane_x * cos(rotation_speed) - plane_y * sin(rotation_speed);
      plane_y = old_plane * sin(rotation_speed) + plane_y * cos(rotation_speed);
    }
    if (tigrKeyHeld(screen, 'D')) {
      f64 old_dir = dir_x;
      f64 old_plane = plane_x;

      dir_x = dir_x * cos(-rotation_speed) - dir_y * sin(-rotation_speed);
      dir_y = old_dir * sin(-rotation_speed) + dir_y * cos(-rotation_speed);
      plane_x = plane_x * cos(-rotation_speed) - plane_y * sin(-rotation_speed);

      plane_y =
          old_plane * sin(-rotation_speed) + plane_y * cos(-rotation_speed);
    }

    tigrUpdate(screen);
  }
  tigrFree(screen);
  return 0;
}