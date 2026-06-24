#include "include/cpu.h"
#include "src/memory.c"
void CLOCK(MOS6502 *cpu){
    if(cpu->cycles_left == 0){
        cpu->IR = read_mem(cpu->PC++);
        if(LUT[cpu->IR] != NULL){
            cpu->cycles_left = LUT[cpu->IR](cpu);
        }
    }

    if(cpu->cycles_left > 0){
        cpu->cycles_left--;
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
    uint8_t high_byte;
    if((base_address & 0x00FF) == 0x00FF){
        high_byte = read_mem(base_address & 0xFF00);
    } else{
        high_byte = read_mem(base_address + 1);
    }
    uint16_t effective_address = (high_byte << 8) | low_byte;

    return effective_address;
}
uint16_t Relative_Mode(MOS6502 *cpu){
    int8_t offset = (int8_t)read_mem(cpu->PC++);
    uint16_t PC_target = cpu->PC + offset;

    return PC_target;
}

uint16_t Indexed_Indirect_Mode(MOS6502 *cpu){
    uint8_t base_address = read_mem(cpu->PC++);
    uint8_t Pointer = (base_address + cpu->X) & 0xFF;
    uint8_t low_byte = read_mem(Pointer);
    uint8_t high_byte = read_mem((uint8_t)(Pointer+1)) & 0xFF;
    uint16_t effective_address = (high_byte << 8) | low_byte;

    return effective_address;
}
uint16_t Indirect_Indexed_Mode(MOS6502 * cpu){
    uint8_t Pointer = read_mem(cpu->PC++);
    uint8_t low_byte = read_mem(Pointer);
    uint8_t high_byte = read_mem((uint8_t)(Pointer+1)) & 0xFF;
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
void STA(MOS6502 *cpu, uint16_t address){
    write_mem(address, cpu->A);
}
void STX(MOS6502 *cpu, uint16_t address){
    write_mem(address, cpu->X);
}
void STY(MOS6502 *cpu, uint16_t address){
    write_mem(address, cpu->Y);
}
void TAX(MOS6502 *cpu, uint16_t address){
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
void TAY(MOS6502 *cpu, uint16_t address){
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
void TXA(MOS6502 *cpu, uint16_t address){
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
void TYA(MOS6502 *cpu, uint16_t address){
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
void TSX(MOS6502 *cpu, uint16_t address){
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
void TXS(MOS6502 *cpu, uint16_t address){
    cpu->S = cpu->X;
}

void PHA(MOS6502 *cpu, uint16_t address){
    uint16_t stack_address = 0x0100 | cpu->S;
    write_mem(stack_address, cpu->A);
    cpu->S--;
} 
void PHP(MOS6502 *cpu, uint16_t address){
    uint16_t stack_address = 0x0100 | cpu->S;
    write_mem(stack_address, cpu->P | FLAG_BREAK | FLAG_UNUSED);
    cpu->S--;
}
void PLA(MOS6502 *cpu, uint16_t address){
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
void PLP(MOS6502 *cpu, uint16_t address){
    cpu->S++;
    uint16_t stack_address = 0x0100 | cpu->S;
    cpu->P = (read_mem(stack_address) & 0xEF) | FLAG_UNUSED;
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
void CLC(MOS6502 *cpu, uint16_t address){
    cpu->P &= ~FLAG_CARRY;
}    
void CLD(MOS6502 *cpu, uint16_t address){
    cpu->P &= ~FLAG_DECIMAL;    
}
void CLI(MOS6502 *cpu, uint16_t address){
    cpu->P &= ~FLAG_INTERUPT_DISABLE;
}
void CLV(MOS6502 *cpu, uint16_t address){
    cpu->P &= ~FLAG_OVERFLOW;
}
void SEC(MOS6502 *cpu, uint16_t address){
    cpu->P |= FLAG_CARRY;
}
void SED(MOS6502 *cpu, uint16_t address){
    cpu->P |= FLAG_DECIMAL;
}
void SEI(MOS6502 *cpu, uint16_t address){
    cpu->P |= FLAG_INTERUPT_DISABLE;
}


void BRK(MOS6502 *cpu, uint16_t address){
    cpu->PC++;
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
    cpu->P = (read_mem(stack_address) & 0xEF) | FLAG_UNUSED;
    cpu->S++;
    stack_address =  0x0100 | cpu->S;
    uint8_t low_byte = read_mem(stack_address);
    cpu->S++;
    stack_address =  0x0100 | cpu->S;
    uint8_t high_byte = read_mem(stack_address);

    cpu->PC = (high_byte << 8) | low_byte;
}
void BCC(MOS6502 *cpu){
    if((cpu->P & 0x01) == 0){
        cpu->branch_result = true;
    } else{
        cpu->branch_result = false;
    }
}
void BCS(MOS6502 *cpu){
    if((cpu->P & 0x01) != 0){
        cpu->branch_result = true;
    } else{
        cpu->branch_result = false;
    };
}
void BEQ(MOS6502 *cpu){
    if((cpu->P & 0x02) != 0){
        cpu->branch_result = true;
    } else{
        cpu->branch_result = false;
    }
}
void BMI(MOS6502 *cpu){
    if((cpu->P & 0x80) != 0){
        cpu->branch_result = true;
    } else{
        cpu->branch_result = false;
    } 
}
void BNE(MOS6502 *cpu){
    if((cpu->P & 0x02) == 0){
        cpu->branch_result = true;
    } else{
        cpu->branch_result = false;
    }
}
void BPL(MOS6502 *cpu){
    if((cpu->P & 0x80) == 0){
        cpu->branch_result = true;
    } else{
        cpu->branch_result = false;
    }
}
void BVC(MOS6502 *cpu){
    if((cpu->P & 0x40) == 0){
        cpu->branch_result = true;
    } else{
        cpu->branch_result = false;
    }
}
void BVS(MOS6502 *cpu){
    if((cpu->P & 0x40) != 0){
        cpu->branch_result = true;
    } else{
        cpu->branch_result = false;
    }
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
} 
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
        cpu->P &= ~FLAG_CARRY;
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
void INX(MOS6502 *cpu, uint16_t address){
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
void INY(MOS6502 *cpu, uint16_t address){
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
void DEX(MOS6502 *cpu, uint16_t address){
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
void DEY(MOS6502 *cpu, uint16_t address){
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
void ASL(MOS6502 *cpu, uint16_t address){
    if(cpu->accumulator_mode){
        if(cpu->A & 0x80){
            cpu->P |= FLAG_CARRY;
        } else {
            cpu->P &= ~FLAG_CARRY;
        }
        cpu->A = (cpu->A << 1) & 0xFF;
        if((cpu->A & 0xFF) == 0){
            cpu->P |= FLAG_ZERO;
        } else {
            cpu->P &= ~FLAG_ZERO;
        }
        if(cpu->A & 0x80){
            cpu->P |= FLAG_NEGATIVE;
        } else{
            cpu->P &= ~FLAG_NEGATIVE;
        }
    } else {
        uint8_t value = read_mem(address);
        if(value & 0x80){
            cpu->P |= FLAG_CARRY;
        } else {
            cpu->P &= ~FLAG_CARRY;
        }
        value = (value << 1) & 0xFF;
        write_mem(address, value);
        if((value & 0xFF) == 0){
            cpu->P |= FLAG_ZERO;
        } else {
            cpu->P &= ~FLAG_ZERO;
        }
        if(value & 0x80){
            cpu->P |= FLAG_NEGATIVE;
        } else{
            cpu->P &= ~FLAG_NEGATIVE;
        }
    }
}
void LSR(MOS6502 *cpu, uint16_t address){
    if(cpu->accumulator_mode){
        if(cpu->A & 0x01){
            cpu->P |= FLAG_CARRY;
        } else {
            cpu->P &= ~FLAG_CARRY;
        }
        cpu->A = (cpu->A >> 1) & 0xFF;
        if((cpu->A & 0xFF) == 0){
            cpu->P |= FLAG_ZERO;
        } else {
            cpu->P &= ~FLAG_ZERO;
        }
        cpu->P &= ~FLAG_NEGATIVE;
    } else {
        uint8_t value = read_mem(address);
        if(value & 0x01){
            cpu->P |= FLAG_CARRY;
        } else {
            cpu->P &= ~FLAG_CARRY;
        }
        value = (value >> 1) & 0xFF;
        write_mem(address, value);
        if((value & 0xFF) == 0){
            cpu->P |= FLAG_ZERO;
        } else {
            cpu->P &= ~FLAG_ZERO;
        }
        cpu->P &= ~FLAG_NEGATIVE;
    }
}
void ROL(MOS6502 *cpu, uint16_t address){
    uint8_t old_carry = (cpu->P & FLAG_CARRY) ? 1:0;
    if(cpu->accumulator_mode){
        if(cpu->A & 0x80){
            cpu->P |= FLAG_CARRY;
        } else {
            cpu->P &= ~FLAG_CARRY;
        }
        cpu->A = ((cpu->A << 1) | old_carry) & 0xFF;
        if((cpu->A & 0xFF) == 0){
            cpu->P |= FLAG_ZERO;
        } else {
            cpu->P &= ~FLAG_ZERO;
        }
        if(cpu->A & 0x80){
            cpu->P |= FLAG_NEGATIVE;
        } else{
            cpu->P &= ~FLAG_NEGATIVE;
        }
    } else {
        uint8_t value = read_mem(address);
        if(value & 0x80){
            cpu->P |= FLAG_CARRY;
        } else {
            cpu->P &= ~FLAG_CARRY;
        }
        value = ((value << 1) | old_carry) & 0xFF;
        write_mem(address, value);
        if((value & 0xFF) == 0){
            cpu->P |= FLAG_ZERO;
        } else {
            cpu->P &= ~FLAG_ZERO;
        }
        if(value & 0x80){
            cpu->P |= FLAG_NEGATIVE;
        } else{
            cpu->P &= ~FLAG_NEGATIVE;
        }
    }
}
void ROR(MOS6502 *cpu, uint16_t address){
    uint8_t old_carry = ((cpu->P & FLAG_CARRY) ? 1:0) << 7;
    if(cpu->accumulator_mode){
        if(cpu->A & 0x01){
            cpu->P |= FLAG_CARRY;
        } else {
            cpu->P &= ~FLAG_CARRY;
        }
        cpu->A = ((cpu->A >> 1) | old_carry) & 0xFF;
        if((cpu->A & 0xFF) == 0){
            cpu->P |= FLAG_ZERO;
        } else {
            cpu->P &= ~FLAG_ZERO;
        }
        if(cpu->A & 0x80){
            cpu->P |= FLAG_NEGATIVE;
        } else{
            cpu->P &= ~FLAG_NEGATIVE;
        }
    } else {
        uint8_t value = read_mem(address);
        if(value & 0x01){
            cpu->P |= FLAG_CARRY;
        } else {
            cpu->P &= ~FLAG_CARRY;
        }
        value = ((value >> 1) | old_carry) & 0xFF;
        write_mem(address, value);
        if((value & 0xFF) == 0){
            cpu->P |= FLAG_ZERO;
        } else {
            cpu->P &= ~FLAG_ZERO;
        }
        if(value & 0x80){
            cpu->P |= FLAG_NEGATIVE;
        } else{
            cpu->P &= ~FLAG_NEGATIVE;
        }
    }
}

void JMP(MOS6502 *cpu, uint16_t address){
    cpu->PC = address;
}
void JSR(MOS6502 *cpu, uint16_t address){
    uint16_t return_address = cpu->PC - 1;
    uint16_t stack_address = 0x0100 | cpu->S;
    uint8_t high_byte = (return_address >> 8) & 0xFF;
    write_mem(stack_address, high_byte);
    cpu->S--;
    stack_address = 0x0100 | cpu->S;
    uint8_t low_byte = return_address & 0xFF;
    write_mem(stack_address, low_byte);
    cpu->S--;
    cpu->PC = address;
}
void RTS(MOS6502 *cpu, uint16_t address){
    cpu->S++;
    uint16_t stack_address = 0x0100 | cpu->S;
    uint8_t low_byte = read_mem(stack_address);
    cpu->S++;
    stack_address = 0x0100 | cpu->S;
    uint8_t high_byte = read_mem(stack_address);
    uint16_t return_address = (high_byte << 8) | low_byte;
    cpu->PC = return_address+1;
}

/*=== uint8_t Opcode Implementations ===*/

/* Implied Mode */
uint8_t NOP_0xEA(MOS6502 *cpu){ NOP(cpu,0); return 2; }
uint8_t BRK_0x00(MOS6502 *cpu){ BRK(cpu,0); return 7; }
uint8_t RTI_0x40(MOS6502 *cpu){ RTI(cpu,0); return 6; }
uint8_t RTS_0x60(MOS6502 *cpu){ RTS(cpu,0); return 6; }
uint8_t PLP_0x28(MOS6502 *cpu){ PLP(cpu,0); return 4; }
uint8_t PLA_0x68(MOS6502 *cpu){ PLA(cpu,0); return 4; }
uint8_t PHP_0x08(MOS6502 *cpu){ PHP(cpu,0); return 3; }
uint8_t PHA_0x48(MOS6502 *cpu){ PHA(cpu,0); return 3; }
uint8_t DEY_0x88(MOS6502 *cpu){ DEY(cpu,0); return 2; }
uint8_t DEX_0xCA(MOS6502 *cpu){ DEX(cpu,0); return 2; }
uint8_t INY_0xC8(MOS6502 *cpu){ INY(cpu,0); return 2; }
uint8_t INX_0xE8(MOS6502 *cpu){ INX(cpu,0); return 2; }
uint8_t TXS_0x9A(MOS6502 *cpu){ TXS(cpu,0); return 2; }
uint8_t TSX_0xBA(MOS6502 *cpu){ TSX(cpu,0); return 2; }
uint8_t TYA_0x98(MOS6502 *cpu){ TYA(cpu,0); return 2; }
uint8_t TAY_0xA8(MOS6502 *cpu){ TAY(cpu,0); return 2; }
uint8_t TXA_0x8A(MOS6502 *cpu){ TXA(cpu,0); return 2; }
uint8_t TAX_0xAA(MOS6502 *cpu){ TAX(cpu,0); return 2; }
uint8_t CLV_0xB8(MOS6502 *cpu){ CLV(cpu,0); return 2; }
uint8_t SEI_0x78(MOS6502 *cpu){ SEI(cpu,0); return 2; }
uint8_t CLI_0x58(MOS6502 *cpu){ CLI(cpu,0); return 2; }
uint8_t SED_0xF8(MOS6502 *cpu){ SED(cpu,0); return 2; }
uint8_t CLD_0xD8(MOS6502 *cpu){ CLD(cpu,0); return 2; }
uint8_t SEC_0x38(MOS6502 *cpu){ SEC(cpu,0); return 2; }
uint8_t CLC_0x18(MOS6502 *cpu){ CLC(cpu,0); return 2; }

/* Accumulator Mode */
uint8_t ASL_0x0A(MOS6502 *cpu){ cpu->accumulator_mode=true; ASL(cpu,0); cpu->accumulator_mode=false; return 2; }
uint8_t ROL_0x2A(MOS6502 *cpu){ cpu->accumulator_mode=true; ROL(cpu,0); cpu->accumulator_mode=false; return 2; }
uint8_t LSR_0x4A(MOS6502 *cpu){ cpu->accumulator_mode=true; LSR(cpu,0); cpu->accumulator_mode=false; return 2; }
uint8_t ROR_0x6A(MOS6502 *cpu){ cpu->accumulator_mode=true; ROR(cpu,0); cpu->accumulator_mode=false; return 2; }

/* Branch Instructions
   Relative_Mode reads the signed offset and returns the target PC address.
   The void branch helpers set cpu->branch_result based on the flag condition.
   NOTE: the void branch helpers are missing an 'else' clause — branch_result
   is always overwritten to false after the if block.  Fix those helpers by
   adding 'else' to each one so branching works correctly. */
uint8_t BPL_0x10(MOS6502 *cpu){
    uint16_t target = Relative_Mode(cpu);
    BPL(cpu);
    if(cpu->branch_result){
        uint8_t extra = ((cpu->PC & 0xFF00) != (target & 0xFF00)) ? 2 : 1;
        cpu->PC = target;
        return 2 + extra;
    }
    return 2;
}
uint8_t BMI_0x30(MOS6502 *cpu){
    uint16_t target = Relative_Mode(cpu);
    BMI(cpu);
    if(cpu->branch_result){
        uint8_t extra = ((cpu->PC & 0xFF00) != (target & 0xFF00)) ? 2 : 1;
        cpu->PC = target;
        return 2 + extra;
    }
    return 2;
}
uint8_t BVC_0x50(MOS6502 *cpu){
    uint16_t target = Relative_Mode(cpu);
    BVC(cpu);
    if(cpu->branch_result){
        uint8_t extra = ((cpu->PC & 0xFF00) != (target & 0xFF00)) ? 2 : 1;
        cpu->PC = target;
        return 2 + extra;
    }
    return 2;
}
uint8_t BVS_0x70(MOS6502 *cpu){
    uint16_t target = Relative_Mode(cpu);
    BVS(cpu);
    if(cpu->branch_result){
        uint8_t extra = ((cpu->PC & 0xFF00) != (target & 0xFF00)) ? 2 : 1;
        cpu->PC = target;
        return 2 + extra;
    }
    return 2;
}
uint8_t BCC_0x90(MOS6502 *cpu){
    uint16_t target = Relative_Mode(cpu);
    BCC(cpu);
    if(cpu->branch_result){
        uint8_t extra = ((cpu->PC & 0xFF00) != (target & 0xFF00)) ? 2 : 1;
        cpu->PC = target;
        return 2 + extra;
    }
    return 2;
}
uint8_t BCS_0xB0(MOS6502 *cpu){
    uint16_t target = Relative_Mode(cpu);
    BCS(cpu);
    if(cpu->branch_result){
        uint8_t extra = ((cpu->PC & 0xFF00) != (target & 0xFF00)) ? 2 : 1;
        cpu->PC = target;
        return 2 + extra;
    }
    return 2;
}
uint8_t BNE_0xD0(MOS6502 *cpu){
    uint16_t target = Relative_Mode(cpu);
    BNE(cpu);
    if(cpu->branch_result){
        uint8_t extra = ((cpu->PC & 0xFF00) != (target & 0xFF00)) ? 2 : 1;
        cpu->PC = target;
        return 2 + extra;
    }
    return 2;
}
uint8_t BEQ_0xF0(MOS6502 *cpu){
    uint16_t target = Relative_Mode(cpu);
    BEQ(cpu);
    if(cpu->branch_result){
        uint8_t extra = ((cpu->PC & 0xFF00) != (target & 0xFF00)) ? 2 : 1;
        cpu->PC = target;
        return 2 + extra;
    }
    return 2;
}

/* Jump / Subroutine */
uint8_t JSR_0x20(MOS6502 *cpu){ uint16_t a=Absolute_Mode(cpu); JSR(cpu,a); return 6; }
uint8_t JMP_0x4C(MOS6502 *cpu){ uint16_t a=Absolute_Mode(cpu); JMP(cpu,a); return 3; }
uint8_t JMP_0x6C(MOS6502 *cpu){ uint16_t a=Absolute_Indirect_Mode(cpu); JMP(cpu,a); return 5; }

/* LDA */
uint8_t LDA_0xA9(MOS6502 *cpu){ LDA(cpu,Immediate_Mode(cpu)); return 2; }
uint8_t LDA_0xA5(MOS6502 *cpu){ LDA(cpu,Zero_Page_Mode(cpu)); return 3; }
uint8_t LDA_0xB5(MOS6502 *cpu){ LDA(cpu,Zero_PageX_Mode(cpu)); return 4; }
uint8_t LDA_0xAD(MOS6502 *cpu){ LDA(cpu,Absolute_Mode(cpu)); return 4; }
uint8_t LDA_0xBD(MOS6502 *cpu){ LDA(cpu,AbsoluteX_Mode(cpu)); return 4; }
uint8_t LDA_0xB9(MOS6502 *cpu){ LDA(cpu,AbsoluteY_Mode(cpu)); return 4; }
uint8_t LDA_0xA1(MOS6502 *cpu){ LDA(cpu,Indexed_Indirect_Mode(cpu)); return 6; }
uint8_t LDA_0xB1(MOS6502 *cpu){ LDA(cpu,Indirect_Indexed_Mode(cpu)); return 5; }

/* LDX */
uint8_t LDX_0xA2(MOS6502 *cpu){ LDX(cpu,Immediate_Mode(cpu)); return 2; }
uint8_t LDX_0xA6(MOS6502 *cpu){ LDX(cpu,Zero_Page_Mode(cpu)); return 3; }
uint8_t LDX_0xB6(MOS6502 *cpu){ LDX(cpu,Zero_PageY_Mode(cpu)); return 4; }
uint8_t LDX_0xAE(MOS6502 *cpu){ LDX(cpu,Absolute_Mode(cpu)); return 4; }
uint8_t LDX_0xBE(MOS6502 *cpu){ LDX(cpu,AbsoluteY_Mode(cpu)); return 4; }

/* LDY */
uint8_t LDY_0xA0(MOS6502 *cpu){ LDY(cpu,Immediate_Mode(cpu)); return 2; }
uint8_t LDY_0xA4(MOS6502 *cpu){ LDY(cpu,Zero_Page_Mode(cpu)); return 3; }
uint8_t LDY_0xB4(MOS6502 *cpu){ LDY(cpu,Zero_PageX_Mode(cpu)); return 4; }
uint8_t LDY_0xAC(MOS6502 *cpu){ LDY(cpu,Absolute_Mode(cpu)); return 4; }
uint8_t LDY_0xBC(MOS6502 *cpu){ LDY(cpu,AbsoluteX_Mode(cpu)); return 4; }

/* STA - store instructions have no page-crossing cycle penalty;
   Absolute,X and Absolute,Y use Absolute_Mode + manual offset to avoid
   the extra cycle that AbsoluteX/Y_Mode would add on page crossings */
uint8_t STA_0x85(MOS6502 *cpu){ STA(cpu,Zero_Page_Mode(cpu)); return 3; }
uint8_t STA_0x95(MOS6502 *cpu){ STA(cpu,Zero_PageX_Mode(cpu)); return 4; }
uint8_t STA_0x8D(MOS6502 *cpu){ STA(cpu,Absolute_Mode(cpu)); return 4; }
uint8_t STA_0x9D(MOS6502 *cpu){ uint16_t a=Absolute_Mode(cpu); STA(cpu,a+cpu->X); return 5; }
uint8_t STA_0x99(MOS6502 *cpu){ uint16_t a=Absolute_Mode(cpu); STA(cpu,a+cpu->Y); return 5; }
uint8_t STA_0x81(MOS6502 *cpu){ STA(cpu,Indexed_Indirect_Mode(cpu)); return 6; }
uint8_t STA_0x91(MOS6502 *cpu){ uint8_t saved=cpu->cycles_left; uint16_t a=Indirect_Indexed_Mode(cpu); cpu->cycles_left=saved; STA(cpu,a); return 6; }

/* STX */
uint8_t STX_0x86(MOS6502 *cpu){ STX(cpu,Zero_Page_Mode(cpu)); return 3; }
uint8_t STX_0x96(MOS6502 *cpu){ STX(cpu,Zero_PageY_Mode(cpu)); return 4; }
uint8_t STX_0x8E(MOS6502 *cpu){ STX(cpu,Absolute_Mode(cpu)); return 4; }

/* STY */
uint8_t STY_0x84(MOS6502 *cpu){ STY(cpu,Zero_Page_Mode(cpu)); return 3; }
uint8_t STY_0x94(MOS6502 *cpu){ STY(cpu,Zero_PageX_Mode(cpu)); return 4; }
uint8_t STY_0x8C(MOS6502 *cpu){ STY(cpu,Absolute_Mode(cpu)); return 4; }

/* ADC */
uint8_t ADC_0x69(MOS6502 *cpu){ ADC(cpu,Immediate_Mode(cpu)); return 2; }
uint8_t ADC_0x65(MOS6502 *cpu){ ADC(cpu,Zero_Page_Mode(cpu)); return 3; }
uint8_t ADC_0x75(MOS6502 *cpu){ ADC(cpu,Zero_PageX_Mode(cpu)); return 4; }
uint8_t ADC_0x6D(MOS6502 *cpu){ ADC(cpu,Absolute_Mode(cpu)); return 4; }
uint8_t ADC_0x7D(MOS6502 *cpu){ ADC(cpu,AbsoluteX_Mode(cpu)); return 4; }
uint8_t ADC_0x79(MOS6502 *cpu){ ADC(cpu,AbsoluteY_Mode(cpu)); return 4; }
uint8_t ADC_0x61(MOS6502 *cpu){ ADC(cpu,Indexed_Indirect_Mode(cpu)); return 6; }
uint8_t ADC_0x71(MOS6502 *cpu){ ADC(cpu,Indirect_Indexed_Mode(cpu)); return 5; }

/* SBC */
uint8_t SBC_0xE9(MOS6502 *cpu){ SBC(cpu,Immediate_Mode(cpu)); return 2; }
uint8_t SBC_0xE5(MOS6502 *cpu){ SBC(cpu,Zero_Page_Mode(cpu)); return 3; }
uint8_t SBC_0xF5(MOS6502 *cpu){ SBC(cpu,Zero_PageX_Mode(cpu)); return 4; }
uint8_t SBC_0xED(MOS6502 *cpu){ SBC(cpu,Absolute_Mode(cpu)); return 4; }
uint8_t SBC_0xFD(MOS6502 *cpu){ SBC(cpu,AbsoluteX_Mode(cpu)); return 4; }
uint8_t SBC_0xF9(MOS6502 *cpu){ SBC(cpu,AbsoluteY_Mode(cpu)); return 4; }
uint8_t SBC_0xE1(MOS6502 *cpu){ SBC(cpu,Indexed_Indirect_Mode(cpu)); return 6; }
uint8_t SBC_0xF1(MOS6502 *cpu){ SBC(cpu,Indirect_Indexed_Mode(cpu)); return 5; }

/* AND */
uint8_t AND_0x29(MOS6502 *cpu){ AND(cpu,Immediate_Mode(cpu)); return 2; }
uint8_t AND_0x25(MOS6502 *cpu){ AND(cpu,Zero_Page_Mode(cpu)); return 3; }
uint8_t AND_0x35(MOS6502 *cpu){ AND(cpu,Zero_PageX_Mode(cpu)); return 4; }
uint8_t AND_0x2D(MOS6502 *cpu){ AND(cpu,Absolute_Mode(cpu)); return 4; }
uint8_t AND_0x3D(MOS6502 *cpu){ AND(cpu,AbsoluteX_Mode(cpu)); return 4; }
uint8_t AND_0x39(MOS6502 *cpu){ AND(cpu,AbsoluteY_Mode(cpu)); return 4; }
uint8_t AND_0x21(MOS6502 *cpu){ AND(cpu,Indexed_Indirect_Mode(cpu)); return 6; }
uint8_t AND_0x31(MOS6502 *cpu){ AND(cpu,Indirect_Indexed_Mode(cpu)); return 5; }

/* ORA */
uint8_t ORA_0x09(MOS6502 *cpu){ ORA(cpu,Immediate_Mode(cpu)); return 2; }
uint8_t ORA_0x05(MOS6502 *cpu){ ORA(cpu,Zero_Page_Mode(cpu)); return 3; }
uint8_t ORA_0x15(MOS6502 *cpu){ ORA(cpu,Zero_PageX_Mode(cpu)); return 4; }
uint8_t ORA_0x0D(MOS6502 *cpu){ ORA(cpu,Absolute_Mode(cpu)); return 4; }
uint8_t ORA_0x1D(MOS6502 *cpu){ ORA(cpu,AbsoluteX_Mode(cpu)); return 4; }
uint8_t ORA_0x19(MOS6502 *cpu){ ORA(cpu,AbsoluteY_Mode(cpu)); return 4; }
uint8_t ORA_0x01(MOS6502 *cpu){ ORA(cpu,Indexed_Indirect_Mode(cpu)); return 6; }
uint8_t ORA_0x11(MOS6502 *cpu){ ORA(cpu,Indirect_Indexed_Mode(cpu)); return 5; }

/* EOR */
uint8_t EOR_0x49(MOS6502 *cpu){ EOR(cpu,Immediate_Mode(cpu)); return 2; }
uint8_t EOR_0x45(MOS6502 *cpu){ EOR(cpu,Zero_Page_Mode(cpu)); return 3; }
uint8_t EOR_0x55(MOS6502 *cpu){ EOR(cpu,Zero_PageX_Mode(cpu)); return 4; }
uint8_t EOR_0x4D(MOS6502 *cpu){ EOR(cpu,Absolute_Mode(cpu)); return 4; }
uint8_t EOR_0x5D(MOS6502 *cpu){ EOR(cpu,AbsoluteX_Mode(cpu)); return 4; }
uint8_t EOR_0x59(MOS6502 *cpu){ EOR(cpu,AbsoluteY_Mode(cpu)); return 4; }
uint8_t EOR_0x41(MOS6502 *cpu){ EOR(cpu,Indexed_Indirect_Mode(cpu)); return 6; }
uint8_t EOR_0x51(MOS6502 *cpu){ EOR(cpu,Indirect_Indexed_Mode(cpu)); return 5; }

/* BIT */
uint8_t BIT_0x24(MOS6502 *cpu){ BIT(cpu,Zero_Page_Mode(cpu)); return 3; }
uint8_t BIT_0x2C(MOS6502 *cpu){ BIT(cpu,Absolute_Mode(cpu)); return 4; }

/* CMP */
uint8_t CMP_0xC9(MOS6502 *cpu){ CMP(cpu,Immediate_Mode(cpu)); return 2; }
uint8_t CMP_0xC5(MOS6502 *cpu){ CMP(cpu,Zero_Page_Mode(cpu)); return 3; }
uint8_t CMP_0xD5(MOS6502 *cpu){ CMP(cpu,Zero_PageX_Mode(cpu)); return 4; }
uint8_t CMP_0xCD(MOS6502 *cpu){ CMP(cpu,Absolute_Mode(cpu)); return 4; }
uint8_t CMP_0xDD(MOS6502 *cpu){ CMP(cpu,AbsoluteX_Mode(cpu)); return 4; }
uint8_t CMP_0xD9(MOS6502 *cpu){ CMP(cpu,AbsoluteY_Mode(cpu)); return 4; }
uint8_t CMP_0xC1(MOS6502 *cpu){ CMP(cpu,Indexed_Indirect_Mode(cpu)); return 6; }
uint8_t CMP_0xD1(MOS6502 *cpu){ CMP(cpu,Indirect_Indexed_Mode(cpu)); return 5; }

/* CPX */
uint8_t CPX_0xE0(MOS6502 *cpu){ CPX(cpu,Immediate_Mode(cpu)); return 2; }
uint8_t CPX_0xE4(MOS6502 *cpu){ CPX(cpu,Zero_Page_Mode(cpu)); return 3; }
uint8_t CPX_0xEC(MOS6502 *cpu){ CPX(cpu,Absolute_Mode(cpu)); return 4; }

/* CPY */
uint8_t CPY_0xC0(MOS6502 *cpu){ CPY(cpu,Immediate_Mode(cpu)); return 2; }
uint8_t CPY_0xC4(MOS6502 *cpu){ CPY(cpu,Zero_Page_Mode(cpu)); return 3; }
uint8_t CPY_0xCC(MOS6502 *cpu){ CPY(cpu,Absolute_Mode(cpu)); return 4; }

/* ASL memory - read-modify-write ops have no page-crossing penalty on Absolute,X;
   use Absolute_Mode + manual X offset for the 7-cycle variant */
uint8_t ASL_0x06(MOS6502 *cpu){ cpu->accumulator_mode=false; ASL(cpu,Zero_Page_Mode(cpu)); return 5; }
uint8_t ASL_0x16(MOS6502 *cpu){ cpu->accumulator_mode=false; ASL(cpu,Zero_PageX_Mode(cpu)); return 6; }
uint8_t ASL_0x0E(MOS6502 *cpu){ cpu->accumulator_mode=false; ASL(cpu,Absolute_Mode(cpu)); return 6; }
uint8_t ASL_0x1E(MOS6502 *cpu){ cpu->accumulator_mode=false; uint16_t a=Absolute_Mode(cpu); ASL(cpu,a+cpu->X); return 7; }

/* LSR memory */
uint8_t LSR_0x46(MOS6502 *cpu){ cpu->accumulator_mode=false; LSR(cpu,Zero_Page_Mode(cpu)); return 5; }
uint8_t LSR_0x56(MOS6502 *cpu){ cpu->accumulator_mode=false; LSR(cpu,Zero_PageX_Mode(cpu)); return 6; }
uint8_t LSR_0x4E(MOS6502 *cpu){ cpu->accumulator_mode=false; LSR(cpu,Absolute_Mode(cpu)); return 6; }
uint8_t LSR_0x5E(MOS6502 *cpu){ cpu->accumulator_mode=false; uint16_t a=Absolute_Mode(cpu); LSR(cpu,a+cpu->X); return 7; }

/* ROL memory */
uint8_t ROL_0x26(MOS6502 *cpu){ cpu->accumulator_mode=false; ROL(cpu,Zero_Page_Mode(cpu)); return 5; }
uint8_t ROL_0x36(MOS6502 *cpu){ cpu->accumulator_mode=false; ROL(cpu,Zero_PageX_Mode(cpu)); return 6; }
uint8_t ROL_0x2E(MOS6502 *cpu){ cpu->accumulator_mode=false; ROL(cpu,Absolute_Mode(cpu)); return 6; }
uint8_t ROL_0x3E(MOS6502 *cpu){ cpu->accumulator_mode=false; uint16_t a=Absolute_Mode(cpu); ROL(cpu,a+cpu->X); return 7; }

/* ROR memory */
uint8_t ROR_0x66(MOS6502 *cpu){ cpu->accumulator_mode=false; ROR(cpu,Zero_Page_Mode(cpu)); return 5; }
uint8_t ROR_0x76(MOS6502 *cpu){ cpu->accumulator_mode=false; ROR(cpu,Zero_PageX_Mode(cpu)); return 6; }
uint8_t ROR_0x6E(MOS6502 *cpu){ cpu->accumulator_mode=false; ROR(cpu,Absolute_Mode(cpu)); return 6; }
uint8_t ROR_0x7E(MOS6502 *cpu){ cpu->accumulator_mode=false; uint16_t a=Absolute_Mode(cpu); ROR(cpu,a+cpu->X); return 7; }

/* INC */
uint8_t INC_0xE6(MOS6502 *cpu){ INC(cpu,Zero_Page_Mode(cpu)); return 5; }
uint8_t INC_0xF6(MOS6502 *cpu){ INC(cpu,Zero_PageX_Mode(cpu)); return 6; }
uint8_t INC_0xEE(MOS6502 *cpu){ INC(cpu,Absolute_Mode(cpu)); return 6; }
uint8_t INC_0xFE(MOS6502 *cpu){ uint16_t a=Absolute_Mode(cpu); INC(cpu,a+cpu->X); return 7; }

/* DEC */
uint8_t DEC_0xC6(MOS6502 *cpu){ DEC(cpu,Zero_Page_Mode(cpu)); return 5; }
uint8_t DEC_0xD6(MOS6502 *cpu){ DEC(cpu,Zero_PageX_Mode(cpu)); return 6; }
uint8_t DEC_0xCE(MOS6502 *cpu){ DEC(cpu,Absolute_Mode(cpu)); return 6; }
uint8_t DEC_0xDE(MOS6502 *cpu){ uint16_t a=Absolute_Mode(cpu); DEC(cpu,a+cpu->X); return 7; }

/*=== Opcode LUT Definition ===*/
const uint8_t (*LUT[256])(MOS6502*) = {
    [0x00]=BRK_0x00, [0x01]=ORA_0x01, [0x05]=ORA_0x05, [0x06]=ASL_0x06, [0x08]=PHP_0x08, [0x09]=ORA_0x09, [0x0A]=ASL_0x0A, [0x0D]=ORA_0x0D, [0x0E]=ASL_0x0E,
    [0x10]=BPL_0x10, [0x11]=ORA_0x11, [0x15]=ORA_0x15, [0x16]=ASL_0x16, [0x18]=CLC_0x18, [0x19]=ORA_0x19, [0x1D]=ORA_0x1D, [0x1E]=ASL_0x1E,
    [0x20]=JSR_0x20, [0x21]=AND_0x21, [0x24]=BIT_0x24, [0x25]=AND_0x25, [0x26]=ROL_0x26, [0x28]=PLP_0x28, [0x29]=AND_0x29, [0x2A]=ROL_0x2A, [0x2C]=BIT_0x2C, [0x2D]=AND_0x2D, [0x2E]=ROL_0x2E,
    [0x30]=BMI_0x30, [0x31]=AND_0x31, [0x35]=AND_0x35, [0x36]=ROL_0x36, [0x38]=SEC_0x38, [0x39]=AND_0x39, [0x3D]=AND_0x3D, [0x3E]=ROL_0x3E,
    [0x40]=RTI_0x40, [0x41]=EOR_0x41, [0x45]=EOR_0x45, [0x46]=LSR_0x46, [0x48]=PHA_0x48, [0x49]=EOR_0x49, [0x4A]=LSR_0x4A, [0x4C]=JMP_0x4C, [0x4D]=EOR_0x4D, [0x4E]=LSR_0x4E,
    [0x50]=BVC_0x50, [0x51]=EOR_0x51, [0x55]=EOR_0x55, [0x56]=LSR_0x56, [0x58]=CLI_0x58, [0x59]=EOR_0x59, [0x5D]=EOR_0x5D, [0x5E]=LSR_0x5E,
    [0x60]=RTS_0x60, [0x61]=ADC_0x61, [0x65]=ADC_0x65, [0x66]=ROR_0x66, [0x68]=PLA_0x68, [0x69]=ADC_0x69, [0x6A]=ROR_0x6A, [0x6C]=JMP_0x6C, [0x6D]=ADC_0x6D, [0x6E]=ROR_0x6E,
    [0x70]=BVS_0x70, [0x71]=ADC_0x71, [0x75]=ADC_0x75, [0x76]=ROR_0x76, [0x78]=SEI_0x78, [0x79]=ADC_0x79, [0x7D]=ADC_0x7D, [0x7E]=ROR_0x7E,
    [0x81]=STA_0x81, [0x84]=STY_0x84, [0x85]=STA_0x85, [0x86]=STX_0x86, [0x88]=DEY_0x88, [0x8A]=TXA_0x8A, [0x8C]=STY_0x8C, [0x8D]=STA_0x8D, [0x8E]=STX_0x8E,
    [0x90]=BCC_0x90, [0x91]=STA_0x91, [0x94]=STY_0x94, [0x95]=STA_0x95, [0x96]=STX_0x96, [0x98]=TYA_0x98, [0x99]=STA_0x99, [0x9A]=TXS_0x9A, [0x9D]=STA_0x9D,
    [0xA0]=LDY_0xA0, [0xA1]=LDA_0xA1, [0xA2]=LDX_0xA2, [0xA4]=LDY_0xA4, [0xA5]=LDA_0xA5, [0xA6]=LDX_0xA6, [0xA8]=TAY_0xA8, [0xA9]=LDA_0xA9, [0xAA]=TAX_0xAA, [0xAC]=LDY_0xAC, [0xAD]=LDA_0xAD, [0xAE]=LDX_0xAE,
    [0xB0]=BCS_0xB0, [0xB1]=LDA_0xB1, [0xB4]=LDY_0xB4, [0xB5]=LDA_0xB5, [0xB6]=LDX_0xB6, [0xB8]=CLV_0xB8, [0xB9]=LDA_0xB9, [0xBA]=TSX_0xBA, [0xBC]=LDY_0xBC, [0xBD]=LDA_0xBD, [0xBE]=LDX_0xBE,
    [0xC0]=CPY_0xC0, [0xC1]=CMP_0xC1, [0xC4]=CPY_0xC4, [0xC5]=CMP_0xC5, [0xC6]=DEC_0xC6, [0xC8]=INY_0xC8, [0xC9]=CMP_0xC9, [0xCA]=DEX_0xCA, [0xCC]=CPY_0xCC, [0xCD]=CMP_0xCD, [0xCE]=DEC_0xCE,
    [0xD0]=BNE_0xD0, [0xD1]=CMP_0xD1, [0xD5]=CMP_0xD5, [0xD6]=DEC_0xD6, [0xD8]=CLD_0xD8, [0xD9]=CMP_0xD9, [0xDD]=CMP_0xDD, [0xDE]=DEC_0xDE,
    [0xE0]=CPX_0xE0, [0xE1]=SBC_0xE1, [0xE4]=CPX_0xE4, [0xE5]=SBC_0xE5, [0xE6]=INC_0xE6, [0xE8]=INX_0xE8, [0xE9]=SBC_0xE9, [0xEA]=NOP_0xEA, [0xEC]=CPX_0xEC, [0xED]=SBC_0xED, [0xEE]=INC_0xEE,
    [0xF0]=BEQ_0xF0, [0xF1]=SBC_0xF1, [0xF5]=SBC_0xF5, [0xF6]=INC_0xF6, [0xF8]=SED_0xF8, [0xF9]=SBC_0xF9, [0xFD]=SBC_0xFD, [0xFE]=INC_0xFE,
};
