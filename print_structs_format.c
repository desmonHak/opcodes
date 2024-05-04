#ifndef __PRINT_STRUCTS_FORMAT_C_
#define __PRINT_STRUCTS_FORMAT_C_

#include "print_structs_format.h"

String_list_link* Init_String(char *string, size_t size_string) {
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(String_list_link*, Init_String)
                TYPE_DATA_DBG(char *, "string = %s")
                TYPE_DATA_DBG(size_t, "size_string = %zu")
            END_TYPE_FUNC_DBG,
            string, size_string);
    #endif
    /*
     *  
     * Init_String(char *string, size_t size_string):
     * Esta funcion inicializa una lista enlazada haciendo uso de la estructura String_list_link
     * 
     * Se espera recibir string el cual seria un puntero a una secuencia de caracteres almacenados,
     * se espera recibir size_string, en el caso de que se recibiera un puntero valido string y un
     * (size_string == 0) la funcion determinara el tamaño del string de entrada.
     * 
     * En caso de que string no sea un puntero valido o NULL, size_string sera definido en 0,
     * independientemente de la entrada de size_string.
     * En caso de que malloc falle al reservar memoria para la estructrua de lista enlazada, o 
     * size_string sea -1(no debe ingresarse este valor, se reserva para indicar errores),
     * El puntero devuelto por la estructura es NULL.
     *  
     */
    if (size_string == -1) return NULL;
    String_list_link *ptr = (String_list_link *)malloc(sizeof(String_list_link));
    if (ptr == NULL) return ptr; // en caso de que malloc falle se retorna

    // vulnerabilidad si string no contiene \0 al final, se a de asegurar la existencia de este caracter
    // al final de string.
    if ((size_string == CALC_SIZE_STRING_FLAG) && (string != NULL)) size_string = strlen(string);
    else if (string == NULL) size_string = 0;

    *ptr = (String_list_link) {
        .actual_string = string,     // string a almacenar
        .next_string   = NULL,
        .size_string   = size_string // tamaño del string almacenado
    };
    return ptr;
}

void print_String_list_link(String_list_link *list) {
    //#ifdef DEBUG_ENABLE
    //    DEBUG_PRINT(DEBUG_LEVEL_INFO,
    //        INIT_TYPE_FUNC_DBG(void, print_String_list_link)
    //            TYPE_DATA_DBG(String_list_link *, "list = %p")
    //        END_TYPE_FUNC_DBG,
    //        list);
    //#endif
    /*
     *  
     * print_String_list_link(String_list_link *list):
     * Esta funcion imprime una lista enlazada del tipo String_list_link
     * 
     * Se espera recibir un String_list_link a imprimir
     *  
     */
    if (list == NULL) printf_color("#{FG:lred}In String_list_link final list or invalid...#{FG:reset}");

    for (String_list_link *i = list; i != NULL; i = i->next_string) {
        printf("%s ", i->actual_string);
    }
}

String_list_link* free_String_list_link(String_list_link* list) {
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(String_list_link*, free_String_list_link)
                TYPE_DATA_DBG(Instruction_info *, "list = %p")
            END_TYPE_FUNC_DBG,
            list);
    #endif
    /*
     *  
     * free_String_list_link(String_list_link* list)
     * Esta funcion libera una lista enlazada del tipo String_list_link
     * 
     * Se espera recibir una lista enlazada a liberar, valida
     *  
     */
    if (list != NULL) {
        size_t count_blocks_nodes_in_list = 0; // contar cuantos bloques tiene la lista enlazada:
        String_list_link *next_block; // apuntar al siguiente bloque, para no perder
        // la referencia al eliminar al bloque que apunta a este
        for (String_list_link *i = list; i != NULL; i = next_block) {
            #ifdef DEBUG_ENABLE
                printf_color("#{FG:reset}Liberando i(#{FG:lred}%p#{FG:reset}), next_block(#{FG:lred}%p#{FG:reset}) #{FG:reset}\n", i, next_block);
            #endif
            next_block = i->next_string; // almacenar el siguiente bloque antes de liberar el actual
            free(i); // liberar el bloque actual
        }
    }
    return NULL;
}

