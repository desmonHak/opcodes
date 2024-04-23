#include "opcodes_prefix.h"
#include "instrutions_format.h"

#include <stdio.h>

int main(){

    uint8_t instrucciones[] = {
        Prefix_operand_size,
        Prefix_CS,
        Prefix_repe_rep_repz,
        0x37,       // AAA        = 0011 0111
        0xd5, 0x0a, // AAD        = 1101 0101  0000 1010
        0xd4, 0x0a, // AAM        = 1101 0100  0000 1010
        0x3f,       // AAS        = 0011 1111
        0x10, 0xc0  // ADC al, al = 0001 0000  11 000 000
    };

    List_instrution *instrutions_struct = format_instruccion(instrucciones, sizeof(instrucciones));
    //size_t number_of_instrutions = get_number_instrutions(instrucciones, sizeof(instrucciones));

    puts("Exit...");
    return 0;
}