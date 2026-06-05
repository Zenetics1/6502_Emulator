#ifndef CHIP_H
#define CHIP_H

#include <stdint.h>
#include <stdbool.h>


//Status Register Flag Bits
#define FLAG_CARRY (1 << 0);
#define FLAG_ZERO (1 << 1);
#define FLAG_INTERUPPT_DISABLE (1 << 2);
#define FLAG_BREAK (1 << 3);
#define FLAG_DECIMAL (1 << 4);
#define FLAG_UNUSED (1 << 5);
#define FLAG_OVERFLOW (1 << 6);
#define FLAG_NEGATIVE (1 << 7);

    typedef struct
    {
        uint8_t A; //Main math register, one of the values must be here during ALU operations, often holds final result
        uint8_t S; //LIFO, 
        //General Purpose helper register, focuses on memory indexing
        uint8_t X; //Holds loop counters or memory offset
        uint8_t Y; //Similar to x reigister, but Indirect indexed Addressing is exclusive to this
        uint8_t P; //Status Register, remembers most recent calculation result with individual flags
        uint16_t PC; //Program Counter, determine the next instruction for cpu

        //State
        uint8_t IR; //Instruction Register
        uint8_t cycles_left; //Timing state Ring
        uint8_t cycle_steps; //Sub-cycle counter for instructions

        //Internal Registers, replicating micro architecture addressing modes
        uint8_t low_byte;
        uint8_t high_byte;
        uint16_t full_address;

        //Pins
        uint16_t address_pin;
        uint8_t data_pin;
        bool read_write;
        
    } MOS6502;
    
    typedef struct
    {
        const char* opcode_name;
        void (*op_func)(MOS6502 *cpu);
        uint8_t total_cycles;
    } Opcode_Entry;
    
    extern const Opcode_Entry LUT[256];

    //CPU Clock 
    void CLOCK(MOS6502 *cpu);
    //Opcode execution
    uint8_t opcode_execute(MOS6502 *cpu, uint8_t IR_OPCODE);

    //Addressing Modes:
    bool mode_implied(MOS6502 *cpu, uint16_t* o_address);
    bool mode_accumulator(MOS6502 *cpu, uint16_t* o_address);
    bool mode_immediate(MOS6502 *cpu, uint16_t* o_address);
    bool mode_absolute(MOS6502 *cpu, uint16_t* o_address);
    bool mode_absolute_x(MOS6502 *cpu, uint16_t* o_address);
    bool mode_absolute_y(MOS6502 *cpu, uint16_t* o_address);
    bool mode_zero_page(MOS6502 *cpu, uint16_t* o_address);
    bool mode_zero_page_x(MOS6502 *cpu, uint16_t* o_address);
    bool mode_zero_page_y(MOS6502 *cpu, uint16_t* o_address);
    bool mode_relative(MOS6502 *cpu, uint16_t* o_address);
    bool mode_indirect(MOS6502 *cpu, uint16_t* o_address);
    bool mode_indexed_indirect_x(MOS6502 *cpu, uint16_t* o_address);
    bool mode_indirect_indexed_y(MOS6502 *cpu, uint16_t* o_address);

    //Instructions:
    void LDA(MOS6502 *cpu, uint16_t address, uint8_t last_cycle);
#endif