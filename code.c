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

size_t get_size_func(unsigned char* func_ptr) {
    for (size_t n_bytes_func = 0; func_ptr != NULL; func_ptr++)
    {
        if ( 
            (
                *(func_ptr + 0) == (unsigned char)0xc3 &&
                *(func_ptr + 1) == (unsigned char)0xc3 &&
                *(func_ptr + 2) == (unsigned char)0xc3 &&
                *(func_ptr + 3) == (unsigned char)0xc3 &&
                *(func_ptr + 4) == (unsigned char)0xc3 &&
                *(func_ptr + 5) == (unsigned char)0xc3 &&
                *(func_ptr + 6) == (unsigned char)0xc3 
            ) || n_bytes_func > 0x75D410 // poner un tamaño limite
        ) return n_bytes_func;
        else {
            n_bytes_func++;
        }
    }
    return 0;
}   

int main(){
    char text[] = " ";
    printf("direccion de main: 0x%p\n", (char*)main);
    /*printf("Tamano de main: %zu bytes\n", get_size_func((char*)main));
    print_table_hex(text, (char*)main, get_size_func((char*)main), ENCODER_IN_32bits);
    List_instrution *instrutions_struct = format_instruccion((char*)main, get_size_func((char*)main), ENCODER_IN_32bits);
    String_list_link *string_asm = get_string_instruction_assembly(instrutions_struct, ENCODER_IN_32bits);
    String_list_link *string_asm_join = join_list_to_String(string_asm, "\n");
    free_String_list_link(string_asm);
    print_String_list_link(string_asm_join);
    free_String_list_link(string_asm_join);
    //instrutions_struct = pop_List_instrution(instrutions_struct,2);
    print_List_instrution(instrutions_struct, ENCODER_IN_32bits);
    //size_t number_of_instrutions = get_number_instrutions(instrucciones, sizeof(instrucciones));
    */
    /**/
    uint8_t instrucciones1[] = {
        0x00, 0xc0,                                   // add al, al
        0x01, 0xd6,                                   // add si, dx
        0x00, 0xd6,                                   // add dh, dl
        0x01, 0xfe,                                   // add si, di
        0x02, 0x31,                                   // add dh, byte ptr [bx + di]
        0x02, 0x30,                                   // add dh, byte ptr [bx + si]
        0x03, 0x31,                                   // add si, word ptr [bx + di]
        0x03, 0x05,                                   // add ax, word ptr [di]
        0x03, 0x3f,                                   // add di, word ptr [bx]
        0x03, 0xb6, 0xca, 0x00,                       // add si, word ptr [bp + 0xca]
        0x03, 0xbc, 0xff, 0x00,                       // add di, word ptr [si + 0xff]
        0x83, 0xc4, 0x11,                             // add sp, 0x11
        0x81, 0x84, 0xbb, 0x11, 0x33, 0x22,           // add word ptr [si + 0x11bb], 0x2233
        0x81, 0xc4, 0x22, 0x11,                       // add sp, 0x1122
        0x83, 0xc0, 0x10,                             // add ax, 0x10
        0x04, 0x10,                                   // add al, 0x10
        Prefix_operand_size,
        Prefix_CS,
        Prefix_repe_rep_repz,
        0x37,                                          // AAA                                          = 0011 0111
        0xd5, 0x0a,                                    // AAD                                          = 1101 0101  0000 1010
        0xd4, 0x0a,                                    // AAM                                          = 1101 0100  0000 1010
        0x3f,                                          // AAS                                          = 0011 1111
        // ADC instrucciones
        // instrucciones con mod = 11
        0x10, 0xc0,                                    // ADC al,                  al                  = 0001 0000  |11| 000 000
        0x11, 0b11010110,                              // ADC si,                  dx                  = 0001 0001  |11| 010 110
        0x10, 0b11010110,                              // ADC dh,                  dl                  = 0001 0000  |11| 010 110
        0x13, 0b11110111,                              // ADC si,                  di                  = 0011 0011  |11| 110 111
        // instrucciones con mod = 00 
            // En 16bits no hay instrucciones SIB
        0x12, 0b00110001,                              // ADC dh,                  [bx+di]             = 0001 0010  |00| 110 001 (0x31)
        0x12, 0x30,                                    // adc dh ,                 [bx+si]             = 0001 0010  |00| 110 000
        0x13, 0x31,                                    // adc si ,                 [bx+di]             = 0001 0010  |00| 110 001
        0x12, 0x09,                                    // adc cl ,                 [bx+di]             = 0001 0010  |00| 001 101
        0x13, 0x2e, 0xcd, 0xab,                        // adc bp ,                 [0xabcd]            = 0011 0011  |00| 101 110
        0x11, 0x2e, 0xcd, 0xab,                        // adc [0xabcd],            bp                  = 0001 0001  |00| 101 110
        0x13, 0x05,                                    // adc ax ,                 [di]                = 0011 0011  |00| 000 101
        0x13, 0x3f,                                    // adc di ,                 [bx]                = 0011 0011  |00| 111 111
        // instrucciones con mod = 01 
            // En 16bits no hay instrucciones SIB
        0x12, 0x60, 0xff,                              // adc ah,                  [bx + si - 1]    o tambien representado como
                                                       // adc ah,                  [bx + si + 0xff]    = 0001 0010  |01| 100 000  1111 1111
        0x12, 0x6e, 0x9f,                              // adc ch,                  [bp - 0x61]      o tambien representado como
                                                       // adc ch,                  [bp + 0x9f]         = 0001 0010  |01| 101 110  1001 1111
        // instrucciones con mod = 10 
            // En 16bits no hay instrucciones SIB
        0x12, 0xA0, 0xFF, 0xFF,                        // adc ah,                  [bx + si - 1]    o tambien representado como
                                                       // adc ah,                  [bx + si + 0xffff]  = 0001 0010  |10| 100 000  1111 1111 1111 1111
        0x12, 0xa0, 0xab, 0x00,                        // adc ah,                  [bx + si + 0x00ab]  = 0001 0010  |10| 100 000
        0x13, 0xb6, 0xca, 0x00,                        // adc si,                  [bp + 0x00ca]       = 0011 0011  |10| 110 110
        0x13, 0xbc, 0xff, 0x00,                        // adc di,                  [si + 0x00ff]       = 0011 0011  |10| 111 100 
        0x12, 0x87, 0xda, 0x00,                        // adc al,                  [bx + 0x00da]       = 0001 0010  |10| 000 111

        // ADC instrucciones con datos inmediatos
        0x83, 0x94, 0xBB, 0x11, 0x22,                  // adc [si + 0x11bb],       0x22                = 1000 0011 |10| 010 100
        0x83, 0xD4, 0x11,                              // adc sp,                  0x11                = 1000 0011 |11| 010 100
        0x81, 0x94, 0xbb, 0x11, 0x33, 0x22,            // adc word ptr [si + 0x11bb], 0x2233
        0x81, 0xd4, 0x22, 0x11,                        // adc sp , 0x1122
        0x83, 0x56, 0x12, 0x11,                        // adc word ptr [bp+0x12], 0x11 = 1000 0011 |01| 010 110
        0x15, 0x10, 0x00,                               // adc ax, 0x0010
        0x14, 0x10                                     // adc al, 0x10


    };
    char text1[] = " ";
    print_table_hex(text1, (char*)instrucciones1, sizeof(instrucciones1), ENCODER_IN_16bits);
    List_instrution *instrutions_struct = format_instruccion(instrucciones1, sizeof(instrucciones1) -1, ENCODER_IN_16bits);
    String_list_link *string_asm = get_string_instruction_assembly(instrutions_struct, ENCODER_IN_16bits);
    String_list_link *string_asm_join = join_list_to_String(string_asm, "\n");
    free_String_list_link(string_asm);
    print_String_list_link(string_asm_join);
    free_String_list_link(string_asm_join);
    //instrutions_struct1 = pop_List_instrution(instrutions_struct,2);
    print_List_instrution(instrutions_struct, ENCODER_IN_16bits);
    //size_t number_of_instrutions = get_number_instrutions(instrucciones1, sizeof(instrucciones1));
    
   uint8_t instrucciones[] = {
        0x00, 0xC0,                                                          // add                                   al,             al
        0x01, 0xD6,                                                          // add                                  esi,             edx
        0x00, 0xD6,                                                          // add                                   dh,             dl
        0x01, 0xFE,                                                          // add                                  esi,             edi
        0x12, 0x30,                                                          // adc                                   dh, byte ptr   [eax]
        0x02, 0x30,                                                          // add                                   dh, byte ptr   [eax]
        0x03, 0x31,                                                          // add                                  esi, dword ptr  [ecx]
        0x02, 0x09,                                                          // add                                   cl, byte ptr   [ecx]
        0x03, 0x2E,                                                          // add                                  ebp, dword ptr  [esi]
        0x01, 0x2E,                                                          // add                      dword ptr [esi],             ebp
        0x03, 0x05, 0xAA, 0xBB, 0xCC, 0xDD,                                  // add                                  eax, dword ptr  [0xddccbbaa]
        0x03, 0x3F,                                                          // add                                  edi, dword ptr  [edi]
        0x02, 0xA0, 0xFF, 0x00, 0x00, 0x00,                                  // add                                   ah, byte ptr   [eax + 0xff]
        0x02, 0x60, 0xFF,                                                    // add                                   ah, byte ptr   [eax - 1]
        0x00, 0xAC, 0xBB, 0x78, 0x56, 0x34, 0x12,                            // add  byte ptr [ebx + edi*4 + 0x12345678],             ch
        0x03, 0xB6, 0xFF, 0x00, 0xAA, 0xBB,                                  // add                                  esi, dword ptr  [esi - 0x4455ff01]  o dword ptr  [esi + BBAA 00FF] 
        0x80, 0x84, 0xBB, 0x11, 0x22, 0x33, 0x44, 0x55,                      // add  byte ptr [ebx + edi*4 + 0x44332211],             0x55
        0x81, 0x84, 0xBB, 0x11, 0x22, 0x33, 0x44, 0x11, 0x55, 0x00, 0x22,    // add dword ptr [ebx + edi*4 + 0x44332211],             0x22005511
        0x83, 0xC4, 0x11,                                                    // add                                  esp,             0x11
        0x80, 0xC7, 0x11,                                                    // add                                   bh,             0x11
        0x81, 0xC4, 0x11, 0x11, 0x22, 0x22,                                  // add                                  esp,             0x22221111
        0x83, 0xC5, 0x11,                                                    // add                                  ebp,             0x11
        0x81, 0x40, 0x0C, 0x44, 0x33, 0x22, 0x11,                            // add                dword ptr [eax + 0xc],             0x11223344
        0x80, 0x40, 0x0C, 0x22,                                              // add                 byte ptr [eax + 0xc],             0x22
        0x83, 0xC0, 0x10,                                                    // add                                  eax,             0x10
        0x04, 0x10,                                                          // add                                   al,             0x10
        0x01, 0xd8,                                                          // add                                  eax,             ebx      = 0000 0001 |11| 011 000
        Prefix_operand_size,
        Prefix_CS,
        Prefix_repe_rep_repz,
        0x37,                                                                  // AAA                                                          = 0011 0111
        0xd5, 0x0a,                                                            // AAD                                                          = 1101 0101  0000 1010
        0xd4, 0x0a,                                                            // AAM                                                          = 1101 0100  0000 1010
        0x3f,                                                                  // AAS                                                          = 0011 1111
        // ADC instrucciones                                      
        // instrucciones con mod = 11                                      
        0x10, 0xc0,                                                            // ADC al,                                  al                  = 0001 0000  |11| 000 000
        0x11, 0b11010110,                                                      // ADC esi,                                 edx                 = 0001 0001  |11| 010 110
        0x10, 0b11010110,                                                      // ADC dh,                                  dl                  = 0001 0000  |11| 010 110
        0x13, 0b11110111,                                                      // ADC esi,                                 edi                 = 0011 0011  |11| 110 111
        // instrucciones con mod = 00                                       
            // instruccion mod == 00 con SIB(R/M = 100)                        (Solo para 32bits, no 16bits)                                                                    Scale * Index + Base 
            0x10, 0x2c, 0xbb,                                                  // adc [ebx + edi*4],                       ch                  = 0001 0000  |00| 101 100 (0x31)   10     111     011 (0xbb)     
        0x10, 0b00110001,                                                      // ADC dh,                                  [ecx]               = 0001 0010  |00| 110 001 (0x31)
        0x12, 0x30,                                                            // adc dh,                                  [eax]               = 0001 0010  |00| 110 000 (0x30)
        0x13, 0x31,                                                            // adc esi,                                 [ecx]               = 0001 0011  |00| 110 001 (0x31)
        0x12, 0x09,                                                            // adc cl,                                  [ecx]               = 0001 0010  |00| 001 001 (0x09)
        0x13, 0x2e,                                                            // adc ebp,                                 [esi]               = 0001 0011  |00| 101 110 (0x2e)
        0x11, 0x2e,                                                            // adc [esi],                               ebp                 = 0001 0001  |00| 101 110 (0x2e)
        0x13, 0x05, 0xaa, 0xbb, 0xcc, 0xdd,                                    // adc eax,                                 [0xddccbbaa]        = 0001 0011  |00| 000 101 (0x05)
        0x13, 0x3f,                                                            // adc edi,                                 [edi]               = 0001 0011  |00| 111 111 (0x3f)
        // instrucciones con mod = 01                                 
        0x12, 0x60, 0xff,                                                      // adc ah,                                  [eax - 1]    o tambien representado como
                                                                               // adc ah,                                  [eax + 0xff]        = 0001 0010  |01| 100 000  1111 1111
            // instruccion mod == 01 con SIB(R/M = 100)                        (Solo para 32bits, no 16bits)         
            0x10, 0x6c, 0xbb, 0x12,                                            // adc [ebx + edi*4 + 0x12],                ch                  = 0001 0000  |01| 101 100 (0x31) 10 111 011(0xbb)     
        0x12, 0x6e, 0x9f,                                                      // adc ch,                                  [esi - 0x61] o tambien representado como
                                                                               // adc ch,                                  [esi + 0x9f]        = 0001 0010  |01| 101 110  1001 1111
        // instrucciones con mod = 10                                  
        0x12, 0xA0, 0xFF, 0xFF, 0xFF, 0xFF,                                    // adc ah,                                   [eax - 1]    o tambien representado como
                                                                               // adc ah,                                   [eax + 0xffffffff]  = 0001 0010  |10| 100 000  1111 1111 1111 1111 1111 1111 1111 1111
            // instruccion mod == 10 con SIB(R/M = 100) (Solo para 32bits, no 16bits)         
            0x10, 0xac, 0xbb, 0x78, 0x56, 0x34, 0x12,                          // adc [ebx + edi*4 + 0x12345678],           ch                  = 0001 0000  |10| 101 100 (0x31) 10 111 011(0xbb)  
        0x13, 0xb6, 0xFF, 0x00, 0xaa, 0xbb,                                    // adc esi,                                  [esi - 0x4455ff01]    o tambien representado como
                                                                               // adc esi,                                  [esi + 0xbbaa00ff]  = 0001 0010  |10| 100 000  1111 1111 0000 0000 1010 1010 1011 1011 
        // ADC instrucciones con datos inmediatos
            // instruccion mod == 10 con SIB(R/M = 100) (Solo para 32bits, no 16bits)
            0x80, 0x94, 0xBB, 0x11, 0x22, 0x33, 0x44, 0x55,                    // adc [ebx + edi*4 + 0x44332211],            0x55               = 1000 0011  |10| 010 100  10 111 011  0001 0001 0010 0010 0011 0011 0100 0100 0101 0101
            // 80 94 BB 11 22 33 44 55 es lo mismo que 82 94 BB 11 22 33 44 11    pues al no poder moverse el valor [ebx + edi*4 + 0x44332211], a 0x55 la cpu crea este formato

            0x81, 0x94, 0xbb, 0x11, 0x22, 0x33, 0x44, 0x11, 0x55, 0x00, 0x22,  // adc [ebx + edi*4 + 0x44332211],            0x22005511         = 1000 0010  |10| 010 100  10 111 011  0001 0001 0010 0010 0011 0011 0100 0100 0001 0001 0101 0101 0000 0000 0010 0010
        
        // si el bit "w" esta junto al bit "s" activados, se usa extension de signo
        0x83, 0xD4, 0x11,                                                      // adc esp,                                   0x11               = 1000 0011  |11| 010 100  0001 0001
        // si el bit "w" esta desactivado y el "s" activado, el valor a almacenar es de 8bits
        0x82, 0xD7, 0x11,                                                      // adc bh,                                    0x11               = 1000 0010  |11| 010 100  0001 0001
        // Caso w = 0 y s = 1
        // El caso w = 0 y s = 1 no es una combinación válida en el conjunto de instrucciones del 80386. El campo s solo tiene significado cuando w = 1, ya que la extensión de signo se aplica para ajustar un operando inmediato de 8 bits a un tamaño mayor (16 o 32 bits).
        // Combinaciones Válidas
        // Las combinaciones válidas y sus significados son:

        // si el bit "w" esta activado  y el "s" desactivado, el valor a almacenar es de 32bits
        0x81, 0xD4, 0x11, 0x11, 0x22, 0x22,                                    // adc esp,                                   0x22221111         = 
        0x83, 0xd5, 0x11,                                                      // adc ebp,                                    0x11               
        0x81, 0x50, 0x0c, 0x44, 0x33, 0x22, 0x11,                              // adc dword ptr [eax + 0xc], 0x11223344                         = 1000 0001 |01| 010 000
        // necesita el prefijo 66
        //0x66, 0x81, 0x50, 0x0c, 0x22, 0x11,                                          // adc  word ptr [eax + 0xc], 0x1122                             = 1000 0001 |01| 010 000 
        0x80, 0x50, 0x0c, 0x22,                                                // adc  byte ptr [eax + 0xc], 0x22                               = 1000 0000 |01| 010 000
        0x15, 0x10, 0x00, 0x00, 0x00,                                          // adc eax, 0x00000010
        0x14, 0x10,                                                            // adc al, 0x00

    };
   

    // solo yo y dios sabemos que hace este codigo.
    // tal vez mañana solo dios.

    
    print_table_hex(text, (char*)instrucciones, sizeof(instrucciones), ENCODER_IN_32bits);
    List_instrution *instrutions_struct32bits = format_instruccion(instrucciones, sizeof(instrucciones)-1, ENCODER_IN_32bits);
    String_list_link *string_asm32bits = get_string_instruction_assembly(instrutions_struct32bits, ENCODER_IN_32bits);
    String_list_link *string_asm_join32bits = join_list_to_String(string_asm32bits, "\n");
    free_String_list_link(string_asm32bits);
    print_String_list_link(string_asm_join32bits);
    free_String_list_link(string_asm_join32bits);
    //instrutions_struct32bits = pop_List_instrution(instrutions_struct32bits,2);
    print_List_instrution(instrutions_struct32bits, ENCODER_IN_32bits);
    //size_t number_of_instrutions = get_number_instrutions(instrucciones, sizeof(instrucciones));

    printf("Numero de instrucciones descodificadas: %zu\n", get_number_instrutions(instrutions_struct32bits));

    puts("Exit...");
    
    /*volatile int dummy = 0;
    asm volatile("ret"); // 1
    asm volatile("ret"); // 2
    asm volatile("ret"); // 3
    asm volatile("ret"); // 4
    asm volatile("ret"); // 5
    asm volatile("ret"); // 6
    asm volatile("ret"); // 7
    asm volatile("ret"); // 8
    */
    return 0;
    
}

