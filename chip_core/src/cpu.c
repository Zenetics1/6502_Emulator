#include "include/cpu.h"

void CLOCK(MOS6502 *cpu){
    if(cpu->cycles_left == 0){
        cpu->IR = cpu->data_pin;
        
        opcode_execute(cpu, cpu->IR);
    }

    if(cpu->cycles_left == 1){
        cpu->address_pin = cpu->PC;
        cpu->read_write = true;
        cpu->PC++;
    }

    if(cpu->cycles_left > 0){
        cpu->cycles_left--;
    }
}

uint8_t opcode_execute(MOS6502 *cpu, uint8_t IR_OPCODE){

}