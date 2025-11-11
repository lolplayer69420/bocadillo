#include "../cpu/cpu_internals.h"
#include "raylib.h"
#include "disassembler.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <raygui.h>
#include <stdint.h>
#include <stdio.h>

// TODO: Terminar esto

#define MAX_INSTRUCTIONS_ON_SCREEN 26
#define INLINE static inline __attribute__((always_inline))


INLINE void _disassemble_arithmetic_instruction(uint16_t address, instr_t src,
                                                char *dest) {
  uint8_t register_x = get_register_x(src);
  uint8_t register_y = get_register_y(src);
  uint8_t operation = get_last_nibble(src);

  switch (operation) {
    case 0x00:
      sprintf(dest, "0x%04X 0x%04X ld v%X, v%X", address, src, register_x,
              register_y);
      break;
    case 0x01:
      sprintf(dest, "0x%04X 0x%04X or v%X, v%X", address, src, register_x,
              register_y);
      break;
    case 0x02:
      sprintf(dest, "0x%04X 0x%04X and v%X, v%X", address, src, register_x,
              register_y);
      break;
    case 0x03:
      sprintf(dest, "0x%04X 0x%04X xor v%X, v%X", address, src, register_x,
              register_y);
      break;
    case 0x04:
      sprintf(dest, "0x%04X 0x%04X add v%X, v%X", address, src, register_x,
              register_y);
      break; 
    case 0x05:
      sprintf(dest, "0x%04X 0x%04X sub v%X, v%X", address, src, register_x,
              register_y);
      break;
    case 0x06:
      sprintf(dest, "0x%04X 0x%04X shr v%X, v%X", address, src, register_x,
              register_y);
      break;
    case 0x07:
      sprintf(dest, "0x%04X 0x%04X subn v%X, v%X", address, src, register_x,
              register_y);
      break;
    case 0x0E:
      sprintf(dest, "0x%04X 0x%04X shl v%X, v%X", address, src, register_x,
              register_y);
      break;
  }
}


INLINE void _disassemble_memory_instruction(uint16_t address, instr_t src,
                                            char *dest) {
  uint8_t register_x = get_register_x(src);
  uint8_t operation = get_byte_constant(src);

  switch (operation) {
    case 0x07:
      sprintf(dest, "0x%04X 0x%04X ld v%X, DT", address, src, register_x);
      break;
    case 0x0A:
      sprintf(dest, "0x%04X 0x%04X ld v%X, K", address, src, register_x);
      break;
    case 0x15:
      sprintf(dest, "0x%04X 0x%04X ld DT, v%X", address, src, register_x);
      break;
    case 0x18:
      sprintf(dest, "0x%04X 0x%04X ld ST, v%X", address, src, register_x);
      break;
    case 0x1E:
      sprintf(dest, "0x%04X 0x%04X add I, v%X", address, src, register_x);
      break;
    case 0x29:
      sprintf(dest, "0x%04X 0x%04X ld F, v%X", address, src, register_x);
      break;
    case 0x33:
      sprintf(dest, "0x%04X 0x%04X ld B, v%X", address, src, register_x);
      break;
    case 0x55:
      sprintf(dest, "0x%04X 0x%04X ld [I], v%X", address, src, register_x);
      break;
    case 0x65:
      sprintf(dest, "0x%04X 0x%04X ld v%X, [I]", address, src, register_x);
      break;
  }
}


