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
#ifndef __PRINT_STRUCTS_FORMAT_C_
#define __PRINT_STRUCTS_FORMAT_C_

#include "print_structs_format.h"

char *get_addr_to_encoder_x86(uint64_t addr, encoder_x86 encoder_val) {
    /*
     *  
     * get_addr_to_encoder_x86(uint64_t addr, encoder_x86 encoder_val):
     * Esta funcion devuelve un string con la direccion de memoria formateada en 16, 32 o 64bits
     * 
     * Se espera recibir addr el cual es la direccion de memoria a formatear,
     * se espera recibir encoder_val, el cual indica si formatear la direcion a 16, 32 o 64bits.
     *      Posibles valores para encoder_x86 encoder_val:
     *          - ENCODER_IN_16bits = 0 : para 16bits
     *          - ENCODER_IN_32bits = 1 : para 32bits
     *          - ENCODER_IN_64bits = 2 : para 64bits
     * 
     * En caso de que malloc devuelva error, la funcion devuelve NULL
     *  
     */
    size_t size;
    char *buffer_Position_memory;
    switch (encoder_val) {
        case ENCODER_IN_16bits:
            size = (snprintf(NULL, 0, "%04x", (uint16_t)addr) + 1) * sizeof(char);
            buffer_Position_memory = (char *)malloc(size);
            if (buffer_Position_memory == NULL) return NULL;
            sprintf(buffer_Position_memory, "%04x", (uint16_t)addr);
            break;
        case ENCODER_IN_32bits:
            size = (snprintf(NULL, 0, "%08x", (uint32_t)addr) + 1) * sizeof(char);
            buffer_Position_memory = (char *)malloc(size);
            if (buffer_Position_memory == NULL) return NULL;
            sprintf(buffer_Position_memory, "%08x", (uint32_t)addr);
            break;
        case ENCODER_IN_64bits:
            size = (snprintf(NULL, 0, "%016llx", (uint64_t)addr) + 1) * sizeof(char);
            buffer_Position_memory = (char *)malloc(size);
            if (buffer_Position_memory == NULL) return NULL;
            sprintf(buffer_Position_memory, "%016llx", (uint64_t)addr);
            break;
        default: return NULL; // error
    }
    return buffer_Position_memory;
}

