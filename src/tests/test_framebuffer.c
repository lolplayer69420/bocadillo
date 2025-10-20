#include "../cpu/framebuffer.h"
#include "../cpu/cpu_internals.h"
#include "../cpu/cpu.h"
#include "Unity/src/unity.h"
#include "Unity/src/unity_internals.h"

// TODO: Escribir las siguientes pruebas:
//  Dibujar un pixel.
//  Dibujar un pixel al final de la pantalla.

#define SINGLE_PIXEL_EXPECTED_RESULT\
  0b000000100000000000000000000000000000000000000000000000000000000

#define PIXEL_AT_THE_END_EXPECTED_RESULF 1

#define len(array) (sizeof(array) / sizeof(array[0]))

void setUp() {
  initialize_cpu(NULL);
  memory[0x0] = 0b10000000;
  memory[0x1] = 0b11111111;
}


void tearDown() {
}


void test_drawing_a_single_pixel() {
  cpu_state.register_file[0x3] = 0x7;
  cpu_state.register_file[0x4] = 0x0;

  _execute_instruction(0xD341);

  uint64_t *framebuffer_data = get_display_data();

  TEST_ASSERT_EQUAL_INT64(SINGLE_PIXEL_EXPECTED_RESULT, framebuffer_data[0]);
}


void test_drawing_a_single_pixel_at_the_end() {
  _clear_display();

  cpu_state.register_file[0x3] = 63;
  cpu_state.register_file[0x4] = 0x0;

  _execute_instruction(0xD341);

  uint64_t *framebuffer_data = get_display_data();

  TEST_ASSERT_EQUAL_INT64(PIXEL_AT_THE_END_EXPECTED_RESULF, framebuffer_data[0]);
}


void test_drawing_a_sprite() {
  initialize_cpu(NULL);

  instr_t program[] = {
    0x00E0,
    0x6100,
    0x6200,
    0x6300,
    0xF129,
    0xD235
  };

  uint64_t sprite[] = {
    0b11110000,
    0b10010000,
    0b10010000,
    0b10010000,
    0b11110000 
  };

  for (uint8_t i = 0; i < 5; ++i) {
    sprite[i] <<= 56;
  }

  for (size_t i = 0; i < len(program); ++i) {
    _execute_instruction(program[i]);
  }

  uint64_t *framebuffer = get_display_data();

  TEST_ASSERT_EQUAL_INT64_ARRAY(sprite, framebuffer, 5);
}


int main() {
  RUN_TEST(test_drawing_a_single_pixel);
  RUN_TEST(test_drawing_a_single_pixel_at_the_end);
  RUN_TEST(test_drawing_a_sprite);
}

