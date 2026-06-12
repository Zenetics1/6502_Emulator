#ifndef MEMORY.H
#define MEMORY.H

#include <stdint.h>

extern uint8_t cpu_ram[65536];

uint8_t read_mem(uint16_t addr);

void write_mem(uint16_t addr, uint8_t value);

#endif