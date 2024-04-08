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

    /*
     * "mod" con valor 00 junto al campo "r/m" 100 indica que se trata del modo SIB
     * Mod_rm:       AA = 00xx x100 (0x04)
     *  - mod:       02 = 00
     *  - reg:       05 =   xxx     (eliminar campo REG usando AND 0b11000111)
     *  - R_M:       02 =       100
     */
    if ((*((uint8_t*)&(my_instruccion->Mod_rm)) & 0b11000111) == 0b00000100){ // modo SIB
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
}



char* get_SIB_32_16bits(Instruction *my_instruccion){
    char* data = NULL;
    if (my_instruccion->Mod_rm.mod == 0b00 && my_instruccion->Mod_rm.R_M == 0b100){ // solo si se da esta condicion se usa sib
        switch (my_instruccion->SIB.index)
        { //  example instruccion jmp [eax*2 + ebx]
            case 0b000: printf(" [ eax ");    break;
            case 0b001: printf(" [ ecx ");    break;
            case 0b010: printf(" [ edx ");    break;
            case 0b011: printf(" [ ebx ");    break;
            case 0b100: printf(" [ ilegal "); break;
            case 0b101: printf(" [ ebp ");    break;
            case 0b110: printf(" [ esi ");    break;
            case 0b111: printf(" [ edi ");    break;
            default:    printf(" [ ilegal "); break;
        }
        switch (my_instruccion->SIB.scale)
        { 
            case 0b00: printf(" * 1 + ");    break;
            case 0b01: printf(" * 2 + ");    break;
            case 0b10: printf(" * 4 + ");    break;
            case 0b11: printf(" * 8 + ");    break;
        }
        switch (my_instruccion->SIB.base)
        {
            case 0b000:printf(" eax ]");    break;
            case 0b001:printf(" ecx ]");    break;
            case 0b010:printf(" edx ]");    break;
            case 0b011:printf(" ebx ]");    break;
            case 0b100:printf(" ilegal ]"); break;
            case 0b101:printf(" ebp ]");    break;
            case 0b110:printf(" esi ]");    break;
            case 0b111:printf(" edi ]");    break;
            default:printf(" ilegal ]");    break;
        }
        puts(" ");
    }
    return data;
}

char *get_modR_M(Instruction *my_instruccion){
    char* data = NULL;

    switch (my_instruccion->Mod_rm.mod)
    {
    case 0b00:
        switch(my_instruccion->Mod_rm.R_M){
            /*case 0b000: printf((my_instruccion.modo_direccionamiento == 32) ? "[eax]"    : "[bx+si]")  break;
            case 0b001: printf((my_instruccion.modo_direccionamiento == 32) ? "[ecx]"    : "[bx+di]")  break;
            case 0b010: printf((my_instruccion.modo_direccionamiento == 32) ? "[edx]"    : "[bp+si]")  break;
            case 0b011: printf((my_instruccion.modo_direccionamiento == 32) ? "[ebx]"    : "[bp+di]")  break;
            case 0b100:(my_instruccion.modo_direccionamiento == 32) ? return get_SIB_32_16bits(my_instruccion) : printf("[si]"); break;
            case 0b101: printf((my_instruccion.modo_direccionamiento == 32) ? "disp32" : "[di]")     break;
            case 0b110: printf((my_instruccion.modo_direccionamiento == 32) ? "[esi]"    : "[disp16]") break;
            case 0b111: printf((my_instruccion.modo_direccionamiento == 32) ? "[edi]"    : "[bx]")     break;*/
            case 0b000: printf("eax");    break;
            case 0b001: printf("ecx");    break;
            case 0b010: printf("edx");    break;
            case 0b011: printf("ebx");    break;
            case 0b100: return            get_SIB_32_16bits(my_instruccion);
            case 0b101: printf("disp32"); break;
            case 0b110: printf("esi");    break;
            case 0b111: printf("edi");    break;
        } break;
    case 0b01:
        switch(my_instruccion->Mod_rm.R_M){
            /*case 0b000: printf((my_instruccion.modo_direccionamiento == 32) ? "[eax] + disp8"  : "[bx+si] + disp8")  break;
            case 0b001: printf((my_instruccion.modo_direccionamiento == 32) ? "[ecx] + disp8"    : "[bx+di] + disp8")  break;
            case 0b010: printf((my_instruccion.modo_direccionamiento == 32) ? "[edx] + disp8"    : "[bp+si] + disp8")  break;
            case 0b011: printf((my_instruccion.modo_direccionamiento == 32) ? "[ebx] + disp8"    : "[bp+di] + disp8")  break;
            case 0b100:(my_instruccion.modo_direccionamiento == 32) ? return get_SIB_32_16bits(my_instruccion) : printf("[si]  + disp8"); break;
            case 0b101: printf((my_instruccion.modo_direccionamiento == 32) ? "[ebp] + disp8" : "[di] + disp8")     break;
            case 0b110: printf((my_instruccion.modo_direccionamiento == 32) ? "[esi] + disp8"    : "[bp] + disp8") break;
            case 0b111: printf((my_instruccion.modo_direccionamiento == 32) ? "[edi] + disp8"    : "[bx] + disp8")     break;*/
            case 0b000: printf("[eax] + disp8"); break;
            case 0b001: printf("[ecx] + disp8"); break;
            case 0b010: printf("[edx] + disp8"); break;
            case 0b011: printf("[ebx] + disp8"); break;
            case 0b100: return            get_SIB_32_16bits(my_instruccion); // + disp8
            case 0b101: printf("[ebp] + disp8");  break;
            case 0b110: printf("[esi] + disp8");  break;
            case 0b111: printf("[edi] + disp8");  break;
        } break;
    case 0b10:
        switch(my_instruccion->Mod_rm.R_M){
            /*case 0b000: printf((my_instruccion.modo_direccionamiento == 32) ? "eax + disp16"    : "[bx+si] + disp16")  break;
            case 0b001: printf((my_instruccion.modo_direccionamiento == 32) ? "ecx + disp16"    : "[bx+di] + disp16")  break;
            case 0b010: printf((my_instruccion.modo_direccionamiento == 32) ? "edx + disp16"    : "[bp+si] + disp16")  break;
            case 0b011: printf((my_instruccion.modo_direccionamiento == 32) ? "ebx + disp16"    : "[bp+di] + disp16")  break;
            case 0b100:(my_instruccion.modo_direccionamiento == 32) ? return get_SIB_32_16bits(my_instruccion) : printf("[si]"); break;
            case 0b101: printf((my_instruccion.modo_direccionamiento == 32) ? "[ebp] + disp16" : "[di] + disp16")     break;
            case 0b110: printf((my_instruccion.modo_direccionamiento == 32) ? "esi + disp16"    : "[bp] + disp16") break;
            case 0b111: printf((my_instruccion.modo_direccionamiento == 32) ? "edi + disp16"    : "[bx] + disp16")     break;*/
            case 0b000: printf("[eax] + disp16"); break;
            case 0b001: printf("[ecx] + disp16"); break;
            case 0b010: printf("[edx] + disp16"); break;
            case 0b011: printf("[ebx] + disp16"); break;
            case 0b100: return            get_SIB_32_16bits(my_instruccion);
            case 0b101: printf("[ebp] + disp16"); break;
            case 0b110: printf("[esi] + disp16"); break;
            case 0b111: printf("[edi] + disp16"); break;
        } break;
    case 0b11: // modo de direcionamiento de registro (acabar, se usa ¿d?)
        switch(my_instruccion->Mod_rm.R_M){ 
            /*case 0b000: printf((my_instruccion.modo_direccionamiento == 32) ? "eax"    : "[bx+si]")  break;
            case 0b001: printf((my_instruccion.modo_direccionamiento == 32) ? "ecx"    : "[bx+di]")  break;
            case 0b010: printf((my_instruccion.modo_direccionamiento == 32) ? "edx"    : "[bp+si]")  break;
            case 0b011: printf((my_instruccion.modo_direccionamiento == 32) ? "ebx"    : "[bp+di]")  break;
            case 0b100:(my_instruccion.modo_direccionamiento == 32) ? return get_SIB_32_16bits(my_instruccion) : printf("[si]"); break;
            case 0b101: printf((my_instruccion.modo_direccionamiento == 32) ? "disp32" : "[di]")     break;
            case 0b110: printf((my_instruccion.modo_direccionamiento == 32) ? "esi"    : "[disp16]") break;
            case 0b111: printf((my_instruccion.modo_direccionamiento == 32) ? "edi"    : "[bx]")     break;*/
            case 0b000: printf("eax");    break;
            case 0b001: printf("ecx");    break;
            case 0b010: printf("edx");    break;
            case 0b011: printf("ebx");    break;
            case 0b100: return            get_SIB_32_16bits(my_instruccion);
            case 0b101: printf("disp32"); break;
            case 0b110: printf("esi");    break;
            case 0b111: printf("edi");    break;
        } break;
    }
    return data;
    
}

