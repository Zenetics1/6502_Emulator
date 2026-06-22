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

void LDA(MOS6502 *cpu, uint16_t address){
        cpu->A = read_mem(address);
        if(cpu->A == 0){
            cpu->P |= FLAG_ZERO;
        } else {
            cpu->P &= ~FLAG_ZERO;
        }
        if(cpu->A & 0x80){
            cpu->P |= FLAG_NEGATIVE;
        } else {
            cpu->P &= ~FLAG_NEGATIVE;
        }
}
void LDX(MOS6502 *cpu, uint16_t address){
        cpu->X = read_mem(address);
        if(cpu->X == 0){
            cpu->P |= FLAG_ZERO;
        } else {
            cpu->P &= ~FLAG_ZERO;
        }
        if(cpu->X & 0x80){
            cpu->P |= FLAG_NEGATIVE;
        } else {
            cpu->P &= ~FLAG_NEGATIVE;
        }
}
void LDY(MOS6502 *cpu, uint16_t address){
        cpu->Y = read_mem(address);
        if(cpu->Y == 0){
            cpu->P |= FLAG_ZERO;
        } else {
            cpu->P &= ~FLAG_ZERO;
        }
        if(cpu->Y & 0x80){
            cpu->P |= FLAG_NEGATIVE;
        } else {
            cpu->P &= ~FLAG_NEGATIVE;
        }
}
void LSR(MOS6502 *cpu){
    if(cpu->A & 0x01){
        cpu->P |= FLAG_CARRY;
    } else {
        cpu->P &= ~FLAG_CARRY;
    }
    cpu->A >>= 1;

    if(cpu->A == 0){
        cpu->P |= FLAG_ZERO;
    } else {
        cpu->P &= ~FLAG_ZERO;
    }

    cpu->P &= ~FLAG_NEGATIVE;
}
void STA(MOS6502 *cpu, uint16_t address){
    write_mem(address, cpu->A);
}
void STX(MOS6502 *cpu, uint16_t address){
    write_mem(address, cpu->X);
}
void STY(MOS6502 *cpu, uint16_t address){
    write_mem(address, cpu->Y);
}
void TAX(MOS6502 *cpu){
    cpu->X = cpu->A;
    if(cpu->X == 0){
        cpu->P |= FLAG_ZERO;
    } else {
        cpu->P &= ~FLAG_ZERO;
    }
    if(cpu->X & 0x80){
        cpu->P |= FLAG_NEGATIVE;
    } else {
        cpu->P &= ~FLAG_NEGATIVE;
    }

}
void TAY(MOS6502 *cpu){
    cpu->Y = cpu->A;
    if(cpu->Y == 0){
        cpu->P |= FLAG_ZERO;
    } else {
        cpu->P &= ~FLAG_ZERO;
    }
    if(cpu->Y & 0x80){
        cpu->P |= FLAG_NEGATIVE;
    } else {
        cpu->P &= ~FLAG_NEGATIVE;
    }
}
void TXA(MOS6502 *cpu){
    cpu->A = cpu->X;
    if(cpu->A == 0){
        cpu->P |= FLAG_ZERO;
    } else {
        cpu->P &= ~FLAG_ZERO;
    }
    if(cpu->A & 0x80){
        cpu->P |= FLAG_NEGATIVE;
    } else {
        cpu->P &= ~FLAG_NEGATIVE;
    }
}
void TYA(MOS6502 *cpu){
 cpu->A = cpu->Y;
    if(cpu->A == 0){
        cpu->P |= FLAG_ZERO;
    } else {
        cpu->P &= ~FLAG_ZERO;
    }
    if(cpu->A & 0x80){
        cpu->P |= FLAG_NEGATIVE;
    } else {
        cpu->P &= ~FLAG_NEGATIVE;
    }
}
void TSX(MOS6502 *cpu){
    cpu->X = cpu->S;
    if(cpu->X == 0){
        cpu->P |= FLAG_ZERO;
    } else {
        cpu->P &= ~FLAG_ZERO;
    }
    if(cpu->X & 0x80){
        cpu->P |= FLAG_NEGATIVE;
    } else {
        cpu->P &= ~FLAG_NEGATIVE;
    }
}
void TXS(MOS6502 *cpu){
    cpu->S = cpu->X;
    if(cpu->S == 0){
        cpu->P |= FLAG_ZERO;
    } else {
        cpu->P &= ~FLAG_ZERO;
    }
    if(cpu->S & 0x80){
        cpu->P |= FLAG_NEGATIVE;
    } else {
        cpu->P &= ~FLAG_NEGATIVE;
    }
}

