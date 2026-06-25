#include "cpu.h"

int main (void){
    MOS6502 cpu;
    cpu.A = 0;
    cpu.X = 0;
    cpu.Y = 0;
    cpu.S = 0xFF;
    cpu.P = 0x24;
    cpu.cycles_left = 0;

    cpu.PC = 0x0400;
    while (1)
    {
    }
    

    return 0;
}