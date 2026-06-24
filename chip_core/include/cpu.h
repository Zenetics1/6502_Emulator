#ifndef CHIP_H
#define CHIP_H

#include <stdint.h>
#include <stdbool.h>

//Status Register Flag Bits
#define FLAG_CARRY (1 << 0)
#define FLAG_ZERO (1 << 1)
#define FLAG_INTERUPT_DISABLE (1 << 2)
#define FLAG_DECIMAL (1 << 3)
#define FLAG_BREAK (1 << 4)
#define FLAG_UNUSED (1 << 5)
#define FLAG_OVERFLOW (1 << 6)
#define FLAG_NEGATIVE (1 << 7)

#define IQR_Interrupt_Vector 0xFFFE

    typedef struct
    {
        uint8_t A; //Main math register, one of the values must be here during ALU operations, often holds final result
        uint8_t S; //LIFO, General Purpose helper register, focuses on memory indexing
        uint8_t X; //Holds loop counters or memory offset
        uint8_t Y; //Similar to x reigister, but Indirect indexed Addressing is exclusive to this
        uint8_t P; //Status Register, remembers most recent calculation result with individual flags
        uint16_t PC; //Program Counter, determine the next instruction for cpu

        //State
        uint8_t IR; //Instruction Register
        uint8_t cycles_left; //Timing state Ring
        bool branch_result; //Result written by branch instruction functions
        bool accumulator_mode; //Set by addr_func to signal shift instructions use accumulator

    } MOS6502;
    
    //Look up table to access various Instruction based on Opcodes
    extern const uint8_t (*LUT[256])(MOS6502*);

    //CPU Clock 
    void CLOCK(MOS6502 *cpu);

    /*Addressing Mode Helper Functions*/
    //Implied and Accumulator Addressing modes do not target address, no functions needed
    uint16_t Immediate_Mode(MOS6502 *cpu);
    uint16_t Zero_Page_Mode(MOS6502 *cpu);
    uint16_t Zero_PageX_Mode(MOS6502 *cpu);
    uint16_t Zero_PageY_Mode(MOS6502 *cpu);
    uint16_t Absolute_Mode(MOS6502 *cpu);
    uint16_t AbsoluteX_Mode(MOS6502 *cpu);
    uint16_t AbsoluteY_Mode(MOS6502 *cpu);
    uint16_t Absolute_Indirect_Mode(MOS6502 *cpu);
    uint16_t Relative_Mode(MOS6502 *cpu);
    uint16_t Indexed_Indirect_Mode(MOS6502 *cpu);
    uint16_t Indirect_Indexed_Mode(MOS6502 *cpu);


    /*Instruction Helper Functions*/
    void LDA(MOS6502 *cpu, uint16_t address); //Load Accumulator
    void LDX(MOS6502 *cpu, uint16_t address); //Load X Register
    void LDY(MOS6502 *cpu, uint16_t address); //Load Y Register
    void LSR(MOS6502 *cpu, uint16_t address); //Logical Shift Right
    void STA(MOS6502 *cpu, uint16_t address); //Store Accumulator
    void STX(MOS6502 *cpu, uint16_t address); //Store X Register
    void STY(MOS6502 *cpu, uint16_t address); //Store Y Register
    
    void TAX(MOS6502 *cpu, uint16_t address); //Transfer Accumulator to X
    void TAY(MOS6502 *cpu, uint16_t address); //Transfer Accumulator to Y
    void TXA(MOS6502 *cpu, uint16_t address); //Transfer X to Accumulator
    void TYA(MOS6502 *cpu, uint16_t address); //Transfer T to Accumulator
    void TSX(MOS6502 *cpu, uint16_t address); //Transfer Stack Pointer to X
    void TXS(MOS6502 *cpu, uint16_t address); //Transfer X to Stack Pointer

    void PHA(MOS6502 *cpu, uint16_t address); //Push Accumulator
    void PHP(MOS6502 *cpu, uint16_t address); //Push Processor Status
    void PLA(MOS6502 *cpu, uint16_t address); //Pull Accumulator
    void PLP(MOS6502 *cpu, uint16_t address); //Push Prcoessor Status

    void AND(MOS6502 *cpu, uint16_t address); //Logical AND
    void EOR(MOS6502 *cpu, uint16_t address); //Exclusive OR
    void ORA(MOS6502 *cpu, uint16_t address); //Logical Inclusive OR
    void BIT(MOS6502 *cpu, uint16_t address); //Bit test

    void CLC(MOS6502 *cpu, uint16_t address); //Clear carry flag
    void CLD(MOS6502 *cpu, uint16_t address); //Clear decimal mode flag
    void CLI(MOS6502 *cpu, uint16_t address); //Clear interrupt disable flag
    void CLV(MOS6502 *cpu, uint16_t address); //Clear overflow flag
    void SEC(MOS6502 *cpu, uint16_t address); //Set carry flag
    void SED(MOS6502 *cpu, uint16_t address); //Set decimal mode flag
    void SEI(MOS6502 *cpu, uint16_t address); //Set interrupt disable flags

    void ADC(MOS6502 *cpu, uint16_t address); //Add with Carry
    void SBC(MOS6502 *cpu, uint16_t address); //Subtract with Carry
    void CMP(MOS6502 *cpu, uint16_t address); //Compare Accumulator
    void CPX(MOS6502 *cpu, uint16_t address); //Compare X register
    void CPY(MOS6502 *cpu, uint16_t address); //Compare Y register

    void INC(MOS6502 *cpu, uint16_t address); //Increment a memory location
    void INX(MOS6502 *cpu, uint16_t address); //Increment the X register
    void INY(MOS6502 *cpu, uint16_t address); //Increment the Y register
    void DEC(MOS6502 *cpu, uint16_t address); //Decrement a memory location
    void DEX(MOS6502 *cpu, uint16_t address); //Decrement the X register
    void DEY(MOS6502 *cpu, uint16_t address); //Decrement the Y register

    void ASL(MOS6502 *cpu, uint16_t address); //Arithmetic Shift Left
    void ROL(MOS6502 *cpu, uint16_t address); //Rotate Left
    void ROR(MOS6502 *cpu, uint16_t address); //Rotate Right

    void JMP(MOS6502 *cpu, uint16_t address); //Jump to another location
    void JSR(MOS6502 *cpu, uint16_t address); //Jump to subroutine
    void RTS(MOS6502 *cpu, uint16_t address); //Return from subroutine

    void BCC(MOS6502 *cpu); //Branch if carry flag clear
    void BCS(MOS6502 *cpu); //Branch if carry flag set
    void BEQ(MOS6502 *cpu); //Branch if zero flag set
    void BMI(MOS6502 *cpu); //Branch if negative flag set
    void BNE(MOS6502 *cpu); //Branch if zero flag clear
    void BPL(MOS6502 *cpu); //Branch if neagtive flag clear
    void BVC(MOS6502 *cpu); //Branch if overflow flag clear
    void BVS(MOS6502 *cpu); //Branch if overflow flag set

    void BRK(MOS6502 *cpu, uint16_t address); //Force and interrupt
    void NOP(MOS6502 *cpu, uint16_t address); //No Operation
    void RTI(MOS6502 *cpu, uint16_t address); //Return from Interrupt


    /*Implied Opcode*/
    //System Control/Break, Operational State instructions
    uint8_t NOP_0xEA(MOS6502 *cpu); // No Operation: Step past byte, 2 cycles 
    uint8_t BRK_0x00(MOS6502 *cpu); // Break/Software Interrupt: Push curr PC(+2) onto stack, Break bit = 1, Push Status Register(P), Interrupt Disable Flag() = 1, Load interrupt vector at $FFFE and $FFFF, 7 cycles
    //Subroutine/Interrupt Return, popps addresses of stack to change where PC points
    uint8_t RTI_0x40(MOS6502 *cpu); // Return from Interrupt: Exec at end of interrupt handler, Pull Status Flag off stack 1st, Pull 16-bit target return address to restore PC, 6 cycles
    uint8_t RTS_0x60(MOS6502 *cpu); // Return from Subroutine: Exec at end of func call, Pull 16-bit address off stack(low then high byte) and adds 1, Sets to PC, 6 cycles
    //Stack Operations, interacts w/systam RAM at page 1($0100 - $01FF) using SP as lower byte offset
    uint8_t PLP_0x28(MOS6502 *cpu); // Pull Processor Status: Inc SP, reads byte from stack, Overwrites Flag Register, 4 Cycles
    uint8_t PLA_0x68(MOS6502 *cpu); // Accumulator: Inc SP + 1, read byte from $0100 + SP, load into A, update Z/N flag, 4 cycles
    uint8_t PHP_0x08(MOS6502 *cpu); // Push Processor Status: Write Status Register Flag to stack memory, dec SP, 3 cycles. (Bit 4&5 must always pushed to stack as 1)
    uint8_t PHA_0x48(MOS6502 *cpu); // Push Accumulator: Write value of A register to mem addr $0100 + SP, then dec SP - 1, 3 cycle
    //Inc/Dec Instructions, All 2 cycles, Update Z/N Flags
    uint8_t DEY_0x88(MOS6502 *cpu); // Dec Y: Subtract 1 from Y Register
    uint8_t DEX_0xCA(MOS6502 *cpu); // Dec X: Subtract 1 from X Register
    uint8_t INY_0xC8(MOS6502 *cpu); // Inc Y: Add 1 to Y Register
    uint8_t INX_0xE8(MOS6502 *cpu); // Inc X: Add 1 to X Register
    //Register Transfer(8-bit), All 2 cycles, Update Z/N Flags
    uint8_t TXS_0x9A(MOS6502 *cpu); //Transfer X to Stack Pointer: SP = X, does not change Z/N Flags
    uint8_t TSX_0xBA(MOS6502 *cpu); //Copy Stack Pointer Register to X Register: X = SP
    uint8_t TYA_0x98(MOS6502 *cpu); //Transfer Y to A: A = Y
    uint8_t TAY_0xA8(MOS6502 *cpu); //Transfer A to Y: Y = A
    uint8_t TXA_0x8A(MOS6502 *cpu); //Transfer X to A: A = X
    uint8_t TAX_0xAA(MOS6502 *cpu); //Transfer A to X: X = A
    //Flag Control, All 2 cycles
    uint8_t CLV_0xB8(MOS6502 *cpu); //Clear Overflow Flag: V = 0
    uint8_t SEI_0x78(MOS6502 *cpu); //Set Interrupt Disable: I = 1, ignores IRQ lines
    uint8_t CLI_0x58(MOS6502 *cpu); //Clear Interrupt Disable: I = 0, enables IRQ lines
    uint8_t SED_0xF8(MOS6502 *cpu); //Set Decimal Mode: D = 1, ALU to perform Binary-Coded Decimal Calc with ADC/SBC
    uint8_t CLD_0xD8(MOS6502 *cpu); //Clear Decimal Mode: D = 0, ALU does Normal Binary Math
    uint8_t SEC_0x38(MOS6502 *cpu); //Set Carry Flag: C = 1, used before SBC subtraction, 6502 uses inverted carry bit for "borrow" indicator
    uint8_t CLC_0x18(MOS6502 *cpu); //Clear Carry Flag: C = 0, before fresh ADC addition, so residual carry is not added
    //Accumulator Shift/Rotate, All 2 cycles, Update C/Z/N Flags
    uint8_t ASL_0x0A(MOS6502 *cpu); //Arithmetic Shift Left (Accumulator): Old bit 7 -> C, A shifted left 1, bit 0 = 0
    uint8_t ROL_0x2A(MOS6502 *cpu); //Rotate Left (Accumulator): Old bit 7 -> C, A shifted left 1, old C -> bit 0
    uint8_t LSR_0x4A(MOS6502 *cpu); //Logical Shift Right (Accumulator): Old bit 0 -> C, A shifted right 1, bit 7 = 0
    uint8_t ROR_0x6A(MOS6502 *cpu); //Rotate Right (Accumulator): Old bit 0 -> C, A shifted right 1, old C -> bit 7
    //Branch Instructions, 2 cycles base, +1 if branch taken, +2 if page boundary crossed
    uint8_t BPL_0x10(MOS6502 *cpu); //Branch if Positive: PC += signed offset if N = 0
    uint8_t BMI_0x30(MOS6502 *cpu); //Branch if Minus: PC += signed offset if N = 1
    uint8_t BVC_0x50(MOS6502 *cpu); //Branch if Overflow Clear: PC += signed offset if V = 0
    uint8_t BVS_0x70(MOS6502 *cpu); //Branch if Overflow Set: PC += signed offset if V = 1
    uint8_t BCC_0x90(MOS6502 *cpu); //Branch if Carry Clear: PC += signed offset if C = 0
    uint8_t BCS_0xB0(MOS6502 *cpu); //Branch if Carry Set: PC += signed offset if C = 1
    uint8_t BNE_0xD0(MOS6502 *cpu); //Branch if Not Equal: PC += signed offset if Z = 0
    uint8_t BEQ_0xF0(MOS6502 *cpu); //Branch if Equal: PC += signed offset if Z = 1
    //Jump/Subroutine Instructions
    uint8_t JSR_0x20(MOS6502 *cpu); //Jump to Subroutine: Push PC-1 onto stack (high then low), load 2-byte absolute address into PC, 6 cycles
    uint8_t JMP_0x4C(MOS6502 *cpu); //Jump Absolute: Load 2-byte absolute address into PC, 3 cycles
    uint8_t JMP_0x6C(MOS6502 *cpu); //Jump Indirect: Read pointer from PC, load effective address into PC (page-wrap bug at $xxFF), 5 cycles
    //Load Instructions
    //LDA - Load Accumulator, Updates Z/N flags
    uint8_t LDA_0xA9(MOS6502 *cpu); //LDA Immediate: A = imm, 2 cycles
    uint8_t LDA_0xA5(MOS6502 *cpu); //LDA Zero Page: A = mem[zp], 3 cycles
    uint8_t LDA_0xB5(MOS6502 *cpu); //LDA Zero Page,X: A = mem[zp+X], 4 cycles
    uint8_t LDA_0xAD(MOS6502 *cpu); //LDA Absolute: A = mem[abs], 4 cycles
    uint8_t LDA_0xBD(MOS6502 *cpu); //LDA Absolute,X: A = mem[abs+X], 4+1 cycles
    uint8_t LDA_0xB9(MOS6502 *cpu); //LDA Absolute,Y: A = mem[abs+Y], 4+1 cycles
    uint8_t LDA_0xA1(MOS6502 *cpu); //LDA (Indirect,X): A = mem[mem[zp+X]], 6 cycles
    uint8_t LDA_0xB1(MOS6502 *cpu); //LDA (Indirect),Y: A = mem[mem[zp]+Y], 5+1 cycles
    //LDX - Load X Register, Updates Z/N flags
    uint8_t LDX_0xA2(MOS6502 *cpu); //LDX Immediate: X = imm, 2 cycles
    uint8_t LDX_0xA6(MOS6502 *cpu); //LDX Zero Page: X = mem[zp], 3 cycles
    uint8_t LDX_0xB6(MOS6502 *cpu); //LDX Zero Page,Y: X = mem[zp+Y], 4 cycles
    uint8_t LDX_0xAE(MOS6502 *cpu); //LDX Absolute: X = mem[abs], 4 cycles
    uint8_t LDX_0xBE(MOS6502 *cpu); //LDX Absolute,Y: X = mem[abs+Y], 4+1 cycles
    //LDY - Load Y Register, Updates Z/N flags
    uint8_t LDY_0xA0(MOS6502 *cpu); //LDY Immediate: Y = imm, 2 cycles
    uint8_t LDY_0xA4(MOS6502 *cpu); //LDY Zero Page: Y = mem[zp], 3 cycles
    uint8_t LDY_0xB4(MOS6502 *cpu); //LDY Zero Page,X: Y = mem[zp+X], 4 cycles
    uint8_t LDY_0xAC(MOS6502 *cpu); //LDY Absolute: Y = mem[abs], 4 cycles
    uint8_t LDY_0xBC(MOS6502 *cpu); //LDY Absolute,X: Y = mem[abs+X], 4+1 cycles
    //Store Instructions
    //STA - Store Accumulator, No flag updates
    uint8_t STA_0x85(MOS6502 *cpu); //STA Zero Page: mem[zp] = A, 3 cycles
    uint8_t STA_0x95(MOS6502 *cpu); //STA Zero Page,X: mem[zp+X] = A, 4 cycles
    uint8_t STA_0x8D(MOS6502 *cpu); //STA Absolute: mem[abs] = A, 4 cycles
    uint8_t STA_0x9D(MOS6502 *cpu); //STA Absolute,X: mem[abs+X] = A, 5 cycles
    uint8_t STA_0x99(MOS6502 *cpu); //STA Absolute,Y: mem[abs+Y] = A, 5 cycles
    uint8_t STA_0x81(MOS6502 *cpu); //STA (Indirect,X): mem[mem[zp+X]] = A, 6 cycles
    uint8_t STA_0x91(MOS6502 *cpu); //STA (Indirect),Y: mem[mem[zp]+Y] = A, 6 cycles
    //STX - Store X Register, No flag updates
    uint8_t STX_0x86(MOS6502 *cpu); //STX Zero Page: mem[zp] = X, 3 cycles
    uint8_t STX_0x96(MOS6502 *cpu); //STX Zero Page,Y: mem[zp+Y] = X, 4 cycles
    uint8_t STX_0x8E(MOS6502 *cpu); //STX Absolute: mem[abs] = X, 4 cycles
    //STY - Store Y Register, No flag updates
    uint8_t STY_0x84(MOS6502 *cpu); //STY Zero Page: mem[zp] = Y, 3 cycles
    uint8_t STY_0x94(MOS6502 *cpu); //STY Zero Page,X: mem[zp+X] = Y, 4 cycles
    uint8_t STY_0x8C(MOS6502 *cpu); //STY Absolute: mem[abs] = Y, 4 cycles
    //Arithmetic Instructions
    //ADC - Add with Carry, Updates C/Z/V/N flags
    uint8_t ADC_0x69(MOS6502 *cpu); //ADC Immediate: A = A + imm + C, 2 cycles
    uint8_t ADC_0x65(MOS6502 *cpu); //ADC Zero Page: A = A + mem[zp] + C, 3 cycles
    uint8_t ADC_0x75(MOS6502 *cpu); //ADC Zero Page,X: A = A + mem[zp+X] + C, 4 cycles
    uint8_t ADC_0x6D(MOS6502 *cpu); //ADC Absolute: A = A + mem[abs] + C, 4 cycles
    uint8_t ADC_0x7D(MOS6502 *cpu); //ADC Absolute,X: A = A + mem[abs+X] + C, 4+1 cycles
    uint8_t ADC_0x79(MOS6502 *cpu); //ADC Absolute,Y: A = A + mem[abs+Y] + C, 4+1 cycles
    uint8_t ADC_0x61(MOS6502 *cpu); //ADC (Indirect,X): A = A + mem[mem[zp+X]] + C, 6 cycles
    uint8_t ADC_0x71(MOS6502 *cpu); //ADC (Indirect),Y: A = A + mem[mem[zp]+Y] + C, 5+1 cycles
    //SBC - Subtract with Carry, Updates C/Z/V/N flags
    uint8_t SBC_0xE9(MOS6502 *cpu); //SBC Immediate: A = A - imm - (1-C), 2 cycles
    uint8_t SBC_0xE5(MOS6502 *cpu); //SBC Zero Page: A = A - mem[zp] - (1-C), 3 cycles
    uint8_t SBC_0xF5(MOS6502 *cpu); //SBC Zero Page,X: A = A - mem[zp+X] - (1-C), 4 cycles
    uint8_t SBC_0xED(MOS6502 *cpu); //SBC Absolute: A = A - mem[abs] - (1-C), 4 cycles
    uint8_t SBC_0xFD(MOS6502 *cpu); //SBC Absolute,X: A = A - mem[abs+X] - (1-C), 4+1 cycles
    uint8_t SBC_0xF9(MOS6502 *cpu); //SBC Absolute,Y: A = A - mem[abs+Y] - (1-C), 4+1 cycles
    uint8_t SBC_0xE1(MOS6502 *cpu); //SBC (Indirect,X): A = A - mem[mem[zp+X]] - (1-C), 6 cycles
    uint8_t SBC_0xF1(MOS6502 *cpu); //SBC (Indirect),Y: A = A - mem[mem[zp]+Y] - (1-C), 5+1 cycles
    //Logical Instructions
    //AND - Logical AND, Updates Z/N flags
    uint8_t AND_0x29(MOS6502 *cpu); //AND Immediate: A = A & imm, 2 cycles
    uint8_t AND_0x25(MOS6502 *cpu); //AND Zero Page: A = A & mem[zp], 3 cycles
    uint8_t AND_0x35(MOS6502 *cpu); //AND Zero Page,X: A = A & mem[zp+X], 4 cycles
    uint8_t AND_0x2D(MOS6502 *cpu); //AND Absolute: A = A & mem[abs], 4 cycles
    uint8_t AND_0x3D(MOS6502 *cpu); //AND Absolute,X: A = A & mem[abs+X], 4+1 cycles
    uint8_t AND_0x39(MOS6502 *cpu); //AND Absolute,Y: A = A & mem[abs+Y], 4+1 cycles
    uint8_t AND_0x21(MOS6502 *cpu); //AND (Indirect,X): A = A & mem[mem[zp+X]], 6 cycles
    uint8_t AND_0x31(MOS6502 *cpu); //AND (Indirect),Y: A = A & mem[mem[zp]+Y], 5+1 cycles
    //ORA - Logical Inclusive OR, Updates Z/N flags
    uint8_t ORA_0x09(MOS6502 *cpu); //ORA Immediate: A = A | imm, 2 cycles
    uint8_t ORA_0x05(MOS6502 *cpu); //ORA Zero Page: A = A | mem[zp], 3 cycles
    uint8_t ORA_0x15(MOS6502 *cpu); //ORA Zero Page,X: A = A | mem[zp+X], 4 cycles
    uint8_t ORA_0x0D(MOS6502 *cpu); //ORA Absolute: A = A | mem[abs], 4 cycles
    uint8_t ORA_0x1D(MOS6502 *cpu); //ORA Absolute,X: A = A | mem[abs+X], 4+1 cycles
    uint8_t ORA_0x19(MOS6502 *cpu); //ORA Absolute,Y: A = A | mem[abs+Y], 4+1 cycles
    uint8_t ORA_0x01(MOS6502 *cpu); //ORA (Indirect,X): A = A | mem[mem[zp+X]], 6 cycles
    uint8_t ORA_0x11(MOS6502 *cpu); //ORA (Indirect),Y: A = A | mem[mem[zp]+Y], 5+1 cycles
    //EOR - Exclusive OR, Updates Z/N flags
    uint8_t EOR_0x49(MOS6502 *cpu); //EOR Immediate: A = A ^ imm, 2 cycles
    uint8_t EOR_0x45(MOS6502 *cpu); //EOR Zero Page: A = A ^ mem[zp], 3 cycles
    uint8_t EOR_0x55(MOS6502 *cpu); //EOR Zero Page,X: A = A ^ mem[zp+X], 4 cycles
    uint8_t EOR_0x4D(MOS6502 *cpu); //EOR Absolute: A = A ^ mem[abs], 4 cycles
    uint8_t EOR_0x5D(MOS6502 *cpu); //EOR Absolute,X: A = A ^ mem[abs+X], 4+1 cycles
    uint8_t EOR_0x59(MOS6502 *cpu); //EOR Absolute,Y: A = A ^ mem[abs+Y], 4+1 cycles
    uint8_t EOR_0x41(MOS6502 *cpu); //EOR (Indirect,X): A = A ^ mem[mem[zp+X]], 6 cycles
    uint8_t EOR_0x51(MOS6502 *cpu); //EOR (Indirect),Y: A = A ^ mem[mem[zp]+Y], 5+1 cycles
    //BIT - Bit Test, Z=(A&mem)==0, V=mem bit6, N=mem bit7
    uint8_t BIT_0x24(MOS6502 *cpu); //BIT Zero Page: Test A & mem[zp], 3 cycles
    uint8_t BIT_0x2C(MOS6502 *cpu); //BIT Absolute: Test A & mem[abs], 4 cycles
    //Compare Instructions
    //CMP - Compare Accumulator, Updates C/Z/N flags (C=1 if A>=mem)
    uint8_t CMP_0xC9(MOS6502 *cpu); //CMP Immediate: A - imm, 2 cycles
    uint8_t CMP_0xC5(MOS6502 *cpu); //CMP Zero Page: A - mem[zp], 3 cycles
    uint8_t CMP_0xD5(MOS6502 *cpu); //CMP Zero Page,X: A - mem[zp+X], 4 cycles
    uint8_t CMP_0xCD(MOS6502 *cpu); //CMP Absolute: A - mem[abs], 4 cycles
    uint8_t CMP_0xDD(MOS6502 *cpu); //CMP Absolute,X: A - mem[abs+X], 4+1 cycles
    uint8_t CMP_0xD9(MOS6502 *cpu); //CMP Absolute,Y: A - mem[abs+Y], 4+1 cycles
    uint8_t CMP_0xC1(MOS6502 *cpu); //CMP (Indirect,X): A - mem[mem[zp+X]], 6 cycles
    uint8_t CMP_0xD1(MOS6502 *cpu); //CMP (Indirect),Y: A - mem[mem[zp]+Y], 5+1 cycles
    //CPX - Compare X Register, Updates C/Z/N flags (C=1 if X>=mem)
    uint8_t CPX_0xE0(MOS6502 *cpu); //CPX Immediate: X - imm, 2 cycles
    uint8_t CPX_0xE4(MOS6502 *cpu); //CPX Zero Page: X - mem[zp], 3 cycles
    uint8_t CPX_0xEC(MOS6502 *cpu); //CPX Absolute: X - mem[abs], 4 cycles
    //CPY - Compare Y Register, Updates C/Z/N flags (C=1 if Y>=mem)
    uint8_t CPY_0xC0(MOS6502 *cpu); //CPY Immediate: Y - imm, 2 cycles
    uint8_t CPY_0xC4(MOS6502 *cpu); //CPY Zero Page: Y - mem[zp], 3 cycles
    uint8_t CPY_0xCC(MOS6502 *cpu); //CPY Absolute: Y - mem[abs], 4 cycles
    //Memory Shift/Rotate Instructions, Updates C/Z/N flags
    //ASL Memory - Arithmetic Shift Left, Old bit7 -> C, bit0 = 0
    uint8_t ASL_0x06(MOS6502 *cpu); //ASL Zero Page: mem[zp] <<= 1, 5 cycles
    uint8_t ASL_0x16(MOS6502 *cpu); //ASL Zero Page,X: mem[zp+X] <<= 1, 6 cycles
    uint8_t ASL_0x0E(MOS6502 *cpu); //ASL Absolute: mem[abs] <<= 1, 6 cycles
    uint8_t ASL_0x1E(MOS6502 *cpu); //ASL Absolute,X: mem[abs+X] <<= 1, 7 cycles
    //LSR Memory - Logical Shift Right, bit7 = 0, Old bit0 -> C
    uint8_t LSR_0x46(MOS6502 *cpu); //LSR Zero Page: mem[zp] >>= 1, 5 cycles
    uint8_t LSR_0x56(MOS6502 *cpu); //LSR Zero Page,X: mem[zp+X] >>= 1, 6 cycles
    uint8_t LSR_0x4E(MOS6502 *cpu); //LSR Absolute: mem[abs] >>= 1, 6 cycles
    uint8_t LSR_0x5E(MOS6502 *cpu); //LSR Absolute,X: mem[abs+X] >>= 1, 7 cycles
    //ROL Memory - Rotate Left, Old bit7 -> C, Old C -> bit0
    uint8_t ROL_0x26(MOS6502 *cpu); //ROL Zero Page: mem[zp] rotate left, 5 cycles
    uint8_t ROL_0x36(MOS6502 *cpu); //ROL Zero Page,X: mem[zp+X] rotate left, 6 cycles
    uint8_t ROL_0x2E(MOS6502 *cpu); //ROL Absolute: mem[abs] rotate left, 6 cycles
    uint8_t ROL_0x3E(MOS6502 *cpu); //ROL Absolute,X: mem[abs+X] rotate left, 7 cycles
    //ROR Memory - Rotate Right, Old bit0 -> C, Old C -> bit7
    uint8_t ROR_0x66(MOS6502 *cpu); //ROR Zero Page: mem[zp] rotate right, 5 cycles
    uint8_t ROR_0x76(MOS6502 *cpu); //ROR Zero Page,X: mem[zp+X] rotate right, 6 cycles
    uint8_t ROR_0x6E(MOS6502 *cpu); //ROR Absolute: mem[abs] rotate right, 6 cycles
    uint8_t ROR_0x7E(MOS6502 *cpu); //ROR Absolute,X: mem[abs+X] rotate right, 7 cycles
    //Memory Increment/Decrement Instructions, Updates Z/N flags
    //INC - Increment Memory
    uint8_t INC_0xE6(MOS6502 *cpu); //INC Zero Page: mem[zp] += 1, 5 cycles
    uint8_t INC_0xF6(MOS6502 *cpu); //INC Zero Page,X: mem[zp+X] += 1, 6 cycles
    uint8_t INC_0xEE(MOS6502 *cpu); //INC Absolute: mem[abs] += 1, 6 cycles
    uint8_t INC_0xFE(MOS6502 *cpu); //INC Absolute,X: mem[abs+X] += 1, 7 cycles
    //DEC - Decrement Memory
    uint8_t DEC_0xC6(MOS6502 *cpu); //DEC Zero Page: mem[zp] -= 1, 5 cycles
    uint8_t DEC_0xD6(MOS6502 *cpu); //DEC Zero Page,X: mem[zp+X] -= 1, 6 cycles
    uint8_t DEC_0xCE(MOS6502 *cpu); //DEC Absolute: mem[abs] -= 1, 6 cycles
    uint8_t DEC_0xDE(MOS6502 *cpu); //DEC Absolute,X: mem[abs+X] -= 1, 7 cycles

#endif