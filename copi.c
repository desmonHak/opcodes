#ifndef __PRINT_STRUCTS_FORMAT_C_
#define __PRINT_STRUCTS_FORMAT_C_

#include "print_structs_format.h"

void print_binary(unsigned int num, uint16_t num_bits, uint16_t init_count) {
    if (init_count != 0)
        for (uint16_t i = 0; i < init_count; i++) {
            printf("-");
            if (i % 4 == 0 && i != 0) printf(" "); 
        }
    for (uint16_t i = num_bits - 1; i >= 0; i--) {
        printf_color("#{FG:lyellow}%d#{FG:reset}", (num >> i) & 1);
        if (i % 4 == 0 && i != 0) printf(" "); 
    } 
    for (uint16_t i = 0; i < 8-num_bits; i++) {
        printf("-");
        if (i % 4 == 0 && i != 0) printf(" "); 
    }
    printf("\n");
}

void print_opcode(opcode opcode, uint8_t number_opcode) {
    printf_color("opcode#{FG:lpurple}{#{FG:lred}%d#{FG:lpurple}}#{FG:reset}:    #{FG:lgreen}%02X#{FG:reset} = #{FG:lyellow}", number_opcode, *((uint8_t*)(&opcode)), 0);
    print_binary( *((uint8_t*)(&opcode)), 8, 0);
    printf_color("#{FG:reset}opcode[#{FG:lred}%d#{FG:reset}]:    #{FG:lgreen}%02X#{FG:reset} = #{FG:lyellow}", number_opcode, opcode.opcode_bits_final.opcode << 2, 0);
    print_binary(opcode.opcode_bits_final.opcode << 2, 8, 0);
    printf_color("#{FG:reset}opcode[#{FG:lred}%d#{FG:reset}].d:  #{FG:lgreen}%02X#{FG:reset} = ---- --#{FG:lyellow}%d#{FG:reset}-\n", number_opcode, opcode.opcode_bits_final.d, opcode.opcode_bits_final.d);
    printf_color("opcode[#{FG:lred}%d#{FG:reset}].s:  #{FG:lgreen}%02X#{FG:reset} = ---- ---#{FG:lyellow}%d#{FG:reset}\n\n", number_opcode, opcode.opcode_bits_final.s, opcode.opcode_bits_final.s);
}

void print_instruccion_binary(Instruction *my_instruccion) {
    printf("Instruccion = \n");
    for (size_t i = 0; i < sizeof(Instruction); i++) {
        printf("\t\t\t\t");
        print_binary(((uint8_t*)(my_instruccion))[i], 8, 0);
        printf(" ");
    }printf("\n");
}

void print_instruccion(Instruction *my_instruccion) {
    printf_color("prefix:       #{FG:lgreen}%02X %02X %02X %02X#{FG:reset} = \n", my_instruccion->prefix[0], my_instruccion->prefix[1], my_instruccion->prefix[2], my_instruccion->prefix[3]);
    for (int i = 0; i < sizeof(uint8_t) *4; i++) {
        printf("\t\t\t\t");
        print_binary(my_instruccion->prefix[i], 8, 0);

    } printf("\n");

    print_opcode(my_instruccion->opcode[0], 0);
    print_opcode(my_instruccion->opcode[1], 1);
    print_opcode(my_instruccion->opcode[2], 2);


    printf_color("Mod_rm:       #{FG:lgreen}%02X#{FG:reset} = ", *((uint8_t*)&(my_instruccion->Mod_rm))); print_binary( *((uint8_t*)&(my_instruccion->Mod_rm)), 8, 0);
    printf_color("mod:          #{FG:lgreen}%02X#{FG:reset} = ", my_instruccion->Mod_rm.mod); print_binary(my_instruccion->Mod_rm.mod, 2, 0); 
    printf_color("reg:          #{FG:lgreen}%02X#{FG:reset} = --", my_instruccion->Mod_rm.reg); print_binary(my_instruccion->Mod_rm.reg, 3, 2);
    printf_color("R_M:          #{FG:lgreen}%02X#{FG:reset} = ", my_instruccion->Mod_rm.R_M); print_binary(my_instruccion->Mod_rm.R_M, 3, 5);

    /*
     * "mod" con valor 00 junto al campo "r/m" 100 indica que se trata del modo SIB
     * Mod_rm:       AA = 00xx x100 (0x04)
     *  - mod:       02 = 00
     *  - reg:       05 =   xxx     (eliminar campo REG usando AND 0b11000111)
     *  - R_M:       02 =       100
     */
    //if ((*((uint8_t*)&(my_instruccion->Mod_rm)) & 0b11000111) == 0b00000100){ // modo SIB
        printf_color("\nSIB:          #{FG:lgreen}%02X#{FG:reset} = ", *((uint8_t*)&(my_instruccion->SIB)));
        print_binary( *((uint8_t*)&(my_instruccion->SIB)), 8, 0);
        printf_color("scale:        #{FG:lgreen}%02X#{FG:reset} = ", my_instruccion->SIB.scale); print_binary(my_instruccion->SIB.scale, 2, 0); 
        printf_color("index:        #{FG:lgreen}%02X#{FG:reset} = --", my_instruccion->SIB.index); print_binary(my_instruccion->SIB.index, 3, 2);
        printf_color("base:         #{FG:lgreen}%02X#{FG:reset} = ",  my_instruccion->SIB.base); print_binary(my_instruccion->SIB.base, 3, 5);
    
        printf_color("\ndisplacement: #{FG:lgreen}%02X %02X %02X %02X#{FG:reset} = \n", my_instruccion->displacement[0], my_instruccion->displacement[1], my_instruccion->displacement[2], my_instruccion->displacement[3]);
        for (int i = 0; i < sizeof(uint8_t) * 4; i++) {
            printf("\t\t\t\t");
            print_binary(my_instruccion->displacement[i], 8, 0);
        } printf("\n");

        printf_color("immediate:    #{FG:lgreen}%02X %02X %02X %02X#{FG:reset} = \n", my_instruccion->immediate[0], my_instruccion->immediate[1], my_instruccion->immediate[2], my_instruccion->immediate[3]);
        for (int i = 0; i < sizeof(uint8_t) * 4; i++) {
            printf("\t\t\t\t");
            print_binary(my_instruccion->immediate[i], 8, 0);
        } printf("\n");
    //}
}

#endif