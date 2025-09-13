#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdbool.h>
#include <stdint.h>

bool _draw_sprite(uint8_t *sprite, uint8_t x, uint8_t y, uint8_t size);
void _clear_display();
uint64_t *get_display_data();

#endif
