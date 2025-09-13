#include "Unity/src/unity.h"
#include "../cpu/cpu_internals.h"
#include "../cpu/cpu.h"
#include "Unity/src/unity_internals.h"
#include <arpa/inet.h>
#include <stdint.h>
#include <string.h>


void setUp() {
  initialize_cpu();
}


void tearDown() {
}


void test_ld_instruction() {
  instr_t instruction = 0x6333;

  _execute_instruction(instruction);

  TEST_ASSERT_EQUAL_HEX16(0x33, cpu_state.register_file[3]);
}


void test_sub_instruction_sets_vf_to_1_when_no_borrow() {
  instr_t step_1 = 0x6023;
  instr_t step_2 = 0x6109;
  instr_t step_3 = 0x8015;

  _execute_instruction(step_1);
  _execute_instruction(step_2);
  _execute_instruction(step_3);

  TEST_ASSERT_EQUAL_HEX8(0x1, cpu_state.register_file[0xF]);
}


void test_sub_operation() {
  cpu_state.register_file[0x1] = 0x5;
  cpu_state.register_file[0x2] = 0x2;

  _execute_instruction(0x8125);

  TEST_ASSERT_EQUAL_HEX8(0x3, cpu_state.register_file[0x1]);
}


void test_add_operation() {
  cpu_state.register_file[0x1] = 0x5;
  cpu_state.register_file[0x2] = 0x2;

  _execute_instruction(0x8124);

  TEST_ASSERT_EQUAL_HEX8(0x7, cpu_state.register_file[0x1]);
}


void test_add_sets_vf_to_1_on_overflow() {
  cpu_state.register_file[0x1] = 0xFF;
  cpu_state.register_file[0x2] = 0x1;

  _execute_instruction(0x8124);
  
  TEST_ASSERT_EQUAL_HEX8(0x1, cpu_state.register_file[0xF]);
}


void test_shr_instruction_sets_vf_to_1_when_lsb_is_1() {
  cpu_state.register_file[0x1] = 0xFF;

  _execute_instruction(0x8106);

  TEST_ASSERT_EQUAL_HEX8(0x1, cpu_state.register_file[0xF]);
}


void test_shl_instruction_sets_vf_to_1_when_msb_is_1() {
  cpu_state.register_file[0x1] = 0xFF;

  _execute_instruction(0x810E);

  TEST_ASSERT_EQUAL_HEX8(0x1, cpu_state.register_file[0xF]);
}


void test_jp_instruction() {
  _execute_instruction(0x1e61);

  TEST_ASSERT_EQUAL_HEX16(0x0e61, cpu_state.pc);
}


void test_se_constant_instruction() {
  cpu_state.register_file[0x3] = 0x9;

  _execute_instruction(0x3309);

  TEST_ASSERT_EQUAL_HEX16(PROGRAM_START + 2, cpu_state.pc);
}


void test_sne_constant_instruction() {
  cpu_state.register_file[0x3] = 0x5;

  _execute_instruction(0x4302);

  TEST_ASSERT_EQUAL_HEX16(PROGRAM_START + 2, cpu_state.pc);
}


void test_se_register_instruction() {
  cpu_state.register_file[0x0] = 0x5;
  cpu_state.register_file[0x2] = 0x5;

  _execute_instruction(0x5020);

  TEST_ASSERT_EQUAL_HEX16(PROGRAM_START + 2, cpu_state.pc);
}


void test_sne_register_instruction() {
  cpu_state.register_file[0x0] = 0x3;
  cpu_state.register_file[0x3] = 0x2;

  _execute_instruction(0x9030);

  TEST_ASSERT_EQUAL_HEX16(PROGRAM_START + 2, cpu_state.pc);
}


void test_ld_bcd_instruction() {
  cpu_state.register_file[0x7] = 238;

  _execute_instruction(0xF733);

  TEST_ASSERT_EQUAL_INT8_ARRAY(&((uint8_t[]){0x02, 0x03, 0x08}), memory, 3);
}


void test_ld_registers_instruction() {
  memcpy(memory, &((uint8_t[]){1, 2, 3, 4}), 4);

  _execute_instruction(0xF465);

  TEST_ASSERT_EQUAL_INT8_ARRAY(&((uint8_t[]){1, 2, 3, 4}),
                               cpu_state.register_file, 4);
}


void test_ld_memory_instruction() {
  memcpy(cpu_state.register_file, &((uint8_t[]){1, 2, 3, 4}), 4);

  _execute_instruction(0xF455);

  TEST_ASSERT_EQUAL_INT8_ARRAY(&((uint8_t[]){1, 2, 3, 4}),
                               memory, 4);
}


void test_random_instruction_actually_generates_random_numbers() {
  _execute_instruction(0xC0FF);
  _execute_instruction(0xC1FF);

  TEST_ASSERT_NOT_EQUAL_INT8(cpu_state.register_file[0x0],
                             cpu_state.register_file[0x1]);
}


void test_sending_key() {
  send_key(0xF);

  TEST_ASSERT_EQUAL_HEX16(0xF, __builtin_ctz(cpu_state.key_register));
}


void test_load_key_instruction() {
  memory[PROGRAM_START] = 0xF9;
  memory[PROGRAM_START + 1] = 0x0A;

  _execute_instruction(0xF90A);

  send_key(0x5);

  TEST_ASSERT_EQUAL_HEX16(0x5, cpu_state.register_file[0x9]);
}


void test_draw_instruction() {
  // TODO: Implementar esto
}


int main() {
  UNITY_BEGIN();

  RUN_TEST(test_ld_instruction);
  RUN_TEST(test_sub_instruction_sets_vf_to_1_when_no_borrow);
  RUN_TEST(test_add_operation);
  RUN_TEST(test_sub_operation);
  RUN_TEST(test_add_sets_vf_to_1_on_overflow);
  RUN_TEST(test_shr_instruction_sets_vf_to_1_when_lsb_is_1);
  RUN_TEST(test_shl_instruction_sets_vf_to_1_when_msb_is_1);
  RUN_TEST(test_jp_instruction);
  RUN_TEST(test_se_constant_instruction);
  RUN_TEST(test_sne_constant_instruction);
  RUN_TEST(test_se_register_instruction);
  RUN_TEST(test_sne_register_instruction);
  RUN_TEST(test_ld_bcd_instruction);
  RUN_TEST(test_ld_registers_instruction);
  RUN_TEST(test_ld_memory_instruction);
  RUN_TEST(test_random_instruction_actually_generates_random_numbers);
  RUN_TEST(test_sending_key);
  RUN_TEST(test_load_key_instruction);

  UNITY_END();
}

