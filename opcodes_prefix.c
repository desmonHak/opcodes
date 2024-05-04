#ifndef __OPCODES_PREFIX_C_
#define __OPCODES_PREFIX_C_

#include "opcodes_prefix.h"

uint8_t get_bit_w(Instruction_info *instrucion) {
    return ((uint8_t)instrucion->instruction.opcode[2].opcode_byte.byte & (uint8_t)instrucion->posicion_w);
}

uint8_t get_bit_d(Instruction_info *instrucion) {
    return instrucion->instruction.opcode[2].opcode_byte.byte & instrucion->posicion_d;
}

uint64_t popcnt_software(uint64_t x)
{
    uint64_t one_count = 0;
    while(x) {
        if(x % 2) ++one_count;
        x >>= 1;
    }
    return one_count;
}
uint64_t count_get_mask(uint64_t x)
{
    /*
     *
     * Cuenta 0's desde la derecha a izquierda hasta obtener algun 1
     *
     */
    for(uint64_t one_count = 0; x; ++one_count, x >>= 1) if(x & 1) return one_count; 
}

static char *get_string_instrution(string_instrution_id id) {
    switch (id)
    {
        case STRING_AAA: return "aaa";
        case STRING_AAD: return "aad";
        case STRING_AAM: return "aam";
        case STRING_AAS: return "aas";
        case STRING_ADC: return "adc";
        
        default: return "error - not exits this instruttion.";
    }
}

static char *get_string_register(encoder_x86 size_word, uint8_t bit_w, register_id id) {
    /*
     *
     *  Se recibe un register_id el cual especifica un registro. Se espera recibir un bit "w"
     *  El cual especifique si se trata de registros de 8bits u de 16/32bits.
     *  Se espera recibir un bit "size_word" el cual indique si se usa operaciones de datos
     *  de 16bits (size_word = 0b0) o si se trata de operaciones de datos de 32bits (size_word = 0b1)
     * 
     */
    switch (popcnt_software(bit_w)){ // si hay un bit, el campo w es 1, si hay 0 bits 1. el campo w esta en 0. si hay mas de 1, error
        case 0b0:  // si el campo "w" fue definido como 0 (8bits)
            switch (id) { // registros de 8 bits
                case 0b000: return "al";
                case 0b001: return "cl";
                case 0b010: return "dl";
                case 0b011: return "bl";
                case 0b100: return "ah";
                case 0b101: return "ch";
                case 0b110: return "dh";
                case 0b111: return "bh";
                default: return  "error - Este registro no esta definido en los 8bits.";
            }
        case 0b1:  // si el campo "w" fue definido como 1
            switch (size_word)
            {
                case 0b0: // para 16 bits
                    switch (id) { // registros de 16 bits
                        case 0b000: return "ax";
                        case 0b001: return "cx";
                        case 0b010: return "dx";
                        case 0b011: return "bx";
                        case 0b100: return "sp";
                        case 0b101: return "bp";
                        case 0b110: return "si";
                        case 0b111: return "di";
                        default: return  "error - Este registro no esta definido en los 16bits.";
                    }
                case 0b1: // para 32 bits
                    switch (id) { // registros de 32 bits
                        case 0b000: return "eax";
                        case 0b001: return "ecx";
                        case 0b010: return "edx";
                        case 0b011: return "ebx";
                        case 0b100: return "esp";
                        case 0b101: return "ebp";
                        case 0b110: return "esi";
                        case 0b111: return "edi";
                        default: return  "error - Este registro no esta definid en los 32bits.";
                    }
                default: return  "error - No se puede operar en este size de datos.";
            }
        default: return "error - campo 'w' excede un bit.";
    }
}


#endif