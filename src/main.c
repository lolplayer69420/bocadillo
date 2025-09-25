#include "cpu/cpu.h"
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
}


int main(int argc, char **argv) {
  initialize_cpu(beep);

  if (argc) {
    load_program(argv[1]);
  }

  init_screen();

  while (!WindowShouldClose()) {
    read_key_and_send();
    update();
  }
}

