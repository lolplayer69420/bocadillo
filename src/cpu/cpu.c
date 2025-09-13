#include "cpu_internals.h"
#include "framebuffer.h"
#include "cpu.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define INLINE static inline __attribute__((always_inline))

unsigned char memory[4096];
uint16_t stack[16];

uint8_t sprites[] = {
  0b11110000,
  0b10010000,
  0b10010000,
  0b10010000,
  0b11110000,
  0b00100000,
  0b01100000,
  0b00100000,
  0b00100000,
  0b01110000,
  0b11110000,
  0b00010000,
  0b11110000,
  0b10000000,
  0b11110000,
  0b11110000,
  0b00010000,
  0b11110000,
  0b00010000,
  0b11110000,
  0b10010000,
  0b10010000,
  0b11110000,
  0b00010000,
  0b00010000,
  0b11110000,
  0b10000000,
  0b11110000,
  0b00010000,
  0b11110000,
  0b11110000,
  0b10000000,
  0b11110000,
  0b10010000,
  0b11110000,
  0b11110000,
  0b00010000,
  0b00100000,
  0b01000000,
  0b01000000,
  0b11110000,
  0b10010000,
  0b11110000,
  0b10010000,
  0b11110000,
  0b11110000,
  0b10010000,
  0b11110000,
  0b00010000,
  0b11110000,
  0b11110000,
  0b10010000,
  0b11110000,
  0b10010000,
  0b10010000,
  0b11100000,
  0b10010000,
  0b11100000,
  0b10010000,
  0b11100000,
  0b11110000,
  0b10000000,
  0b10000000,
  0b10000000,
  0b11110000,
  0b11100000,
  0b10010000,
  0b10010000,
  0b10010000,
  0b11100000,
  0b11110000,
  0b10000000,
  0b11110000,
  0b10000000,
  0b10000000
};

CPUState cpu_state;


void load_program(const char *path) {
  uint16_t current_address = PROGRAM_START;
  FILE *rom_file = fopen(path, "rb");
  char readed;

  while (fread(&readed, 1, 1, rom_file)) {
    memory[current_address++] = readed;
  }
}


void initialize_cpu() {
  memset(&cpu_state, 0, sizeof(CPUState));
  memcpy(memory, sprites, sizeof(sprites));

  cpu_state.pc = PROGRAM_START;
}


INLINE void _do_ld_kk_instruction(uint8_t dest, uint8_t value) {
  cpu_state.register_file[dest] = value;
}


INLINE void _do_add_byte_instruction(uint8_t dest, uint8_t value) {
  cpu_state.register_file[dest] += value;
}


INLINE void _do_operation_on_registers(uint8_t x, uint8_t y, uint8_t operation) {
  uint16_t result_16_bit = 0;

  switch (operation) {
    case 0x0:
      cpu_state.register_file[x] = cpu_state.register_file[y];
      break;
    case 0x1:
      cpu_state.register_file[x] |= cpu_state.register_file[y];
      break;
    case 0x3:
      cpu_state.register_file[x] ^= cpu_state.register_file[y];
      break;
    case 0x4:
      result_16_bit = cpu_state.register_file[x] + cpu_state.register_file[y];

      if (result_16_bit & 0xFF00) {
        cpu_state.register_file[0xF] = 1;
      }

      cpu_state.register_file[x] = result_16_bit;
      break;
    case 0x5:
      cpu_state.register_file[x] -= cpu_state.register_file[y];
      cpu_state.register_file[0xF] = ~(cpu_state.register_file[x] >> 7) & 0x1;
      break;
    case 0x6:
      cpu_state.register_file[x] >>= 1;
      cpu_state.register_file[0xF] = cpu_state.register_file[x] & 0x1;
      break;
    case 0x7:
      cpu_state.register_file[y] -= cpu_state.register_file[x];
      cpu_state.register_file[0xF] = ~(cpu_state.register_file[x] >> 7) & 0x1;
      break;
    case 0xE:
      cpu_state.register_file[x] <<= 1;
      cpu_state.register_file[0xF] = cpu_state.register_file[x] & 0x80;
      cpu_state.register_file[0xF] >>= 7;
      break;
  }
}


INLINE uint8_t _get_pressed_key() {
  return __builtin_ctz(cpu_state.key_register);
}


void _handle_key_press() {
  uint8_t key = _get_pressed_key();
  instr_t last_instruction = ntohs(*((instr_t*)&memory[cpu_state.pc]));

  if (cpu_state.waiting_for_key) {
    uint8_t register_x = get_register_x(last_instruction);
    cpu_state.register_file[register_x] = key;
    cpu_state.waiting_for_key = false;
    cpu_state.key_register = 0;
  }
}

