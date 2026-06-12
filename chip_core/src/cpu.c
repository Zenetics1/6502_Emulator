#include "include/cpu.h"
#include "src/memory.c"
void CLOCK(MOS6502 *cpu){
    if(cpu->cycles_left == 0){
        cpu->IR = read_mem(cpu->PC++);
        
        cpu->cycles_left += LUT[cpu->IR].total_cycles;

        cpu->cycles_left += LUT[cpu->IR].op_func(cpu);
    }

    if(cpu->cycles_left > 0){
        cpu->cycles_left--;
    }
}

void update_Z_N_flags(MOS6502 *cpu, uint8_t reg_result){
    if(reg_result == 0){
        cpu->P |= FLAG_ZERO;
    } else {
        cpu->P &= ~FLAG_ZERO;
    }

    if((reg_result & 0x80) != 0){
        cpu->P |= FLAG_NEGATIVE;
    } else{
        cpu->P &= ~FLAG_NEGATIVE;
    }
}