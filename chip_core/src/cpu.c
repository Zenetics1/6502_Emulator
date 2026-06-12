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

uint16_t Immediate_Mode(MOS6502 *cpu){
    uint16_t effective_address = cpu->PC++;
    return effective_address;
}
uint16_t Zero_Page_Mode(MOS6502 *cpu){
    uint16_t effective_address = read_mem(cpu->PC++);
    return effective_address;
}
uint16_t Zero_PageX_Mode(MOS6502 *cpu){
    uint16_t base_address = Zero_Page_Mode(cpu);
    uint16_t effective_address = (base_address + cpu->X) & 0x00FF;
    return effective_address;
}
uint16_t Zero_PageY_Mode(MOS6502 *cpu){
    uint16_t base_address = Zero_Page_Mode(cpu);
    uint16_t effective_address = (base_address + cpu->Y) & 0x00FF;
    return effective_address;
}
uint16_t Absolute_Mode(MOS6502 *cpu){
    uint8_t low_byte = read_mem(cpu->PC++);
    uint8_t high_byte = read_mem(cpu->PC++);
    uint16_t effective_address = (high_byte << 8) | low_byte;
    return effective_address;
}
uint16_t AbsoluteX_Mode(MOS6502 *cpu){
    uint16_t base_address = Absolute_Mode(cpu);
    uint16_t effective_address = base_address + cpu->X;
    if((base_address & 0xFF00) != (effective_address & 0xFF00)){
        cpu->cycles_left += 1;
    }
    return effective_address;
}
uint16_t AbsoluteY_Mode(MOS6502 *cpu){
    uint16_t base_address = Absolute_Mode(cpu);
    uint16_t effective_address = base_address + cpu->Y;
    if((base_address & 0xFF00) != (effective_address & 0xFF00)){
        cpu->cycles_left += 1;
    }
    return effective_address;
}
uint16_t Absolute_Indirect_Mode(MOS6502 *cpu){
    uint16_t base_address = Absolute_Mode(cpu);
    uint8_t low_byte = read_mem(base_address);
    uint8_t high_byte = read_mem(base_address + 1);
    uint16_t effective_address = (high_byte << 8) | low_byte;

    return effective_address;
}
uint16_t Relative_Mode(MOS6502 *cpu){
    int8_t offset = (int8_t)(cpu->PC++);
    uint16_t PC_target = cpu->PC + offset;

    return PC_target;
}

uint16_t Indexed_Indirect_Mode(MOS6502 *cpu){
    uint8_t base_address = read_mem(cpu->PC++);
    uint8_t Pointer = (base_address + cpu->X) & 0xFF;
    uint8_t low_byte = read_mem(Pointer);
    uint8_t high_byte = read_mem(Pointer+1) & 0xFF;
    uint16_t effective_address = (high_byte << 8) | low_byte;

    return effective_address;
}
uint16_t Indirect_Indexed_Mode(MOS6502 * cpu){
    uint8_t Pointer = read_mem(cpu->PC++);
    uint8_t low_byte = read_mem(Pointer);
    uint8_t high_byte = read_mem(Pointer+1) & 0xFF;
    uint16_t base_address = (high_byte << 8) | low_byte;
    uint16_t effective_address = base_address + cpu->Y;
    if((base_address & 0xFF00) != (effective_address & 0xFF00)){
        cpu->cycles_left += 1;
    }
    return effective_address;
}