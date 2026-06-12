#include "include/memory.h"

uint8_t read_mem(uint16_t addr){
    return cpu_ram[addr];
}

void write_mem(uint16_t addr, uint8_t value){
    cpu_ram[addr] = value;
}