void print_table_hex(char *string_init, char *string_text_for_printing, size_t size_string_text_for_printing, encoder_x86 encoder_val) {
    /*
     *  
     * print_table_hex(char *string_init, char *string_text_for_printing, size_t size_string_text_for_printing, encoder_x86 encoder_val):
     * Esta funcion imprime un "string_text_for_printing" en forma de tabla hexadecimal de tamaño de X * 0xf(16 en decimal).
     * Donde X es size_string_text_for_printing / 16. 
     * 
     * Se espera recibir un string_init, el cual es un string que se imprimira al inicio de cada fila.
     * Se espera recibir un string_text_for_printing el cual sea la cadena o secuencia de bytes a imprimir en formao hexadecimal
     * Se espera recibir un size_string_text_for_printing el cual sea el tamaño del string  o secuencia de bytes a imprimir.
     * Se espera a recibir un encoder_val el cual indica el tamaño de las direcciones de memoria a imprimir:
     *      Posibles valores para encoder_x86 encoder_val:
     *          - ENCODER_IN_16bits = 0 : para 16bits
     *          - ENCODER_IN_32bits = 1 : para 32bits
     *          - ENCODER_IN_64bits = 2 : para 64bits
     * 
     * En caso de que string_init no sea un puntero valido o NULL se usara una cadena vacia por defecto.
     * En caso de que string_text_for_printing no sea un puntero valido o NULL, la funcion no imprimira nada.
     * En caso de que size_string_text_for_printing sea 0 no se imprimira nada.
     *  
     */

    char my_string_default[] = "";
    if ((string_text_for_printing == NULL) || (size_string_text_for_printing == 0)) return; // error
    if (string_init == NULL) string_init = my_string_default; // no se ingreso un valor de incio que usar para imprimir cada fila

    size_t size;
    char *buffer_Position_memory = get_addr_to_encoder_x86(0, encoder_val), *buffer_spaces = NULL;

    uint32_t level_space = strlen(buffer_Position_memory);
    buffer_spaces = (char *)malloc(level_space);
    for (uint32_t i = 0; i < level_space; i++) buffer_spaces[i] = ' '; // generar espacios para la primera fila
    buffer_spaces[level_space] = '\0';
    unsigned int random_color = jenkins_hash(string_text_for_printing[0], level_space, 0, 1, 2, 3, 4);

    printf_color("\n%s%s ", string_init, buffer_spaces);

    // imprimir primera fila. (empezamos desde 0x23 para tener unos colores agradables)
    for (uint16_t r = 0x23; r < 0x33; r++) printf_color("|"FOREGROUND_COLOR_CUSTOM("%d")"%02x#{FG:reset}", r >> 2, ((uint8_t)(r-0x33)) - 0xf0 ); 
    printf_color("|\n%s"FOREGROUND_COLOR_CUSTOM("%d")"%s #{FG:reset}", string_init, ((((uint8_t)random_color >> 2)) & 0b1110111) | 0b00001001, buffer_Position_memory);

    for (uint32_t i = 0; i < size_string_text_for_printing; i++)
    {
        // imprimir el resto de filas
        unsigned int Avalue1, Avalue2, Avalue3, seed, values[] = {
            (unsigned int)size_string_text_for_printing, 
            size, level_space,  0xa0, 0xe1, string_text_for_printing[i]
        };
        
        // generar 3 valores apartir de una semilla de entrada y 6 valores(pueden ser constantes o variables)
        seed = (unsigned int)string_text_for_printing[i];
        Avalue1 = jenkins_hash(seed,    values[0], values[1], values[2], values[3], values[4], values[5]);
        Avalue2 = jenkins_hash(Avalue1, values[0], values[1], values[2], values[3], values[4], values[5]);
        Avalue3 = jenkins_hash(Avalue2, values[0], values[1], values[2], values[3], values[4], values[5]);

        // mediante la operacion ((((uint8_t)valor >> 2)) & 0b1110111) | 0b00001001
        // de puede obtener colores claros
        printf_color("|"FOREGROUND_COLOR_CUSTOM("%d")"%.2X#{BG:reset}", 
        ((((uint8_t)string_text_for_printing[i] >> 2)) & 0b1110111) | 0b00001001, 
        (uint8_t)string_text_for_printing[i]);

        // imrpimir en X * 16
        if ((i+1) % (BLOCK_SLICES / 8) == 0){
            // imprimir la siguiente filas, despues de imprimir 16 bytes
            free(buffer_Position_memory);
            buffer_Position_memory = get_addr_to_encoder_x86(i+1, encoder_val);
            random_color = jenkins_hash(string_text_for_printing[i], Avalue1, Avalue2, Avalue3, 2, 3, 4);
            printf_color("|\n%s"FOREGROUND_COLOR_CUSTOM("%d")"%s #{FG:reset}", string_init, ((((uint8_t)random_color >> 2)) & 0b1111111) | 0b00001001, buffer_Position_memory, i+1);
        }
    }
    
    free(buffer_Position_memory);
    free(buffer_spaces);
    printf("|\n");
}

void print_binary(unsigned int num, uint16_t num_bits, uint16_t init_count) {
    /*
     *  
     * print_binary(unsigned int num, uint16_t num_bits, uint16_t init_count):
     * Esta funcion imprime un numero binario de entrada con color amarillo, usando un espacio por cada nibble.
     * Se rellenara una cantidad de "init_count" con guiones en color blanco como relleno. 
     * 
     * Se espera recibir un "num" el cual sea el valor a imprimir en binario.
     * Se espera recibir un num_bits el cual indique la cantidad de bits a imprimir
     * Se espera recibir un init_count el cual desde que posicion empezar a poner 1's o 0's, hasta que no se alcanze esa posicion
     *      se rellenara con guiones.
     * 
     *  
     */
    uint16_t counter_bits = num_bits -1;
    for (uint16_t i = 1; (counter_bits  > 0) || (i <= 8); i++ ) {
        //printf("(%d)", counter_bits);
        if (i > init_count && counter_bits+1 != 0) 
        {
            printf_color("#{FG:lyellow}%d#{FG:reset}", (num >> counter_bits) & 1);
            if (counter_bits > 0) counter_bits--;
        }
        else putchar('-');
        if ((i % 4) == 0 && (i != 0)) putchar(' ');
    }
    putchar('\n');
}

