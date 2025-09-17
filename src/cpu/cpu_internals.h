#ifndef CPU_INTERNALS_H
#define CPU_INTERNALS_H

#include <stdbool.h>
#include <stdint.h>

#define DELAY_SPEED 60.0f
#define CLOCK_SPEED 500.0f
#define PROGRAM_START 0x200

#define DEBUG

extern unsigned char memory[4096];
extern uint16_t stack[16];


#define get_opcode(instruction)\
  (instruction >> 12)

#define get_register_x(instruction)\
  ((instruction & 0x0F00) >> 8)

#define get_register_y(instruction)\
  ((instruction & 0x00F0) >> 4)

#define get_byte_constant(instruction)\
  (instruction & 0x00FF)

#define get_address(instruction)\
  (instruction & 0x0FFF)

#define get_last_nibble(instruction)\
  (instruction & 0x000F)

typedef uint16_t instr_t;

#ifdef DEBUG
  bool _execute_instruction(instr_t instruction);
#endif



typedef struct CPUState {
  uint8_t register_file[16];
  uint16_t index;
  uint16_t pc;
  uint16_t key_register;
  uint8_t sp;
  bool running;
  bool waiting_for_key;
  uint8_t delay_timer;
  uint8_t sound_timer;
} CPUState;

extern CPUState cpu_state;

#endif