void PHA(MOS6502 *cpu){
    uint16_t stack_address = 0x0100 | cpu->S;
    write_mem(stack_address, cpu->A);
    cpu->S--;
} 
void PHP(MOS6502 *cpu){
    uint16_t stack_address = 0x0100 | cpu->S;
    write_mem(stack_address, cpu->P);
    cpu->S--;
}
void PLA(MOS6502 *cpu){
    cpu->S++;
    uint16_t stack_address = 0x0100 | cpu->S;
    cpu->A = read_mem(stack_address);
    if(cpu->A == 0){
        cpu->P |= FLAG_ZERO;
    } else {
        cpu->P &= ~FLAG_ZERO;
    }
    if(cpu->A & 0x80){
        cpu->P |= FLAG_NEGATIVE;
    } else {
        cpu->P &= ~FLAG_NEGATIVE;
    }
}
void PLP(MOS6502 *cpu){
    cpu->S++;
    uint16_t stack_address = 0x0100 | cpu->S;
    cpu->P = (read_mem(stack_address) & ~FLAG_BREAK) | FLAG_UNUSED;
}
void AND(MOS6502 *cpu, uint16_t address){
    uint8_t content = read_mem(address);
    cpu->A &= content;
    if(cpu->A == 0){
        cpu->P |= FLAG_ZERO;
    } else {
        cpu->P &= ~FLAG_ZERO;
    }
    if(cpu->A & 0x80){
        cpu->P |= FLAG_NEGATIVE;
    } else {
        cpu->P &= ~FLAG_NEGATIVE;
    }
}
void EOR(MOS6502 *cpu, uint16_t address){
    uint8_t content = read_mem(address);
    cpu->A ^= content;
    if(cpu->A == 0){
        cpu->P |= FLAG_ZERO;
    } else {
        cpu->P &= ~FLAG_ZERO;
    }
    if(cpu->A & 0x80){
        cpu->P |= FLAG_NEGATIVE;
    } else {
        cpu->P &= ~FLAG_NEGATIVE;
    }
}
void ORA(MOS6502 *cpu, uint16_t address){
    uint8_t content = read_mem(address);
    cpu->A |= content;
    if(cpu->A == 0){
        cpu->P |= FLAG_ZERO;
    } else {
        cpu->P &= ~FLAG_ZERO;
    }
    if(cpu->A & 0x80){
        cpu->P |= FLAG_NEGATIVE;
    } else {
        cpu->P &= ~FLAG_NEGATIVE;
    }
}
void BIT(MOS6502 *cpu, uint16_t address){
    uint8_t content = read_mem(address);
    if((cpu->A & content) == 0){
        cpu->P |= FLAG_ZERO;
    } else {
        cpu->P &= ~FLAG_ZERO;
    }
    if(content & 0x80){
        cpu->P |= FLAG_NEGATIVE;
    } else {
        cpu->P &= ~FLAG_NEGATIVE;
    }
    if(content & 0x40){
        cpu->P |= FLAG_OVERFLOW;
    } else {
        cpu->P &= ~FLAG_OVERFLOW;
    }
}
void CLC(MOS6502 *cpu){
    cpu->P &= ~FLAG_CARRY;
}    
void CLD(MOS6502 *cpu){
    cpu->P &= ~FLAG_DECIMAL;    
}
void CLI(MOS6502 *cpu){
    cpu->P &= ~FLAG_INTERUPT_DISABLE;
}
void CLV(MOS6502 *cpu){
    cpu->P &= ~FLAG_OVERFLOW;
}
void SEC(MOS6502 *cpu){
    cpu->P |= FLAG_CARRY;
}
void SED(MOS6502 *cpu){
    cpu->P |= FLAG_DECIMAL;
}
void SEI(MOS6502 *cpu){
    cpu->P |= FLAG_INTERUPT_DISABLE;
}


