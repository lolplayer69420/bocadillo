#ifndef CPU_H
#define CPU_H

#include <stdint.h>

void load_program(const char *path);
void initialize_cpu();
void send_key(uint8_t key);

#endif
