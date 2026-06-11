#include "include/cpu.h"

void CLOCK(MOS6502 *cpu){
    if(cpu->cycles_left == 0){
        cpu->IR = cpu->data_pin;
        
        opcode_execute(cpu, cpu->IR);
    }

    if(cpu->cycles_left > 0){
        cpu->cycles_left--;
    }

    if(cpu->cycles_left == 0){
        cpu->address_pin = cpu->PC;
        cpu->read_write = true;
        cpu->PC++;
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


uint8_t Instr_execute(MOS6502 *cpu, uint8_t IR_OPCODE){
    //TODO: implement MUX for decoding and understand what each opcode is and where it goes.
}