INLINE void _disassemble_instruction(uint16_t instruction_address, instr_t src,
                                     char *dest) {
  uint8_t opcode = get_opcode(src);
  uint8_t register_x = get_register_x(src);
  uint8_t register_y = get_register_y(src);
  uint16_t address = get_address(src);
  uint8_t byte_constant = get_byte_constant(src);
  uint8_t last_nibble = get_last_nibble(src);

  switch (opcode) {
    case 0x00:
      if (byte_constant == 0xEE) {
        sprintf(dest, "0x%04X 0x%04X ret", instruction_address, src);
      } else if (byte_constant == 0xE0) {
        sprintf(dest, "0x%04X 0x%04X cls", instruction_address, src);
      } else {
        sprintf(dest, "0x%04X 0x%04X unknown", instruction_address, src);
      }
      break;
    case 0x01:
      sprintf(dest, "0x%04X 0x%04X jp 0x%03X", instruction_address, src, address);
      break;
    case 0x02:
      sprintf(dest, "0x%04X 0x%04X call 0x%03X", instruction_address, src, address);
      break;
    case 0x03:
      sprintf(dest, "0x%04X 0x%04X se v%X, 0x%02x", instruction_address, src,
              register_x, byte_constant);
      break;
    case 0x04:
      sprintf(dest, "0x%04X 0x%04X sne v%X, 0x%02x", instruction_address, src,
              register_x, byte_constant);
      break;
    case 0x05:
      sprintf(dest, "0x%04X 0x%04X se v%X, v%X", instruction_address, src,
              register_x, register_x);
      break;
    case 0x06:
      sprintf(dest, "0x%04X 0x%04X ld v%X, 0x%02X", instruction_address, src,
              register_x, byte_constant);
      break;
    case 0x07:
      sprintf(dest, "0x%04X 0x%04X add v%X, 0x%02X", instruction_address, src,
              register_x, byte_constant);
      break;
    case 0x08:
      _disassemble_arithmetic_instruction(instruction_address, src, dest);
      break;
    case 0x09:
      sprintf(dest, "0x%04X 0x%04X sne v%X, v%X", instruction_address, src,
              register_x, register_y);
      break;
    case 0x0A:
      sprintf(dest, "0x%04X 0x%04X ld I, 0x%03X", instruction_address, src,
              address);
      break;
    case 0x0B:
      sprintf(dest, "0x%04X 0x%04X jp v0, 0x%03X", instruction_address, src,
              address);
      break;
    case 0x0C:
      sprintf(dest, "0x%04X 0x%04X rnd v%X 0x%02X", instruction_address, src, 
              register_x, byte_constant);
      break;
    case 0x0D:
      sprintf(dest, "0x%04X 0x%04X drw v%X, v%X, %X", instruction_address, src,
              register_x, register_y, last_nibble);
      break;
    case 0x0E:
      if (byte_constant == 0x9E) {
        sprintf(dest, "0x%04X 0x%04X skp v%X", instruction_address, src,
                register_x);
      } else if (byte_constant == 0xA1) {
        sprintf(dest, "0x%04X 0x%04X sknp v%X", instruction_address, src,
                register_x);
      } else {
        sprintf(dest, "0x%04X 0x%04X unknown", instruction_address, src);
      }
      break;
    case 0x0F:
      _disassemble_memory_instruction(instruction_address, src, dest);
      break;
  }
}


void _show_instruction_stream() {
  char disassembled_instruction[64];
  uint16_t instruction_slice_start;

  if (cpu_state.pc == PROGRAM_START) {
    instruction_slice_start = PROGRAM_START;
  } else {
    instruction_slice_start = cpu_state.pc - MAX_INSTRUCTIONS_ON_SCREEN;
  }

  for (
    uint16_t i = instruction_slice_start, j = 0;
    j < MAX_INSTRUCTIONS_ON_SCREEN;
    i += sizeof(instr_t), ++j
  ) {
    _disassemble_instruction(i, ntohs(*((instr_t*)&memory[i])),
                             disassembled_instruction);

    int old_color = GuiGetStyle(LABEL, TEXT_COLOR_NORMAL);

    if (i == cpu_state.pc) {
      GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, 0x00FF00FF);
    }

    GuiLabel((Rectangle){20 * 10, (j * 10) - 10, 30 * 10, 10 * 10},
             disassembled_instruction);
    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, old_color);
  }
}


void show_disassembler_window() {
    GuiWindowBox(
    (Rectangle){10 * 10, 0, 32 * 10, 30 * 10},
    "Disassembler"
  );

  _show_instruction_stream();
}

