/*
 *	Licencia Apache, Version 2.0 con Modificacion
 *	
 *	Copyright 2023 Desmon (David)
 *	
 *	Se concede permiso, de forma gratuita, a cualquier persona que obtenga una copia de 
 *	este software y archivos de documentacion asociados (el "Software"), para tratar el 
 *	Software sin restricciones, incluidos, entre otros, los derechos de uso, copia, 
 *	modificacion, fusion, publicacion, distribucion, sublicencia y/o venta de copias del 
 *	Software, y para permitir a las personas a quienes se les proporcione el Software 
 *	hacer lo mismo, sujeto a las siguientes condiciones:
 *	
 *	El anterior aviso de copyright y este aviso de permiso se incluiran en todas las 
 *	copias o partes sustanciales del Software.
 *	
 *	EL SOFTWARE SE PROPORCIONA "TAL CUAL", SIN GARANTiA DE NINGÚN TIPO, EXPRESA O 
 *	IMPLiCITA, INCLUYENDO PERO NO LIMITADO A LAS GARANTiAS DE COMERCIABILIDAD, IDONEIDAD 
 *	PARA UN PROPoSITO PARTICULAR Y NO INFRACCIoN. EN NINGÚN CASO LOS TITULARES DEL 
 *	COPYRIGHT O LOS TITULARES DE LOS DERECHOS DE AUTOR SERaN RESPONSABLES DE NINGÚN 
 *	RECLAMO, DAnO U OTRA RESPONSABILIDAD, YA SEA EN UNA ACCIoN DE CONTRATO, AGRAVIO O DE 
 *	OTRA MANERA, QUE SURJA DE, FUERA DE O EN CONEXIoN CON EL SOFTWARE O EL USO U OTRO TIPO
 *	DE ACCIONES EN EL SOFTWARE.
 *	
 *	Ademas, cualquier modificacion realizada por terceros se considerara propiedad del 
 *	titular original de los derechos de autor. Los titulares de derechos de autor 
 *	originales no se responsabilizan de las modificaciones realizadas por terceros.
 *	
 *	Queda explicitamente establecido que no es obligatorio especificar ni notificar los 
 *	cambios realizados entre versiones, ni revelar porciones especificas de codigo 
 *	modificado.
 */
//#define DEBUG_ENABLE // debug activado
#include "opcodes_prefix.h"
#include "instrutions_format.h"

#include <stdio.h>

