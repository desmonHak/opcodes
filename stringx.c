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
#ifndef __STRINGX_C__
#define __STRINGX_C__

#include "stringx.h"

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

String_list_link *push_String(String_list_link *list, char* string, size_t size_string) {
    /*
     *  
     * push_String(String_list_link *list, char* string, size_t size_string):
     * Esta funcion añade un nodo a la lista enlazada.
     * 
     * Se espera recibir un list que sea valido, en caso de ser NULL o un puntero no valido, se retorna NULL.
     * Se espera recibir un string para crear el nuevo nodo.
     * Se espera recibir un size_string con el tamaño del string, en caso de ser 0 se calcula el tamaño.
     * 
     * Se espera retornar una lista enlazada con el nuevo nodo creado.
     *  
     */
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(String_list_link*, push_String)
                TYPE_DATA_DBG(String_list_link *, "list = %p")
            END_TYPE_FUNC_DBG,
            list);
    #endif
    if (list == NULL) return NULL; // la lista enlazada recibida es un puntero no valido o es nulo

    list->next_string = Init_String(string, size_string);
    return list->next_string;
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
    char *string = get_string_instruction_by_id(my_instruccion_->string); // string de la instruccion ya descodificada
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
    switch(my_instruccion_->instruction.Mod_rm.mod) { 
        case 0b00:  // Modo 00 (00b): El operando se codifica como un registro, sin desplazamiento (offset). En este modo, el campo 
                    // "R/M" especifica el registro.
            break;
        case 0b10: // Modo 01 (01b): El operando se codifica como un registro con un desplazamiento de 8 bits. En este modo, el campo 
                   // "R/M" especifica el registro, y sigue un byte inmediato que proporciona el desplazamiento.
        case 0b01: //  Modo 10 (10b): El operando se codifica como un registro con un desplazamiento de 32 bits. En este modo, el campo 
                   // "R/M" especifica el registro, y sigue una palabra doble (dword) inmediata que proporciona el desplazamiento.

        case 0b11: // Modo 11 (11b): El operando se codifica como un registro de memoria. En este modo, el campo "R/M" especifica el 
                   // registro de memoria.
            string = get_string_register(encoder_val, get_bit_w(my_instruccion_), destino);
            ptr = push_String(ptr, string, CALC_SIZE_STRING_FLAG); 
            // CALC_SIZE_STRING_FLAG especifica que se a de obtener la longitud del string
            string = get_string_register(encoder_val, get_bit_w(my_instruccion_), fuente);
            ptr = push_String(ptr, string, CALC_SIZE_STRING_FLAG);
            break;
        default: break;// error al descodificar la instruccion

    }
    
    exit_get_string_instruction:
    return ptr_org;
}


#endif