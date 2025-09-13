// TODO: Implementar esto

#include "framebuffer.h"
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <stdint.h>

#define FRAMEBUFFER_SIZE 32
#define INLINE static inline __attribute__((always_inline))

uint64_t framebuffer[FRAMEBUFFER_SIZE];


bool _draw_sprite(uint8_t *sprite, uint8_t x, uint8_t y, uint8_t size) {
  // TODO: Probar esto
  bool it_collides = false;

  uint8_t x_pos = x % 64;
  uint8_t y_pos = y % 32;

  for (uint8_t row = 0; row < size && y_pos < 32; ++row, ++y_pos) {
    uint64_t sprite_row = sprite[row];

    sprite_row = (sprite_row << 56) >> x_pos;

    if (framebuffer[y_pos] & sprite_row) {
      it_collides = true;
    }

    framebuffer[y_pos] ^= sprite_row;
  }

  return it_collides;
}


void _clear_display() {
  for (size_t i = 0; i < FRAMEBUFFER_SIZE; ++i) {
    framebuffer[i] = 0;
  }
}


uint64_t *get_display_data() {
  return framebuffer;
}

