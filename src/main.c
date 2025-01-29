#include "tigr.h"
#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define map_width 24
#define map_height 24
#define screen_width 640
#define screen_height 480

static inline int32_t min(int32_t x, int32_t y) { return (x < y) ? x : y; }

static inline int32_t max(int32_t x, int32_t y) { return (x > y) ? x : y; }

typedef float f32;
typedef double f64;

int32_t worldMap[map_width][map_height] = {
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

int32_t main() {
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

    for (int32_t x = 0; x < screen_width; x++) {
      f64 camera_x = 2 * x / (f64)screen_width - 1;
      f64 raydir_x = dir_x + plane_x * camera_x;
      f64 raydir_y = dir_y + plane_y * camera_x;

      int32_t map_x = (int32_t)pos_x, map_y = (int32_t)pos_y;

      // arbitrarily large 1e30 to avoid division by zero
      f64 dist_dx = raydir_x == 0 ? 1e30 : fabs(1.0 / raydir_x);
      f64 dist_dy = raydir_y == 0 ? 1e30 : fabs(1.0 / raydir_y);

      f64 side_dist_x, side_dist_y;
      f64 step_x, step_y, perpwall_dist;
      int32_t hit = 0, side;  // hit x wall or y wall

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

      int32_t line_height = (int32_t)(screen_height / perpwall_dist);
      int32_t draw_start = max(-line_height / 2 + screen_height / 2, 0);
      int32_t draw_end =
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

      for (int32_t dy = draw_start; dy < draw_end; dy++) {
        screen->pix[dy * screen->w + x].r = color.r;
        screen->pix[dy * screen->w + x].g = color.g;
        screen->pix[dy * screen->w + x].b = color.b;
        screen->pix[dy * screen->w + x].a = color.a;
      }
    }

    old_time = time;
    time = tigrTime();
    f64 dt_seconds = fabs((time - old_time) / 1000.0);
    f64 fps = (1.0 / dt_seconds);

    f64 move_speed = dt_seconds * (1e5);      // sq/second
    f64 rotation_speed = dt_seconds * (1e5);  // rad/second

    if ( tigrKeyHeld(screen, 'W')) {      
      printf("pos: (%f, %f)\n", pos_x, pos_y);
      if (worldMap[(uint32_t)pos_x][(uint32_t)(pos_y + dir_y * move_speed)] == 0) pos_y += dir_y * move_speed;
      if (worldMap[(uint32_t)(pos_x + dir_x * move_speed)][(uint32_t)(pos_y)] == 0) pos_x += dir_x * move_speed;
    }
    if ( tigrKeyHeld(screen, 'S')) {
      printf("pos: (%f, %f)\n", pos_x, pos_y);
      if (worldMap[(uint32_t)pos_x][(uint32_t)(pos_y - dir_y * move_speed)] == 0) pos_y -= dir_y * move_speed;
      if (worldMap[(uint32_t)(pos_x - dir_x * move_speed)][(uint32_t)(pos_y)] == 0) pos_x -= dir_x * move_speed;
    }

    if  (tigrKeyHeld(screen, 'A')) {
      printf("a down\n");
      f64 old_dir = dir_x;
      f64 old_plane = plane_x;

      dir_x = dir_x * cos(rotation_speed) - dir_y * sin(rotation_speed);
      dir_y = old_dir * sin(rotation_speed) + dir_y * cos(rotation_speed);
      plane_x = plane_x * cos(rotation_speed) - plane_y * sin(rotation_speed);
      plane_y = old_plane * sin(rotation_speed) + plane_y * cos(rotation_speed);
    }
    if  (tigrKeyHeld(screen, 'D')) {
      printf("d down\n");
      f64 old_dir = dir_x;
      f64 old_plane = plane_x;

      dir_x = dir_x * cos(-rotation_speed) - dir_y * sin(-rotation_speed);
      dir_y = old_dir * sin(-rotation_speed) + dir_y * cos(-rotation_speed);
      plane_x = plane_x * cos(-rotation_speed) - plane_y * sin(-rotation_speed);

      plane_y = old_plane * sin(-rotation_speed) + plane_y * cos(-rotation_speed);
    }

    tigrUpdate(screen);
  }
  tigrFree(screen);
  return 0;
}