String_list_link* get_string_instruction(Instruction_info *my_instruccion_, encoder_x86 encoder_val) {
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(String_list_link*, get_string_instruction)
                TYPE_DATA_DBG(Instruction_info *, "my_instruccion_ = %p")
                TYPE_DATA_DBG(encoder_x86, "encoder_val = %d")
            END_TYPE_FUNC_DBG,
            my_instruccion_, encoder_val);
    #endif
    /*
     *  
     * get_string_instruction(Instruction_info *my_instruccion_, encoder_x86 encoder_val):
     * Esta funcion genera una lista enlazada haciendo uso de la estructura String_list_link
     * donde se almacenara como resultado el string de la descodificacion de una instruccion.
     * 
     * Se espera recibir my_instruccion_ donde deberia estar contenido la informacion de la instruccion
     * a descodificar y se espera recibir un valor encoder_x86 que dfina el si sera descorficada para
     * 16, 32 o 64bits.
     * 
     * Se espera retornar una lista enlazada con los string's descodificados, en caso de que malloc
     * falle, el valor devuelto es NULL. En caso de que los strings a contener no puedan ser reservador correctamente
     * se emitira la estrcutura con los cambios miemrbros en NULL. En el cado de que my_instruccion_ no pueda
     * ser descodificada de forma correcta por falta de errores o algun otro caso, el campo miembro size_string
     * de la estructura sera -1.
     *  
     */
    char *string = get_string_instrution(my_instruccion_->string); // string de la instruccion ya descodificada
    String_list_link *ptr_org = Init_String(string, CALC_SIZE_STRING_FLAG); // el tamaño del string se calcula dentro de la funcion
    if (ptr_org == NULL) goto exit_get_string_instruction; // error ocurrio

    // d = 0 -> <instruccion> <registro R/M>, <registro reg>
    // d = 1 -> <instruccion> <registro reg>, <registro R/M>
    uint8_t destino = my_instruccion_->instruction.Mod_rm.R_M, fuente = my_instruccion_->instruction.Mod_rm.reg, auxiliar; // se mantiene asi si d == 0
    if (get_bit_d(my_instruccion_)) { // si d == 1 se cambia para que reg sea fuente y rm destino
        auxiliar = destino;  // c = a
        destino  = fuente;   // a = b
        fuente   = auxiliar; // b = c
    }
    
    String_list_link *ptr = ptr_org; // copia del puntero que usar para añadir elementos
    switch (my_instruccion_->number_reg) { // si hay algun registro, se obtiene
        case 0b00: break; // - no usa campo reg
        case 0b10: // uso dos campos como registros, "reg" y "r/m"
            // obtener registro del campo RM
            string = get_string_register(encoder_val, get_bit_w(my_instruccion_), destino);
            ptr->next_string = Init_String(string, CALC_SIZE_STRING_FLAG);
            ptr = ptr->next_string;
        case 0b01: // solo tiene un campo de registro, "reg"
        // obtener registro del campo reg
            string = get_string_register(encoder_val, get_bit_w(my_instruccion_), fuente);
            ptr->next_string = Init_String(string, CALC_SIZE_STRING_FLAG);
            ptr = ptr->next_string;
            break;
        default: // error al descodificar la instruccion
            ptr->size_string = -1;
            goto exit_get_string_instruction;
    }
    
    exit_get_string_instruction:
    return ptr_org;
}

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
    char *buffer_Position_memory, *buffer_spaces = NULL;
    switch (encoder_val) {
        case ENCODER_IN_16bits:
            size = (snprintf(NULL, 0, "%004x", addr) + 1) * sizeof(char);
            buffer_Position_memory = (char *)malloc(size);
            if (buffer_Position_memory == NULL) return NULL;
            sprintf(buffer_Position_memory, "%004x", addr);
            break;
        case ENCODER_IN_32bits:
            size = (snprintf(NULL, 0, "%008x", addr) + 1) * sizeof(char);
            buffer_Position_memory = (char *)malloc(size);
            if (buffer_Position_memory == NULL) return NULL;
            sprintf(buffer_Position_memory, "%008x", addr);
            break;
        case ENCODER_IN_64bits:
            size = (snprintf(NULL, 0, "%016x", addr) + 1) * sizeof(char);
            buffer_Position_memory = (char *)malloc(size);
            if (buffer_Position_memory == NULL) return NULL;
            sprintf(buffer_Position_memory, "%016x", addr);
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
    uint16_t global_counter = 0;
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
    printf_color("opcode#{FG:lpurple}{#{FG:lred}%d#{FG:lpurple}}#{FG:reset} (#{FG:lblue}8bits#{FG:reset}): #{FG:lgreen}%02X#{FG:reset} = #{FG:lyellow}", number_opcode, *((uint8_t*)(&opcode)));
    print_binary( *((uint8_t*)(&opcode)), 8, 0);
    printf_color("#{FG:reset}opcode[#{FG:lred}%d#{FG:reset}] (#{FG:lblue}6bits#{FG:reset}): (value) #{FG:lgreen}%02X#{FG:reset} = #{FG:lyellow}", number_opcode, opcode.opcode_bits_final.opcode);
    print_binary(opcode.opcode_bits_final.opcode<< 2, 8, 0);
    //printf_color("#{FG:reset}opcode[#{FG:lred}%d#{FG:reset}].d:  #{FG:lgreen}%02X#{FG:reset} =        #{FG:lyellow}%d#{FG:reset}\n", number_opcode, opcode.opcode_bits_final.d, opcode.opcode_bits_final.d);
    //printf_color("opcode[#{FG:lred}%d#{FG:reset}].s:  #{FG:lgreen}%02X#{FG:reset} =         #{FG:lyellow}%d#{FG:reset}\n\n", number_opcode, opcode.opcode_bits_final.s, opcode.opcode_bits_final.s);
    if (number_opcode == 2) {
        if (my_instruccion_->posicion_d != 0) {
            printf_color("#{FG:reset}opcode[#{FG:lred}%d#{FG:reset}].d(#{FG:lblue}1bit#{FG:reset}): (value) #{FG:lgreen}%02X#{FG:reset} = #{FG:reset}", number_opcode, opcode.opcode_bits_final.d);
            print_binary(opcode.opcode_bits_final.d, 1, 6);
        } else printf_color("#{FG:reset}opcode[#{FG:lred}%d#{FG:reset}].d(#{FG:lblue}1bit#{FG:reset}): #{FG:lred}%02X#{FG:reset} = #{FG:lred}not exists field D#{FG:reset}\n", number_opcode, opcode.opcode_bits_final.d);
        if (my_instruccion_->posicion_s != 0) {
            printf_color("opcode[#{FG:lred}%d#{FG:reset}].s(#{FG:lblue}1bit#{FG:reset}): (value) #{FG:lgreen}%02X#{FG:reset} = #{FG:reset}", number_opcode, opcode.opcode_bits_final.s);
            print_binary(opcode.opcode_bits_final.d, 1, 7);
        } else printf_color("#{FG:reset}opcode[#{FG:lred}%d#{FG:reset}].s(#{FG:lblue}1bit#{FG:reset}): #{FG:lred}%02X#{FG:reset} = #{FG:lred}not exists field S#{FG:reset}\n", number_opcode, opcode.opcode_bits_final.s);
        if (my_instruccion_->posicion_w != 0) {
            printf_color("opcode[#{FG:lred}%d#{FG:reset}].w(#{FG:lblue}1bit#{FG:reset}): (value) #{FG:lgreen}%02X#{FG:reset}   -> #{FG:lred}(#{FG:reset}mask#{FG:lred})#{FG:reset} #{FG:reset}", number_opcode, get_bit_w(my_instruccion_));
            print_binary(my_instruccion_->posicion_w, 4, 4);
        } else printf_color("#{FG:reset}opcode[#{FG:lred}%d#{FG:reset}].w(#{FG:lblue}1bit#{FG:reset}): #{FG:lred}%02X#{FG:reset} = #{FG:lred}not exists field W#{FG:reset}\n", number_opcode, get_bit_w(my_instruccion_));
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
    
    printf_color("string instruccion: #{BG:%d;%d;%d} %s#{FG:reset} (color: %u %u %u)\n", (unsigned char)Avalue1, (unsigned char)Avalue2, (unsigned char)Avalue3,  get_string_instrution(my_instruccion_->string), (unsigned char)Avalue1, (unsigned char)Avalue2, (unsigned char)Avalue3);

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
        print_opcode(my_instruccion_, my_instruccion->opcode[3-pos], pos);
        // array[limite del array - posicion actual] == recorrer array desde el final

    
    printf_color("Mod_rm:       #{FG:lgreen}%02X#{FG:reset} =       ", *((uint8_t*)&(my_instruccion->Mod_rm)));
    print_binary( *((uint8_t*)&(my_instruccion->Mod_rm)), 8, 0);
    printf_color("mod:          #{FG:lgreen}%02X#{FG:reset} =       ", my_instruccion->Mod_rm.mod); print_binary(my_instruccion->Mod_rm.mod, 2, 0);
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