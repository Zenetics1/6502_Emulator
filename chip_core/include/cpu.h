#ifndef CHIP_H
#define CHIP_H

#include <stdint.h>
#include <stdbool.h>

//Status Register Flag Bits
#define FLAG_CARRY (1 << 0)
#define FLAG_ZERO (1 << 1)
#define FLAG_INTERUPT_DISABLE (1 << 2)
#define FLAG_BREAK (1 << 3)
#define FLAG_DECIMAL (1 << 4)
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
      
    } MOS6502;
    
    typedef struct
    {
        const char* opcode_name;
        uint8_t (*op_func)(MOS6502 *cpu); //Writes result to memory, returns additional cycles for cpu clock
        uint8_t total_cycles;
    } Opcode_Entry;
    
    //Look up table to access various Instruction based on Opcodes
    extern const Opcode_Entry LUT[252];

    //CPU Clock 
    void CLOCK(MOS6502 *cpu);

    //Update Z/N Flags
    void update_Z_N_flags(MOS6502 *cpu, uint8_t reg_result);

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
    
    void TAX(MOS6502 *cpu); //Transfer Accumulator to X
    void TAY(MOS6502 *cpu); //Transfer Accumulator to Y
    void TXA(MOS6502 *cpu); //Transfer X to Accumulator
    void TYA(MOS6502 *cpu); //Transfer T to Accumulator
    void TSX(MOS6502 *cpu); //Transfer Stack Pointer to X
    void TXS(MOS6502 *cpu); //Transfer X to Stack Pointer 

    void PHA(MOS6502 *cpu); //Push Accumulator
    void PHP(MOS6502 *cpu); //Push Processor Status
    void PLA(MOS6502 *cpu); //Pull Accumulator
    void PLP(MOS6502 *cpu); //Push Prcoessor Status

    void AND(MOS6502 *cpu, uint16_t address); //Logical AND
    void EOR(MOS6502 *cpu, uint16_t address); //Exclusive OR
    void ORA(MOS6502 *cpu, uint16_t address); //Logical Inclusive OR
    void BIT(MOS6502 *cpu, uint16_t address); //Bit test

    void CLC(MOS6502 *cpu); //Clear carry flag
    void CLD(MOS6502 *cpu); //Clear decimal mode flag
    void CLI(MOS6502 *cpu); //Clear interrupt disable flag
    void CLV(MOS6502 *cpu); //Clear overflow flag
    void SEC(MOS6502 *cpu); //Set carry flag
    void SED(MOS6502 *cpu); //Set decimal mode flag
    void SEI(MOS6502 *cpu); //Set interrupt disable flags

    void ADC(MOS6502 *cpu, uint16_t address); //Add with Carry
    void SBC(MOS6502 *cpu, uint16_t address); //Subtract with Carry
    void CMP(MOS6502 *cpu, uint16_t address); //Compare Accumulator
    void CPX(MOS6502 *cpu, uint16_t address); //Compare X register
    void CPY(MOS6502 *cpu, uint16_t address); //Compare Y register

    void INC(MOS6502 *cpu, uint16_t address); //Increment a memory location
    void INX(MOS6502 *cpu); //Increment the X register
    void INY(MOS6502 *cpu); //Increment the Y register
    void DEC(MOS6502 *cpu, uint16_t address); //Decrement a memory location
    void DEX(MOS6502 *cpu); //Decrement the X register
    void DEY(MOS6502 *cpu); //Decrement the Y register

    void ASL(MOS6502 *cpu, uint16_t address); //Arithmetic Shift Left
    void LSR(MOS6502 *cpu, uint16_t address); //Logical Shift Right
    void ROL(MOS6502 *cpu, uint16_t address); //Rotate Left
    void ROR(MOS6502 *cpu, uint16_t address); //Rotate Right

    void JMP(MOS6502 *cpu, uint16_t address); //Jump to another location
    void JSR(MOS6502 *cpu, uint16_t address); //Jump to subroutine
    void RTS(MOS6502 *cpu, uint16_t address); //Return from subroutine

    bool BCC(MOS6502 *cpu); //Branch if carry flag clear
    bool BCS(MOS6502 *cpu); //Branch if carry flag set
    bool BEQ(MOS6502 *cpu); //Branch if zero flag set
    bool BMI(MOS6502 *cpu); //Branch if negative flag set
    bool BNE(MOS6502 *cpu); //Branch if zero flag clear
    bool BPL(MOS6502 *cpu); //Branch if neagtive flag clear
    bool BVC(MOS6502 *cpu); //Branch if overflow flag clear
    bool BVS(MOS6502 *cpu); //Branch if overflow flag set

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

#endif