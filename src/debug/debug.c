#include "disassembler.h"
#include <stdbool.h>
#include <raylib.h>
#include <stdint.h>
#include <stdio.h>

#define RAYGUI_IMPLEMENTATION

#include <raygui.h>
#include "../cpu/cpu_internals.h"
#include "debug.h"

bool enabled = false;


bool is_debug_info_visible() {
  return enabled;
}


void _show_register_file() {
  GuiWindowBox(
    (Rectangle){0, 0, 10 * 10, 28 * 10},
    "Registers"
  );

  Vector2 mouse_cell = {0};
  char register_contents[64];

  for (uint8_t reg = 0; reg < 16; ++reg) {
    uint8_t register_value = cpu_state.register_file[reg];

    sprintf(register_contents, "V%X: %d (0x%02X)", 
            reg, register_value, register_value);

    GuiLabel((Rectangle){10, (reg * 10) - 10, 100, 100}, register_contents);
  }

  sprintf(register_contents, "I: 0x%04X", cpu_state.index);
  GuiLabel((Rectangle){10, 18 * 10, 100, 100}, register_contents);

  sprintf(register_contents, "PC: 0x%04X", cpu_state.pc);
  GuiLabel((Rectangle){10, 19 * 10, 100, 100}, register_contents);

  sprintf(register_contents, "SOUND: %d", cpu_state.sound_timer);
  GuiLabel((Rectangle){10, 20 * 10, 100, 100}, register_contents);

  sprintf(register_contents, "DELAY: %d", cpu_state.delay_timer);
  GuiLabel((Rectangle){10, 21 * 10, 100, 100}, register_contents);
}


void show_debug_info() {
  if (!enabled) {
    enabled = true;
    GuiEnable();
  }

  _show_register_file();
  show_disassembler_window();
}


void hide_debug_info() {
  enabled = false;
}

