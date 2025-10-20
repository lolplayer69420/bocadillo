#include "cpu/cpu.h"
#include "debug/debug.h"
#include "graphics.h"
#include <raylib.h>
#include <stdio.h>


void beep() {
  // TODO: Implementar esto
}


void read_key_and_send() {
  char key = GetCharPressed();

  if (key >= '0' && key <= '9') {
    send_key(key - '0');
  }

  if (key >= 'a' && key <= 'f') {
    send_key((key - 'a') + 10);
  }
}


void update() {
  draw_screen();
  do_cycle();

  if (is_debug_info_visible()) {
    show_debug_info();
  }
}


int main(int argc, char **argv) {
  initialize_cpu(beep);

  if (argc) {
    load_program(argv[1]);
  }

  init_screen();

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_F1)) {
      if (!is_debug_info_visible()) {
        show_debug_info();
      } else {
        hide_debug_info();
      }
    }

    if (IsKeyPressed(KEY_SPACE)) {
      if (is_cpu_running()) {
        halt_cpu();
      } else {
        resume_cpu();
      }
    }


    if (IsKeyPressed(KEY_F2)) {
      reset_cpu();
    }

    if (IsKeyPressed(KEY_F3)) {
      if (!is_cpu_running()) {
        do_n_steps(1);
      }
    }

    read_key_and_send();
    update();
  }
}