/*void print_binary(unsigned int num, int num_bits) {
    for (int i = num_bits - 1; i >= 0; i--) {
        printf_color("#{FG:lyellow}%d#{FG:reset}", (num >> i) & 1);
        if (i % 4 == 0 && i != 0) printf(" "); 
    } printf("\n");
}*/

void print_opcode(Instruction_info *my_instruccion_, opcode opcode, uint8_t number_opcode) {
    printf_color("opcode#{FG:lpurple}{#{FG:lred}%d#{FG:lpurple}}#{FG:reset} (#{FG:lblue}8bits#{FG:reset}): (value) #{FG:lgreen}%02X#{FG:reset} = #{FG:lyellow}", number_opcode, *((uint8_t*)(&opcode)));
    print_binary( *((uint8_t*)(&opcode)), 8, 0);
    printf_color("#{FG:reset}opcode[#{FG:lred}%d#{FG:reset}] (#{FG:lblue}6bits#{FG:reset}): (value) #{FG:lgreen}%02X#{FG:reset} = #{FG:lyellow}", number_opcode, opcode.opcode_bits_final.opcode);
    print_binary(opcode.opcode_bits_final.opcode<< 2, 8, 0);
    //printf_color("#{FG:reset}opcode[#{FG:lred}%d#{FG:reset}].d:  #{FG:lgreen}%02X#{FG:reset} =        #{FG:lyellow}%d#{FG:reset}\n", number_opcode, opcode.opcode_bits_final.d, opcode.opcode_bits_final.d);
    //printf_color("opcode[#{FG:lred}%d#{FG:reset}].s:  #{FG:lgreen}%02X#{FG:reset} =         #{FG:lyellow}%d#{FG:reset}\n\n", number_opcode, opcode.opcode_bits_final.s, opcode.opcode_bits_final.s);
    if (number_opcode >= 2) {
        if (my_instruccion_->posicion_d != 0) {
            printf_color("#{FG:reset}opcode[#{FG:lred}%d#{FG:reset}].d(#{FG:lblue}1bit#{FG:reset}): (value) #{FG:lgreen}%02X#{FG:reset} = ", number_opcode, opcode.opcode_bits_final.d);
            print_binary(opcode.opcode_bits_final.d << (my_instruccion_->posicion_d-1), 4, 4);
        } else printf_color("#{FG:reset}opcode[#{FG:lred}%d#{FG:reset}].d(#{FG:lblue}1bit#{FG:reset}): (value) #{FG:lred}--#{FG:reset} = #{FG:lred}not exists field D#{FG:reset}\n", number_opcode, opcode.opcode_bits_final.d);
        if (my_instruccion_->posicion_s != 0) {
            printf_color("opcode[#{FG:lred}%d#{FG:reset}].s(#{FG:lblue}1bit#{FG:reset}): (value) #{FG:lgreen}%02X#{FG:reset} = ", number_opcode, opcode.opcode_bits_final.s);
            print_binary(opcode.opcode_bits_final.s << (my_instruccion_->posicion_s-1), 4, 4);
        } else printf_color("#{FG:reset}opcode[#{FG:lred}%d#{FG:reset}].s(#{FG:lblue}1bit#{FG:reset}): (value) #{FG:lred}--#{FG:reset} = #{FG:lred}not exists field S#{FG:reset}\n", number_opcode);
        if (my_instruccion_->posicion_w != 0) {
            printf_color("opcode[#{FG:lred}%d#{FG:reset}].w(#{FG:lblue}1bit#{FG:reset}): (value) #{FG:lgreen}%02X#{FG:reset} = ", number_opcode, get_bit_w(my_instruccion_));
            print_binary(my_instruccion_->posicion_w, 4, 4);
        } else printf_color("#{FG:reset}opcode[#{FG:lred}%d#{FG:reset}].w(#{FG:lblue}1bit#{FG:reset}): (value) #{FG:lred}--#{FG:reset} = #{FG:lred}not exists field W#{FG:reset}\n", number_opcode, get_bit_w(my_instruccion_));
    }
    putchar('\n');
}

