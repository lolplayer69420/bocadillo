#ifndef CPU_H
#define CPU_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void load_program(const char *path);
void initialize_cpu();
void send_key(uint8_t key);
void do_cycle();
void halt_cpu();
void resume_cpu();
bool is_cpu_running();
void do_n_steps(size_t steps);
void reset_cpu();

#endif
