#ifndef CHIP_H
#define CHIP_H

#include <stdint.h>
#include <stdbool.h>

    typedef struct
    {
        //Status Register Flag Bits
        #define FLAG_CARRY (1 << 0);
        #define FLAG_ZERO (1 << 1);
        #define FLAG_INTERUPPT_DISABLE (1 << 2);
        #define FLAG_BREAK (1 << 3);
        #define FLAG_DECIMAL (1 << 4);
        #define FLAG_UNUSED (1 << 5);
        #define FLAG_OVERFLOW (1 << 6);
        #define FLAG_NEGATIVE (1 << 7);

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

        //Pins
        uint16_t address_pin;
        uint8_t data_pin;
        bool read_write;
        
    } MOS6502;
    
    void CLOCK(MOS6502 *cpu);

    uint8_t opcode_execute(MOS6502 *cpu, uint8_t IR_OPCODE);

    // typedef struct
    // {   
    //     uint8_t A;
    //     uint8_t S;
    //     uint8_t X;
    //     uint8_t Y;

    //     typedef union 
    //     {
    //        uint8_t RAW_P;
    //        typedef struct 
    //        {
    //             uint8_t n : 1;
    //             uint8_t v : 1;
    //             uint8_t b : 1;
    //             uint8_t d : 1;
    //             uint8_t i : 1;
    //             uint8_t z : 1;
    //             uint8_t c : 1;
    //        } FLAGS;
           
    //     } P;

    //     typedef union 
    //     {
    //         uint16_t RAW_PC;
    //         typedef struct
    //         {
    //             uint8_t PC_LOWER;
    //             uint8_t PC_UPPER;
    //         } PC_BYTES;
            
    //     } PC;
        
        
    // } 6502_REGISTERS;
    
    
#endif