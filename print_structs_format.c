#ifndef __PRINT_STRUCTS_FORMAT_C_
#define __PRINT_STRUCTS_FORMAT_C_
#include "print_structs_format.h"

void print_binary(unsigned int num, int num_bits) {
    for (int i = num_bits - 1; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
        if (i % 4 == 0 && i != 0) printf(" "); 
    } printf("\n");
}



void print_opcode(opcode opcode, uint8_t number_opcode){
    printf("opcode{%d}:    %02X = ", number_opcode, *((uint8_t*)(&opcode)));
    print_binary( *((uint8_t*)(&opcode)), 8);
    printf("opcode[%d]:    %02X = ", number_opcode, opcode.opcode);
    print_binary(opcode.opcode, 6);
    printf("opcode[%d].d:  %02X =        %d\n", number_opcode, opcode.d, opcode.d);
    printf("opcode[%d].s:  %02X =         %d\n\n", number_opcode, opcode.s, opcode.s);
}
void print_instruccion_binary(Instruction *my_instruccion){
    printf("Instruccion = \n");
    for (size_t i = 0; i < sizeof(Instruction); i++) {
        printf("\t\t\t\t");
        print_binary(((uint8_t*)(my_instruccion))[i], 8);
        printf(" ");
    }printf("\n");
}

void print_instruccion(Instruction *my_instruccion){
    printf("prefix:       %02X %02X %02X %02X = \n", my_instruccion->prefix[0], my_instruccion->prefix[1], my_instruccion->prefix[2], my_instruccion->prefix[3]);
    for (int i = 0; i < sizeof(uint8_t) *4; i++) {
        printf("\t\t\t\t");
        print_binary(my_instruccion->prefix[i], 8);

    } printf("\n");

    print_opcode(my_instruccion->opcode[0], 0);
    print_opcode(my_instruccion->opcode[1], 1);
    print_opcode(my_instruccion->opcode[2], 2);


    printf("Mod_rm:       %02X = ", *((uint8_t*)&(my_instruccion->Mod_rm)));
    print_binary( *((uint8_t*)&(my_instruccion->Mod_rm)), 8);
    printf("mod:          %02X = ", my_instruccion->Mod_rm.mod); print_binary(my_instruccion->Mod_rm.mod, 2);
    printf("reg:          %02X =   ", my_instruccion->Mod_rm.reg); print_binary(my_instruccion->Mod_rm.reg, 3);
    printf("R_M:          %02X =       ", my_instruccion->Mod_rm.R_M); print_binary(my_instruccion->Mod_rm.R_M, 3);

    printf("\nSIB:          %02X = ", *((uint8_t*)&(my_instruccion->SIB)));
    print_binary( *((uint8_t*)&(my_instruccion->SIB)), 8);
    printf("scale:        %02X = ", my_instruccion->SIB.scale); print_binary(my_instruccion->SIB.scale, 2);
    printf("index:        %02X =   ", my_instruccion->SIB.index); print_binary(my_instruccion->SIB.index, 3);
    printf("base:         %02X =       ",  my_instruccion->SIB.base); print_binary(my_instruccion->SIB.base, 3);

    printf("\ndisplacement: %02X %02X %02X %02X = \n", my_instruccion->displacement[0], my_instruccion->displacement[1], my_instruccion->displacement[2], my_instruccion->displacement[3]);
    for (int i = 0; i < sizeof(uint8_t) * 4; i++) {
        printf("\t\t\t\t");
        print_binary(my_instruccion->displacement[i], 8);
    } printf("\n");
    printf("immediate:    %02X %02X %02X %02X = \n", my_instruccion->immediate[0], my_instruccion->immediate[1], my_instruccion->immediate[2], my_instruccion->immediate[3]);
    for (int i = 0; i < sizeof(uint8_t) * 4; i++) {
        printf("\t\t\t\t");
        print_binary(my_instruccion->immediate[i], 8);
    } printf("\n");
}

#endif