char* get_asm_opcode(Instruction *my_instruccion){
    char *data = NULL;
    switch (*((uint8_t*)(&my_instruccion->opcode[2]))) // primer byte del opcode
    {
    case INC_DEC_CALL_JMP_PUSH:
        //printf("Se encontro en la posicion %d INC_DEC_CALL_JMP_PUSH\n", sizeof(Opcodes_x86_array_1)); 
        switch (my_instruccion->Mod_rm.reg)
        {
        case 0b000: // INC
            printf("INC"); break;
        case 0b001: // DEC
            printf("DEC"); break;
        case 0b010: // near CALL
            printf("CALL"); break;
        case 0b011: // far CALL
            printf("CALL far"); break;
        case 0b100: // near JMP
            printf("JMP"); break;
        case 0b101: // far JMP
            printf("JMP far"); break;
        case 0b110: // PUSH
            printf("PUSH"); break;
        default:
            puts("opcode invalid");
            goto err_opcode_invalid_get_asm_opcode;
            break;
        }
        get_SIB_32_16bits(my_instruccion);
        break;
    case ADD_ADC_AND_XOR_OR_SBB_SUB_CMP:
        printf("ADD_ADC_AND_XOR_OR_SBB_SUB_CMP\n"); 
        break;
    case TEST_NOT_NEG_MUL_DIV:
        printf("TEST_NOT_NEG_MUL_DIV\n"); 
        break;
    case TWO_BYTE: // si el opcode corresponde a TWO_BYTE entonces se trata de una instruccion de dos bytes de opcode
        break;
    /*case THREE_BYTE: // ?// si el opcode corresponde a THREE_BYTE entonces se trata de una instruccion de tres bytes de opcode
        break;*/
    default: // caso por defecto (instrucciones de un byte de opcode)
        for (uint8_t i = 0; i < sizeof(Opcodes_x86_array_1); i++) // 0xff+1 se uso para indicar el final de los arrays que contiene las instrucciones
        {
            if(Opcodes_x86_array[0][i] <= *((uint8_t*)(&my_instruccion->opcode[2])) && *((uint8_t*)(&my_instruccion->opcode[2])) < Opcodes_x86_array[0][i+1]){
                printf("Se encontro la instruccion %s\n", instructions_asm[(uint8_t)Opcodes_x86_array[0][i]]); break;
            } 
        }
        break;
    }
    err_opcode_invalid_get_asm_opcode:
    return data;
}

#endif