int main(){
    /**/
    uint8_t instrucciones[] = {
        /**/Prefix_operand_size,
        Prefix_CS,
        Prefix_repe_rep_repz,
        0x37,                   // AAA                               = 0011 0111
        0xd5, 0x0a,             // AAD                               = 1101 0101  0000 1010
        0xd4, 0x0a,             // AAM                               = 1101 0100  0000 1010
        0x3f,                   // AAS                               = 0011 1111
        // instrucciones con mod = 11
        0x10, 0xc0,             // ADC al,       al                  = 0001 0000  |11| 000 000
        0x11, 0b11010110,       // ADC si,       dx                  = 0001 0001  |11| 010 110
        0x10, 0b11010110,       // ADC dh,       dl                  = 0001 0000  |11| 010 110
        0x13, 0b11110111,       // ADC si,       di                  = 0011 0011  |11| 110 111
        // instrucciones con mod = 00 
        0x12, 0b00110001,       // ADC dh,       [bx+di]             = 0001 0010  |00| 110 001 (0x31)
        0x12, 0x30,             // adc dh ,      [bx+si]             = 0001 0010  |00| 110 000
        0x13, 0x31,             // adc si ,      [bx+di]             = 0001 0010  |00| 110 001
        0x12, 0x09,             // adc cl ,      [bx+di]             = 0001 0010  |00| 001 101
        0x13, 0x2e, 0xcd, 0xab, // adc bp ,      [0xabcd]            = 0011 0011  |00| 101 110
        0x11, 0x2e, 0xcd, 0xab, // adc [0xabcd], bp                  = 0001 0001  |00| 101 110
        0x13, 0x05,             // adc ax ,      [di]                = 0011 0011  |00| 000 101
        0x13, 0x3f,             // adc di ,      [bx]                = 0011 0011  |00| 111 111
        // instrucciones con mod = 01
        0x12, 0x60, 0xff,       // adc ah,       [bx + si - 1]    o tambien representado como
                                // adc ah,       [bx + si + 0xff]    = 0001 0010  |01| 100 000  1111 1111
        0x12, 0x6e, 0x9f,       // adc ch,       [bp - 0x61]      o tambien representado como
                                // adc ch,       [bp + 0x9f]         = 0001 0010  |01| 101 110  1001 1111
        // instrucciones con mod = 10
        0x12, 0xA0, 0xFF, 0xFF, // adc ah,       [bx + si - 1]    o tambien representado como
                                // adc ah,       [bx + si + 0xffff]  = 0001 0010  |10| 100 000  1111 1111 1111 1111
        0x12, 0xa0, 0xab, 0x00, // adc ah,       [bx + si + 0x00ab]  = 0001 0010  |10| 100 000
        0x13, 0xb6, 0xca, 0x00, // adc si,       [bp + 0x00ca]       = 0011 0011  |10| 110 110
        0x13, 0xbc, 0xff, 0x00, // adc di,       [si + 0x00ff]       = 0011 0011  |10| 111 100 
        0x12, 0x87, 0xda, 0x00, // adc al,       [bx + 0x00da]       = 0001 0010  |10| 000 111

    };
    char text[] = " ";
    print_table_hex(text, (char*)instrucciones, sizeof(instrucciones), ENCODER_IN_16bits);
    List_instrution *instrutions_struct = format_instruccion(instrucciones, sizeof(instrucciones), ENCODER_IN_16bits);
    String_list_link *string_asm = get_string_instruction_assembly(instrutions_struct, ENCODER_IN_16bits);
    String_list_link *string_asm_join = join_list_to_String(string_asm, "\n");
    free_String_list_link(string_asm);
    print_String_list_link(string_asm_join);
    free_String_list_link(string_asm_join);
    //instrutions_struct = pop_List_instrution(instrutions_struct,2);
    print_List_instrution(instrutions_struct, ENCODER_IN_16bits);
    //size_t number_of_instrutions = get_number_instrutions(instrucciones, sizeof(instrucciones));
    
   /*uint8_t instrucciones[] = {
        Prefix_operand_size,
        Prefix_CS,
        Prefix_repe_rep_repz,
        0x37,                               // AAA               = 0011 0111
        0xd5, 0x0a,                         // AAD               = 1101 0101  0000 1010
        0xd4, 0x0a,                         // AAM               = 1101 0100  0000 1010
        0x3f,                               // AAS               = 0011 1111
        0x10, 0xc0,                         // ADC al,    al     = 0001 0000  11 000 000
        0x11, 0b11010110,                   // ADC esi,   edx    = 0001 0001  11 010 110
        0x10, 0b11010110,                   // ADC dh,    dl     = 0001 0000  11 010 110
        0x13, 0b11110111,                   // ADC esi,   edi    = 0011 0011  11 110 111
        0x12, 0b00110001,                   // ADC dh,    [ecx]  = 0001 0010  00 110 001 (0x31)
        0x12, 0x30,                         // adc dh,    [eax]
        0x13, 0x31,                         // adc esi,   [ecx]
        0x12, 0x09,                         // adc cl,    [ecx]
        0x13, 0x2e,                         // adc ebp,   [esi]
        0x11, 0x2e,                         // adc [esi], ebp
        0x13, 0x05, 0xaa, 0xbb, 0xcc, 0xdd, // adc eax, dword ptr [0xddccbbaa] 
        0x13, 0x3f,                         // adc edi, dword ptr [edi]
    };
    char text[] = " ";
    print_table_hex(text, (char*)instrucciones, sizeof(instrucciones), ENCODER_IN_32bits);
    List_instrution *instrutions_struct = format_instruccion(instrucciones, sizeof(instrucciones), ENCODER_IN_32bits);
    instrutions_struct = pop_List_instrution(instrutions_struct,2);
    print_List_instrution(instrutions_struct, ENCODER_IN_32bits);
    //size_t number_of_instrutions = get_number_instrutions(instrucciones, sizeof(instrucciones));*/
 

    printf("Numero de instrucciones descodificadas: %zu\n", get_number_instrutions(instrutions_struct));

    puts("Exit...");
    return 0;
}