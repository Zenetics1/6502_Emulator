#include "memory.h"

uint8_t cpu_ram[65536];

uint8_t read_mem(uint16_t addr){
    return cpu_ram[addr];
}

void write_mem(uint16_t addr, uint8_t value){
    cpu_ram[addr] = value;
}