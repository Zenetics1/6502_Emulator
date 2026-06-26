#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "cpu.h"
#include "memory.h"

int main (int argc, char *argv[]){
    MOS6502 cpu;
    cpu.A = 0;
    cpu.X = 0;
    cpu.Y = 0;
    cpu.S = 0xFF;
    cpu.P = 0x24;
    cpu.cycles_left = 0;

    cpu.PC = 0x0400;

    if(argc == 3 && strcmp(argv[1], "--test") == 0){
        if(load_rom(argv[2]) != 0){
            fprintf(stderr, "Cannot open %s\n", argv[2]);
            return 1;
        }
    }
    uint16_t pc_prev;
    while (1)
    {   
        if(cpu.cycles_left == 0){
            pc_prev = cpu.PC;
        }
        CLOCK(&cpu);
        if(cpu.PC == pc_prev){
            if(cpu.PC == 0x3469){
                fprintf(stdout, "ALL TESTS PASSED");
            } else{
                fprintf(stderr, "TEST FAILED at 0x%04X\n", cpu.PC);
                break;
            }
        }
    }
    

    return 0;
}