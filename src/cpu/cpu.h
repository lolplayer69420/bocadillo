#ifndef CPU_H
#define CPU_H

#include <stdint.h>

typedef void(*SoundCallback)();

void load_program(const char *path);
void initialize_cpu(SoundCallback sound_callback);
void send_key(uint8_t key);

#endif
