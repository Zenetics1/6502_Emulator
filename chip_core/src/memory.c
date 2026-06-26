#include "memory.h"
#include <stdint.h>
#include <stdio.h>
uint8_t cpu_ram[65536];

uint8_t read_mem(uint16_t addr){
    return cpu_ram[addr];
}

void write_mem(uint16_t addr, uint8_t value){
    cpu_ram[addr] = value;
}

int load_rom(const char *path){
    FILE *f = fopen(path, "rb");
    if (!f) return -1;
    fread(cpu_ram, 1, 65536, f);
    fclose(f);
    return 0;
}