void BRK(MOS6502 *cpu, uint16_t address){
    cpu->PC += 2;
    uint16_t stack_address = 0x0100 | cpu->S;
    write_mem(stack_address, (cpu->PC >> 8) & 0xFF);
    cpu->S--;
    stack_address =  0x0100 | cpu->S;
    write_mem(stack_address, (cpu->PC & 0xFF ));
    cpu->S--;
    stack_address =  0x0100 | cpu->S;
    write_mem(stack_address, cpu->P | FLAG_BREAK | FLAG_UNUSED);
    cpu->S--;

    cpu->P |= FLAG_INTERUPT_DISABLE;
    uint8_t low_byte = read_mem(0xFFFE);
    uint8_t high_byte = read_mem(0xFFFF);
    cpu->PC = (high_byte << 8) | low_byte; 
    
}
void NOP(MOS6502 *cpu, uint16_t address){
    //No Program needed, PC is already handled
}
void RTI(MOS6502 *cpu, uint16_t address){
    cpu->S++;
    uint16_t stack_address = 0x0100 | cpu->S;
    cpu->P = (read_mem(stack_address) & ~FLAG_BREAK) | FLAG_UNUSED;
    cpu->S++;
    stack_address =  0x0100 | cpu->S;
    uint8_t low_byte = read_mem(stack_address);
    cpu->S++;
    stack_address =  0x0100 | cpu->S;
    uint8_t high_byte = read_mem(stack_address);

    cpu->PC = (high_byte << 8) | low_byte;
}
bool BCC(MOS6502 *cpu){
    if((cpu->P & 0x01) == 0){
        return true;
    }
    return false;
} 
bool BCS(MOS6502 *cpu){
    if((cpu->P & 0x01) != 0){
        return true;
    }
    return false;
}
bool BEQ(MOS6502 *cpu){
    if((cpu->P & 0x02) != 0){
        return true;
    }
    return false;
} 
bool BMI(MOS6502 *cpu){
    if((cpu->P & 0x80) != 0){
        return true;
    }
    return false;
} 
bool BNE(MOS6502 *cpu){
    if((cpu->P & 0x02) == 0){
        return true;
    }
    return false;
} 
bool BPL(MOS6502 *cpu){
    if((cpu->P & 0x80) == 0){
        return true;
    }
    return false;
} 
bool BVC(MOS6502 *cpu){
    if((cpu->P & 0x40) == 0){
        return true;
    }
    return false;
} 
bool BVS(MOS6502 *cpu){
    if((cpu->P & 0x40) != 0){
        return true;
    }
    return false;    
} 
void ADC(MOS6502 *cpu, uint16_t address){
    uint8_t operand = read_mem(address);
    uint16_t sum = cpu->A + operand + (cpu->P & FLAG_CARRY);
    if((sum & 0xFF) == 0){
        cpu->P |= FLAG_ZERO;
    } else{
        cpu->P &= ~FLAG_ZERO;
    }
    if((sum & 0x80)){
        cpu->P |= FLAG_NEGATIVE;
    } else{
        cpu->P &= ~FLAG_NEGATIVE;
    }
    if(sum > 0xFF){
        cpu->P |= FLAG_CARRY;
    } else{
        cpu->P &= ~FLAG_CARRY;
    }
    if(~(cpu->A ^ read_mem(address)) & (cpu->A ^ sum) & 0x80){
        cpu->P |= FLAG_OVERFLOW;
    } else{
        cpu->P &= ~FLAG_OVERFLOW;
    }

    cpu->A = (uint8_t)(sum & 0xFF);
} //Add with Carry
void SBC(MOS6502 *cpu, uint16_t address){
    uint8_t operand = read_mem(address);
    uint16_t sum = cpu->A + (~operand) + (cpu->P & FLAG_CARRY);
    if((sum & 0xFF) == 0){
        cpu->P |= FLAG_ZERO;
    } else{
        cpu->P &= ~FLAG_ZERO;
    }
    if((sum & 0x80)){
        cpu->P |= FLAG_NEGATIVE;
    } else{
        cpu->P &= ~FLAG_NEGATIVE;
    }
    if(sum > 0xFF){
        cpu->P |= FLAG_CARRY;
    } else{
        cpu->P &= FLAG_CARRY;
    }
    if((cpu->A ^ ~operand) & (cpu->A ^ sum) & 0x80){
        cpu->P |= FLAG_OVERFLOW;
    } else{
        cpu->P &= ~FLAG_OVERFLOW;
    }

    cpu->A = (uint8_t)(sum & 0xFF);
} //Subtract with Carry
void CMP(MOS6502 *cpu, uint16_t address){
    uint8_t operand = read_mem(address);

    if(cpu->A >= operand){
        cpu->P |= FLAG_CARRY;
    } else{
        cpu->P &= ~FLAG_CARRY;
    }
    if(cpu->A == operand){
        cpu->P |= FLAG_ZERO;
    } else{
        cpu->P &= ~FLAG_ZERO;
    }
    if((cpu->A - operand) & 0x80){
        cpu->P |= FLAG_NEGATIVE;
    } else{
        cpu->P &= ~FLAG_NEGATIVE;
    }
}
void CPX(MOS6502 *cpu, uint16_t address){
    uint8_t operand = read_mem(address);

    if(cpu->X >= operand){
        cpu->P |= FLAG_CARRY;
    } else{
        cpu->P &= ~FLAG_CARRY;
    }
    if(cpu->X == operand){
        cpu->P |= FLAG_ZERO;
    } else{
        cpu->P &= ~FLAG_ZERO;
    }
    if((cpu->X - operand) & 0x80){
        cpu->P |= FLAG_NEGATIVE;
    } else{
        cpu->P &= ~FLAG_NEGATIVE;
    }
}
void CPY(MOS6502 *cpu, uint16_t address){
    uint8_t operand = read_mem(address);

    if(cpu->Y >= operand){
        cpu->P |= FLAG_CARRY;
    } else{
        cpu->P &= ~FLAG_CARRY;
    }
    if(cpu->Y == operand){
        cpu->P |= FLAG_ZERO;
    } else{
        cpu->P &= ~FLAG_ZERO;
    }
    if((cpu->Y - operand) & 0x80){
        cpu->P |= FLAG_NEGATIVE;
    } else{
        cpu->P &= ~FLAG_NEGATIVE;
    }
}
void INC(MOS6502 *cpu, uint16_t address){
    uint8_t sum = read_mem(address) + 1;
    write_mem(address, sum);
    if((sum & 0xFF) == 0){
        cpu->P |= FLAG_ZERO;
    } else{
        cpu->P &= ~FLAG_ZERO;
    }
    if(sum & 0x80){
        cpu->P |= FLAG_NEGATIVE;
    } else{
        cpu->P &= ~FLAG_NEGATIVE;
    }
}
void INX(MOS6502 *cpu){
    cpu->X += 1;
    if((cpu->X & 0xFF) == 0){
        cpu->P |= FLAG_ZERO;
    } else{
        cpu->P &= ~FLAG_ZERO;
    }
    if(cpu->X & 0x80){
        cpu->P |= FLAG_NEGATIVE;
    } else{
        cpu->P &= ~FLAG_NEGATIVE;
    }
}
void INY(MOS6502 *cpu){
    cpu->Y += 1;
    if((cpu->Y & 0xFF) == 0){
        cpu->P |= FLAG_ZERO;
    } else{
        cpu->P &= ~FLAG_ZERO;
    }
    if(cpu->Y & 0x80){
        cpu->P |= FLAG_NEGATIVE;
    } else{
        cpu->P &= ~FLAG_NEGATIVE;
    }
}
void DEC(MOS6502 *cpu, uint16_t address){
    uint8_t sum = read_mem(address) - 1;
    write_mem(address, sum);
    if((sum & 0xFF) == 0){
        cpu->P |= FLAG_ZERO;
    } else{
        cpu->P &= ~FLAG_ZERO;
    }
    if(sum & 0x80){
        cpu->P |= FLAG_NEGATIVE;
    } else{
        cpu->P &= ~FLAG_NEGATIVE;
    }
}
void DEX(MOS6502 *cpu){
    cpu->X -= 1;
    if((cpu->X & 0xFF) == 0){
        cpu->P |= FLAG_ZERO;
    } else{
        cpu->P &= ~FLAG_ZERO;
    }
    if(cpu->X & 0x80){
        cpu->P |= FLAG_NEGATIVE;
    } else{
        cpu->P &= ~FLAG_NEGATIVE;
    }
}
void DEY(MOS6502 *cpu){
    cpu->Y -= 1;
    if((cpu->Y & 0xFF) == 0){
        cpu->P |= FLAG_ZERO;
    } else{
        cpu->P &= ~FLAG_ZERO;
    }
    if(cpu->Y & 0x80){
        cpu->P |= FLAG_NEGATIVE;
    } else{
        cpu->P &= ~FLAG_NEGATIVE;
    }
}

