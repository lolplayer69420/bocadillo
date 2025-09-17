#include "cpu/framebuffer.h"
#include "raylib/src/raylib.h"
#include <stddef.h>
#include <stdint.h>


static int screen_width = 64 * 10;
static int screen_height = 32 * 10;


void init_screen() {
  InitWindow(screen_width, screen_height, "bocadillo");
  ClearBackground(BLACK);
}


void draw_screen() {
  BeginDrawing();

  if (IsWindowResized()) {
    screen_width = GetScreenWidth();
    screen_height = GetScreenHeight();
  }

  int pixel_width = screen_width / 64;
  int pixel_height = screen_height / 32;
  uint64_t *framebuffer = get_display_data(); 

  for (int pos_y = 0; pos_y < 32; ++pos_y) {
    for (int pos_x = 0; pos_x < 64; ++pos_x) {
      uint64_t current_pixel = (framebuffer[pos_y] >> pos_x) & 0x1;

      if (current_pixel) {
        DrawRectangle(pos_x * pixel_width, pos_y * pixel_height,
                      pixel_width, pixel_height, RAYWHITE);
      }
    }
  }

  EndDrawing();
}

