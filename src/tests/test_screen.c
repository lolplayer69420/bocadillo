#include "Unity/src/unity.h"
#include "../cpu/cpu_internals.h"
#include "../graphics.h"
#include "../cpu/cpu.h"
#include "../raylib/src/raylib.h"
#include "Unity/src/unity_internals.h"

#define len(array) (sizeof(array) / sizeof(array[0]))

void setUp() {
  SetTraceLogLevel(LOG_NONE);
  initialize_cpu(NULL);
}


void tearDown() {
}


void test_drawing_at_coords_0_0() {
  initialize_cpu(NULL);

  instr_t program[] = {
    0x00E0,
    0x6100,
    0x6200,
    0x6300,
    0xF129,
    0xD235
  };

  for (size_t i = 0; i < len(program); ++i) {
    _execute_instruction(program[i]);
  }

  init_screen();
  draw_screen();

  char answer;

  puts("Test drawing at coordinates 0,0");
  printf("Does a 0 appear at those coordinates?: ");
  scanf(" %c", &answer);

  CloseWindow();

  TEST_ASSERT_EQUAL('y', answer);
}


void test_drawing_at_coords_31_0() {
  initialize_cpu(NULL);

  instr_t program[] = {
    0x00E0,
    0x6100,
    0x621F,
    0x6300,
    0xF129,
    0xD235
  };

  for (size_t i = 0; i < len(program); ++i) {
    _execute_instruction(program[i]);
  }

  init_screen();
  draw_screen();

  char answer;

  puts("Test drawing at coordinates 32, 0");
  printf("Does a 0 appear at those coordinates?: ");
  scanf(" %c", &answer);

  CloseWindow();

  TEST_ASSERT_EQUAL('y', answer);
}


void test_drawing_at_coords_31_15() {
  initialize_cpu(NULL);

  instr_t program[] = {
    0x00E0,
    0x6101,
    0x621F,
    0x630F,
    0xF129,
    0xD235
  };

  for (size_t i = 0; i < len(program); ++i) {
    _execute_instruction(program[i]);
  }

  init_screen();
  draw_screen();

  char answer;

  puts("Test drawing at coordinates 31, 15");
  printf("Does a 1 appear at those coordinates?: ");
  scanf(" %c", &answer);

  CloseWindow();

  TEST_ASSERT_EQUAL('y', answer);
}


int main() {
  UNITY_BEGIN();

  RUN_TEST(test_drawing_at_coords_0_0);
  RUN_TEST(test_drawing_at_coords_31_0);
  RUN_TEST(test_drawing_at_coords_31_15);

  UNITY_END();
}

