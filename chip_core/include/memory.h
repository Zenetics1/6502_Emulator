#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

extern uint8_t cpu_ram[65536];

uint8_t read_mem(uint16_t addr);

void write_mem(uint16_t addr, uint8_t value);

int load_rom(const char *path);

#endif