void print_instruccion_binary(Instruction *my_instruccion) {
    printf("Instruccion = \n");
    for (size_t i = 0; i < sizeof(Instruction); i++) {
        printf("\t\t\t\t");
        print_binary(((uint8_t*)(my_instruccion))[i], 8, 0);
        printf(" ");
    }printf("\n");
}

void print_instruccion_hex(Instruction *my_instruccion, encoder_x86 encoder_val) {
    printf("Instruccion = \n");
    char text[] = " ";
    print_table_hex(text, ((char*)(my_instruccion)), sizeof(Instruction), encoder_val);
    printf("\n");
}


void print_instruccion(Instruction_info *my_instruccion_, encoder_x86 encoder_val) {

    unsigned int Avalue1, Avalue2, Avalue3, seed, values[] = {
        my_instruccion_->instruction.opcode[0].opcode_byte.byte + 0x0f01, 
        my_instruccion_->instruction.opcode[1].opcode_byte.byte + 0xf0010a0, 
        my_instruccion_->instruction.opcode[2].opcode_byte.byte, 
        0xa0, 0xf1, 
        my_instruccion_->instruction.opcode[2].opcode_byte.byte + 1 +
        my_instruccion_->instruction.opcode[1].opcode_byte.byte +
        my_instruccion_->instruction.opcode[0].opcode_byte.byte + 20
    };
    regenerate_keys:
        //shuffle_array(values, sizeof(values)/sizeof(int));
        seed = my_instruccion_->instruction.opcode[2].opcode_byte.byte * 
                            my_instruccion_->instruction.opcode[1].opcode_byte.byte * 
                            my_instruccion_->instruction.opcode[0].opcode_byte.byte ;
        Avalue1 = jenkins_hash(seed,    values[0], values[1], values[2], values[3], values[4], values[5]);
        Avalue2 = jenkins_hash(Avalue1, values[0], values[1], values[2], values[3], values[4], values[5]);
        Avalue3 = jenkins_hash(Avalue2, values[0], values[1], values[2], values[3], values[4], values[5]);
        if ((Avalue1 & Avalue2 & Avalue3) == Avalue1) { 
                    values[0] ^= 0b10101010; values[1] ^= values[0];
                    values[2] ^= values[1];  values[3] &= values[2];
                    values[4] ^= values[3];  values[5] ^= values[4];
                    goto regenerate_keys;
        }
    
    printf_color("string instruccion: #{BG:%d;%d;%d} %s#{FG:reset} (color: %u %u %u)\n", (unsigned char)Avalue1, (unsigned char)Avalue2, (unsigned char)Avalue3,  get_string_instruction_by_id(my_instruccion_->string), (unsigned char)Avalue1, (unsigned char)Avalue2, (unsigned char)Avalue3);

    String_list_link *string_list = get_string_instruction(my_instruccion_, encoder_val);
    printf_color("total string instruccion: #{BG:%d;%d;%d} ", (unsigned char)Avalue1, (unsigned char)Avalue2, (unsigned char)Avalue3);
    print_String_list_link(string_list);
    string_list = free_String_list_link(string_list);
    printf_color("#{FG:reset}\n");

    Instruction *my_instruccion = &(my_instruccion_->instruction);
    printf_color("prefix:       #{FG:lgreen}%02X %02X %02X %02X#{FG:reset} = \n", my_instruccion->prefix[0], my_instruccion->prefix[1], my_instruccion->prefix[2], my_instruccion->prefix[3]);
    for (int i = 0; i < sizeof(uint8_t) *4; i++) {
        printf("\t\t\t\t");
        print_binary(my_instruccion->prefix[i], 8, 0);

    } printf("\n");

    printf_color("SizeOpcode(#{FG:lblue}2bits#{FG:reset}): #{FG:lgreen}%02X#{FG:reset} = ", my_instruccion_->opcode_size+1);
    print_binary(my_instruccion_->opcode_size, 2, 6);

    // mostart solo x bytes del opcode definidos por el miembro .opcode_size
    // se recorre el array de 3 posiciones de forma inversa, [2, 1, 0], siendo 2 el opcode primario.
    for (unsigned char pos = my_instruccion_->opcode_size +1; pos != 0; pos--)
        print_opcode(my_instruccion_, my_instruccion->opcode[pos], pos);
        // array[limite del array - posicion actual] == recorrer array desde el final

    
    printf_color("Mod_rm:       #{FG:lgreen}%02X#{FG:reset} =       ", *((uint8_t*)&(my_instruccion->Mod_rm)));
    print_binary( *((uint8_t*)&(my_instruccion->Mod_rm)), 8, 0);
    printf_color("mod:          #{FG:lgreen}%02X#{FG:reset} =       ", my_instruccion->Mod_rm.mod); print_binary(my_instruccion->Mod_rm.mod << 6, 8, 0);
    if ( my_instruccion_->number_reg != 0b00 ) {
        printf_color("reg:          #{FG:lgreen}%02X#{FG:reset} = %03s = ", my_instruccion->Mod_rm.reg, get_string_register(encoder_val, get_bit_w(my_instruccion_), my_instruccion->Mod_rm.reg)); 
        //(instrutions[i] & my_instruccion_->mask_reg) >> count_get_mask(actual_node->Instruction.mask_reg)
        print_binary(my_instruccion->Mod_rm.reg, 3, 2);
        printf_color("R_M:          #{FG:lgreen}%02X#{FG:reset} = %03s = ", my_instruccion->Mod_rm.R_M, get_string_register(encoder_val, get_bit_w(my_instruccion_), my_instruccion->Mod_rm.R_M));
        print_binary(my_instruccion->Mod_rm.R_M, 3, 5);
    } else {
        printf_color("reg:          #{FG:lred}%02X#{FG:reset} = #{FG:lred}---#{FG:reset} = #{FG:lred}not exists field REG#{FG:reset}\n", 0); 
        printf_color("R_M:          #{FG:lred}%02X#{FG:reset} = #{FG:lred}---#{FG:reset} = #{FG:lred}not exists field R/M#{FG:reset}\n", 0);
    }
    

    /*
     * "mod" con valor 00 junto al campo "r/m" 100 indica que se trata del modo SIB
     * Mod_rm:       AA = 00xx x100 (0x04)
     *  - mod:       02 = 00
     *  - reg:       05 =   xxx     (eliminar campo REG usando AND 0b11000111)
     *  - R_M:       02 =       100
     */
    if ((*((uint8_t*)&(my_instruccion->Mod_rm)) & 0b11000111) == 0b00000100){ // modo SIB
        printf_color("\nSIB:          #{FG:lgreen}%02X#{FG:reset} = ", *((uint8_t*)&(my_instruccion->SIB)));
        print_binary( *((uint8_t*)&(my_instruccion->SIB)), 8, 0);
        printf_color("scale:        #{FG:lgreen}%02X#{FG:reset} = ", my_instruccion->SIB.scale); print_binary(my_instruccion->SIB.scale, 2, 0);
        printf_color("index:        #{FG:lgreen}%02X#{FG:reset} = ", my_instruccion->SIB.index); print_binary(my_instruccion->SIB.index, 3, 2);
        printf_color("base:         #{FG:lgreen}%02X#{FG:reset} = ",  my_instruccion->SIB.base); print_binary(my_instruccion->SIB.base, 3, 5);
    
        printf_color("\ndisplacement: #{FG:lgreen}%02X %02X %02X %02XX#{FG:reset} = \n", my_instruccion->displacement[0], my_instruccion->displacement[1], my_instruccion->displacement[2], my_instruccion->displacement[3]);
        for (int i = 0; i < sizeof(uint8_t) * 4; i++) {
            printf("\t\t\t\t");
            print_binary(my_instruccion->displacement[i], 8, 0);
        } printf("\n");

        printf_color("immediate:    #{FG:lgreen}%02X %02X %02X %02XX#{FG:reset} = \n", my_instruccion->immediate[0], my_instruccion->immediate[1], my_instruccion->immediate[2], my_instruccion->immediate[3]);
        for (int i = 0; i < sizeof(uint8_t) * 4; i++) {
            printf("\t\t\t\t");
            print_binary(my_instruccion->immediate[i], 8, 0);
        } printf("\n");
    }
}

#endif