INLINE void _do_branches(uint8_t operation, uint8_t x, uint8_t y,
                         uint16_t constant) {
  switch (operation) {
    case 0x01:
      cpu_state.pc = constant;
      break;
    case 0x03:
      if (cpu_state.register_file[x] == ((uint8_t)constant)) {
        cpu_state.pc += 2;
      }

      break;
    case 0x04:
      if (cpu_state.register_file[x] != ((uint8_t)constant)) {
        cpu_state.pc += 2;
      }

      break;
    case 0x05:
      if (cpu_state.register_file[x] == cpu_state.register_file[y]) {
        cpu_state.pc += 2;
      }

      break;
    case 0x09:
      if (cpu_state.register_file[x] != cpu_state.register_file[y]) {
        cpu_state.pc += 2;
      }

      break;
    case 0x0B:
      cpu_state.pc = constant + cpu_state.register_file[0x0];
      break;
    case 0x0E:
      if (constant == 0x9E) {
        if (!cpu_state.key_register) {
          return;
        }

        if (_get_pressed_key() == cpu_state.register_file[x]) {
          cpu_state.pc += 2;
        }
      }

      if (constant == 0xA1) {
        if (!cpu_state.key_register) {
          return;
        }

        if (_get_pressed_key() != cpu_state.register_file[x]) {
          cpu_state.pc += 2;
        }
      }
      break;
  }
}


INLINE void _do_ld_address_instruction(uint16_t address) {
  cpu_state.index = address;
}


INLINE void _do_memory_instructions(uint8_t register_x, uint8_t operation) {
  uint8_t tmp;

  switch (operation) {
    case 0x07:
      cpu_state.register_file[register_x] = cpu_state.delay_timer;
      break;
    case 0x0A:
      cpu_state.waiting_for_key = true;
      break;
    case 0x15:
      cpu_state.delay_timer = cpu_state.register_file[register_x];
  break;
    case 0x18:
      cpu_state.sound_timer = cpu_state.register_file[register_x];
      break;
    case 0x1E:
      cpu_state.index += cpu_state.register_file[register_x];
      break;
    case 0x29:
      cpu_state.index = 0;
      cpu_state.index = cpu_state.register_file[register_x] * 5;
      break;
    case 0x33:
      tmp = cpu_state.register_file[register_x];

      memory[cpu_state.index] = tmp / 100;
      memory[++cpu_state.index] = (tmp % 100) / 10;
      memory[++cpu_state.index] = tmp % 10;
      break;
    case 0x55:
      for (uint8_t i = 0; i < register_x; ++i) {
        memory[cpu_state.index++] = cpu_state.register_file[i];
      }
      break;
    case 0x65:
      for (uint8_t i = 0; i < register_x; ++i) {
        cpu_state.register_file[i] = memory[cpu_state.index++];
      }
      break;
  }
}


INLINE void _do_random_instruction(uint8_t register_x, uint8_t byte) {
  cpu_state.register_file[register_x] = rand() & byte;
  srand(time(NULL));
}


INLINE void _do_call_and_return(uint8_t opcode, uint16_t call_address) {
  uint16_t return_address;

  switch (opcode) {
    case 0x20:
      stack[cpu_state.sp] = cpu_state.pc;
      cpu_state.pc = call_address;
      ++cpu_state.sp;
      break;
    case 0x0:
      cpu_state.pc = stack[cpu_state.sp];
      --cpu_state.sp;
      break;
  }
}


INLINE void _do_draw(uint8_t register_x, uint8_t register_y, uint8_t size) {
  uint8_t x_pos = cpu_state.register_file[register_x];
  uint8_t y_pos = cpu_state.register_file[register_y];

  cpu_state.register_file[0xF] = _draw_sprite(&memory[cpu_state.index],
                                              x_pos, y_pos, size);
}


void send_key(uint8_t key) {
  cpu_state.key_register = 1;
  cpu_state.key_register <<= key;
  _handle_key_press();
}


void _execute_instruction(instr_t instruction) {
  uint8_t opcode = get_opcode(instruction);
  uint8_t register_x = get_register_x(instruction);
  uint8_t register_y = get_register_y(instruction);
  uint8_t byte_constant = get_byte_constant(instruction);
  uint16_t address = get_address(instruction);
  uint8_t last_nibble = get_last_nibble(instruction);

  switch (opcode) {
    case 0x06:
      _do_ld_kk_instruction(register_x, byte_constant);
      break;
    case 0x07:
      _do_add_byte_instruction(register_x, byte_constant);
      break;
    case 0x08:
      _do_operation_on_registers(register_x, register_y, last_nibble);
      break;
    case 0x01:
    case 0x03:
    case 0x04:
    case 0x05:
    case 0x09:
    case 0x0B:
      _do_branches(opcode,register_x, register_y, address);
      break;
    case 0x0A:
      _do_ld_address_instruction(address);
      break;
    case 0x0F:
      _do_memory_instructions(register_x, byte_constant);
      break;
    case 0x0C:
      _do_random_instruction(register_x, byte_constant);
      break;
    case 0x20:
    case 0x00:
      if (byte_constant == 0xE0) {
        _clear_display();
      } else {
        _do_call_and_return(opcode, address);
      }
      break;
    case 0xD:
      _do_draw(register_x, register_y, last_nibble);
      break;
  }
}


void do_cycle() {
  // TODO: Implementar esto
}
