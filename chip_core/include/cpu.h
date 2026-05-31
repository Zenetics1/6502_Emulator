#include <stdint.h>
#ifdef chip_h
    typedef struct
    {   
        uint8_t A;
        uint8_t S;
        uint8_t X;
        uint8_t Y;

        typedef union 
        {
           uint8_t RAW_P;
           typedef struct 
           {
                uint8_t n : 1;
                uint8_t v : 1;
                uint8_t b : 1;
                uint8_t d : 1;
                uint8_t i : 1;
                uint8_t z : 1;
                uint8_t c : 1;
           } FLAGS;
           
        } P;

        typedef union 
        {
            uint16_t RAW_PC;
            typedef struct
            {
                uint8_t PC_LOWER;
                uint8_t PC_UPPER;
            } PC_BYTES;
            
        } PC;
        
        
    } 6502_REGISTERS;
    
    
#endif