/*
 *  Licencia Apache, Version 2.0 con Modificacion
 *  
 *  Copyright 2023 Desmon (David)
 *  
 *  Se concede permiso, de forma gratuita, a cualquier persona que obtenga una copia de 
 *  este software y archivos de documentacion asociados (el "Software"), para tratar el 
 *  Software sin restricciones, incluidos, entre otros, los derechos de uso, copia, 
 *  modificacion, fusion, publicacion, distribucion, sublicencia y/o venta de copias del 
 *  Software, y para permitir a las personas a quienes se les proporcione el Software 
 *  hacer lo mismo, sujeto a las siguientes condiciones:
 *  
 *  El anterior aviso de copyright y este aviso de permiso se incluiran en todas las 
 *  copias o partes sustanciales del Software.
 *  
 *  EL SOFTWARE SE PROPORCIONA "TAL CUAL", SIN GARANTiA DE NINGÚN TIPO, EXPRESA O 
 *  IMPLiCITA, INCLUYENDO PERO NO LIMITADO A LAS GARANTiAS DE COMERCIABILIDAD, IDONEIDAD 
 *  PARA UN PROPoSITO PARTICULAR Y NO INFRACCIoN. EN NINGÚN CASO LOS TITULARES DEL 
 *  COPYRIGHT O LOS TITULARES DE LOS DERECHOS DE AUTOR SERaN RESPONSABLES DE NINGÚN 
 *  RECLAMO, DAnO U OTRA RESPONSABILIDAD, YA SEA EN UNA ACCIoN DE CONTRATO, AGRAVIO O DE 
 *  OTRA MANERA, QUE SURJA DE, FUERA DE O EN CONEXIoN CON EL SOFTWARE O EL USO U OTRO TIPO
 *  DE ACCIONES EN EL SOFTWARE.
 *  
 *  Ademas, cualquier modificacion realizada por terceros se considerara propiedad del 
 *  titular original de los derechos de autor. Los titulares de derechos de autor 
 *  originales no se responsabilizan de las modificaciones realizadas por terceros.
 *  
 *  Queda explicitamente establecido que no es obligatorio especificar ni notificar los 
 *  cambios realizados entre versiones, ni revelar porciones especificas de codigo 
 *  modificado.
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
        .next_string   = NULL,                    // hacer que no apunte a ningun nodo mas
        .size_string   = size_string,             // tamaño del string almacenado
        .string_action = (Action_Native_String){
            .flags     = FLAG_NONE_Sll,           // indicar que ninguna flag esta activa
            .string    = string,                  // string a almacenar
        }
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
    //join_list_to_String(list, " ");
    for (String_list_link *i = list; i != NULL; i = i->next_string) {
        printf("%s ", i->string_action.string);
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
     * Se espera recibir una lista enlazada a liberar, valida.
     *  Los nodos que tenga activada la FLAG_FREE_Sll, seran liberados sus miembros actual_string
     *  
     */
    if (list != NULL) {
        String_list_link *next_block = list; // apuntar al siguiente bloque, para no perder
        // la referencia al eliminar al bloque que apunta a este
        for (String_list_link *i = list; i != NULL; i = next_block) {
            #ifdef DEBUG_ENABLE
                printf_color("\t -> [free_String_list_link] #{FG:reset}Liberando i(#{FG:cyan}%p#{FG:reset}), flag_i(%02x), next_block(#{FG:cyan}%p#{FG:reset})#{FG:reset},  -> %s\n", i, i->string_action.flags, next_block, i->string_action.string);
            #endif

            #ifdef DEBUG_ENABLE
            printf("\t -> [free_String_list_link] string_action_struct->flags: %04x\n", i->string_action.flags);
            #endif
            if ((i->string_action.flags & FLAG_FREE_Sll) == FLAG_FREE_Sll) {
                #ifdef DEBUG_ENABLE
                printf("\t -> [free_String_list_link] Liberando String(direccion: %p, valor = \"%s\"\n", i->string_action.string, i->string_action.string);
                #endif
                if (i->string_action.string != NULL) free(i->string_action.string); // liberar el string
            }
            if ((i->string_action.flags & FLAG_RESTORE_FLAGS) == FLAG_RESTORE_FLAGS){ 
                #ifdef DEBUG_ENABLE
                printf("\t -> [free_String_list_link] Restaurando Flags...(valor actual: %04x)\n", i->string_action.flags);
                #endif
                i->string_action.flags = FLAG_NONE_Sll; // restaurar las flags
            }
            
            next_block = i->next_string; // almacenar el siguiente bloque antes de liberar el actual
            free(i); // liberar el bloque actual
        }
    }
    return NULL;
}

static inline String_list_link *push_String(String_list_link *list, char* string, size_t size_string) {
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
Action_Native_String *Start_Action_Native_String(Action_Native_String *string_action_struct) {
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(Action_Native_String*, Start_Action_Native_String)
                TYPE_DATA_DBG(Action_Native_String *, "string_action_struct = %p")
            END_TYPE_FUNC_DBG,
            string_action_struct);
    #endif 
    if (string_action_struct == NULL) {
        #ifdef DEBUG_ENABLE
        printf("\t -> [Start_Action_Native_String] string_action_struct == NULL\n");
        #endif
        return NULL;
    }
    #ifdef DEBUG_ENABLE
    printf("\t -> [Start_Action_Native_String] string_action_struct->flags: %04x\n", string_action_struct->flags);
    #endif
    if ((string_action_struct->flags & FLAG_FREE_Sll) == FLAG_FREE_Sll) {
        #ifdef DEBUG_ENABLE
        printf("\t -> [Start_Action_Native_String] Liberando String(direccion: %p, valor = \"%s\"\n", string_action_struct->string, string_action_struct->string);
        #endif
        if (string_action_struct->string != NULL) free(string_action_struct->string); // liberar el string
    }
    if ((string_action_struct->flags & FLAG_RESTORE_FLAGS) == FLAG_RESTORE_FLAGS){ 
        #ifdef DEBUG_ENABLE
        printf("\t -> [Start_Action_Native_String] Restaurando Flags...(valor actual: %04x)\n", string_action_struct->flags);
        #endif
        string_action_struct->flags = FLAG_NONE_Sll; // restaurar las flags
    }
    if ((string_action_struct->flags & FLAG_FREE_ANS) == FLAG_FREE_ANS) { // liberar estructura
        #ifdef DEBUG_ENABLE
        printf("\t -> [Start_Action_Native_String] Liberando estructura Action_Native_String...(direccion actual: %p)\n", string_action_struct);
        #endif
        free(string_action_struct);
        return NULL; // se retorna NULL si se libera la estructura
    }
    return string_action_struct; // al finalizar las acciones, se retorna la estcutura excepto que esta se libere
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

    // si no tiene campo mod o rm se terna el string solo con la instruccion
    if ((my_instruccion_->position_rm == 0b11) || (my_instruccion_->position_mod == 0b11) || (my_instruccion_->position_reg == 0b11)) return ptr_org;

    uint32_t val = 0;
    bool is_free = false;
    Action_Native_String *dest  = (Action_Native_String *)malloc(sizeof(Action_Native_String)),
                         *fuent = (Action_Native_String *)malloc(sizeof(Action_Native_String)),
                         *auxi  = (Action_Native_String *)malloc(sizeof(Action_Native_String)); // se usa para el intercambio de variables
    dest->flags  = FLAG_FREE_ANS; // indicar que la estructura a de liberarse al llamar a Start_Action_Native_String, ademas de poner todas las demas flags a 0
    fuent->flags = FLAG_FREE_ANS; // indicar que la estructura a de liberarse al llamar a Start_Action_Native_String, ademas de poner todas las demas flags a 0
    auxi->flags  = FLAG_FREE_ANS; // indicar que la estructura a de liberarse al llamar a Start_Action_Native_String, ademas de poner todas las demas flags a 0

    uint8_t destino, fuente, auxiliar; // se mantiene asi si d == 0
    String_list_link *ptr = ptr_org; // copia del puntero que usar para añadir elementos
    int size_len_register = 0;

    switch(my_instruccion_->instruction.Mod_rm.mod) { 
        case 0b00:  // Modo 00 (00b): El operando se codifica como un registro, sin desplazamiento (offset). En este modo, el campo 
                    // "R/M" especifica el registro.
            dest->string = get_string_mod_0(encoder_val, my_instruccion_);
            is_free = false; // si se marca en true, se a de marcar en la lista enlazada que el string del desplazamiento a de liberarse
            if ((encoder_val == 0b0) && (my_instruccion_->instruction.Mod_rm.R_M == 0b110)) { // desplazamiento de 16bits
                val = *((uint16_t*)&(my_instruccion_->instruction.displacement));
            } else if (encoder_val == 0b1) {  // desplazamiento de 32bits o SIB
                if (my_instruccion_->instruction.Mod_rm.R_M == 0b101) { // desplazamiento de 32bits
                    val = *((uint32_t*)&(my_instruccion_->instruction.displacement));
                } else if (my_instruccion_->instruction.Mod_rm.R_M == 0b100) { // SIB
                    dest->string = get_build_SIB_format(my_instruccion_);
                    dest->flags |= FLAG_FREE_Sll; // indicar que se a de liberar el espacio
                    goto no_formatear_registro_desplazamiento; // no formatear por que get_build_SIB_format ya devuleve el desplazamiento formateado
                    //char * s = get_build_SIB_format(my_instruccion_);
                }
            } else goto no_formatear_registro_desplazamiento;

            is_free = true; // indicar que el desplazamiento debera liberarse al liberar la lista enlazada
            // solo se realiza esta operacion si se esta operando con desplazamiento de 16 o 32bits
            size_len_register = (snprintf(NULL, 0, dest->string,  val) + 1) * sizeof(char);
            auxi->string = (char *)malloc(size_len_register); // almacenar el buffer para guardar la instruccion formateada
            sprintf(auxi->string, dest->string, val); // formatear la instruiccion
            dest->string = auxi->string;  // poner el string formateado como destino.

            no_formatear_registro_desplazamiento: // obviar el formateo de desplazamiento
                                                  // si lo anterior no se a de realizar o si hay SIB

            fuent->string = get_string_register(encoder_val, get_bit_w(my_instruccion_), my_instruccion_->instruction.Mod_rm.reg);
            if (get_bit_d(my_instruccion_)) { // si d == 1 se cambia para que reg sea fuente y rm destino
                //auxi->string  = dest->string;  // c = a
                //dest->string  = fuent->string; // a = b
                //fuent->string = auxi->string;  // b = c
                #ifdef DEBUG_ENABLE
                printf("d = 1 -> %s %s\n", fuent->string, dest->string);
                #endif
                ptr = push_String(ptr, fuent->string, CALC_SIZE_STRING_FLAG); 
                ptr = push_String(ptr, ",", CALC_SIZE_STRING_FLAG); 
                // CALC_SIZE_STRING_FLAG especifica que se a de obtener la longitud del string
                ptr = push_String(ptr, dest->string, CALC_SIZE_STRING_FLAG);
                if(is_free) ptr->string_action.flags |= FLAG_FREE_Sll; // indiciar que se a de liberar el desplazamiento
                                                         // solo si se indico que existe
            } else {
                #ifdef DEBUG_ENABLE
                printf("d = 0 -> %s %s\n", dest->string, fuent->string);
                #endif
                ptr = push_String(ptr, dest->string, CALC_SIZE_STRING_FLAG); 
                if(is_free) ptr->string_action.flags |= FLAG_FREE_Sll; // indiciar que se a de liberar el desplazamiento
                                                         // solo si se indico que existe
                ptr = push_String(ptr, ",", CALC_SIZE_STRING_FLAG); 
                // CALC_SIZE_STRING_FLAG especifica que se a de obtener la longitud del string
                ptr = push_String(ptr, fuent->string, CALC_SIZE_STRING_FLAG);
            }
            ptr = join_list_to_String(ptr_org, " "); // unir todo usando un espacio
            free_String_list_link(ptr_org); // liberar la antigua lista enlazada
            break;
        case 0b01: // Modo 01 (01b): El operando se codifica como un registro con un desplazamiento de 8 bits. En este modo, el campo 
                   // "R/M" especifica el registro, y sigue un byte inmediato que proporciona el desplazamiento.
            if (my_instruccion_->instruction.Mod_rm.R_M == 0b100) { // SIB
                    dest->string = get_build_SIB_format(my_instruccion_);
            } else {
                dest->string = get_string_mod_1(encoder_val, my_instruccion_);
                val = *((uint8_t*)&(my_instruccion_->instruction.displacement)); // desplazamientod e 8bits
                is_free = true; // si se marca en true, se a de marcar en la lista enlazada que el string del desplazamiento a de liberarse
                                // indicar que el desplazamiento debera liberarse al liberar la lista enlazada
                
                size_len_register = (snprintf(NULL, 0, dest->string,  val) + 1) * sizeof(char);
                auxi->string = (char *)malloc(size_len_register); // almacenar el buffer para guardar la instruccion formateada
                sprintf(auxi->string, dest->string, val); // formatear la instruiccion
                dest->string = auxi->string; // poner el string formateado como destino.
            }

            fuent->string = get_string_register(encoder_val, get_bit_w(my_instruccion_), my_instruccion_->instruction.Mod_rm.reg);
            if (get_bit_d(my_instruccion_)) { // si d == 1 se cambia para que reg sea fuente y rm destino
                //auxi->string  = dest->string;  // c = a
                //dest->string  = fuent->string; // a = b
                //fuent->string = auxi->string;  // b = c
                #ifdef DEBUG_ENABLE
                printf("d = 1 -> %s %s\n", fuent->string, dest->string);
                #endif
                ptr = push_String(ptr, fuent->string, CALC_SIZE_STRING_FLAG); 
                ptr = push_String(ptr, ",", CALC_SIZE_STRING_FLAG); 
                // CALC_SIZE_STRING_FLAG especifica que se a de obtener la longitud del string
                ptr = push_String(ptr, dest->string, CALC_SIZE_STRING_FLAG);
                ptr->string_action.flags |= FLAG_FREE_Sll; // indiciar que se a de liberar el desplazamiento
                                                        
            } else {
                #ifdef DEBUG_ENABLE
                printf("d = 0 -> %s %s\n", dest->string, fuent->string);
                #endif
                ptr = push_String(ptr, dest->string, CALC_SIZE_STRING_FLAG); 
                ptr->string_action.flags |= FLAG_FREE_Sll; // indiciar que se a de liberar el desplazamiento
                                                         
                ptr = push_String(ptr, ",", CALC_SIZE_STRING_FLAG); 
                // CALC_SIZE_STRING_FLAG especifica que se a de obtener la longitud del string
                ptr = push_String(ptr, fuent->string, CALC_SIZE_STRING_FLAG);
            }
            ptr = join_list_to_String(ptr_org, " "); // unir todo usando un espacio
            free_String_list_link(ptr_org); // liberar la antigua lista enlazada
            break; 
        case 0b10: //  Modo 10 (10b): El operando se codifica como un registro con un desplazamiento de 32 bits. En este modo, el campo 
                   // "R/M" especifica el registro, y sigue una palabra doble (dword) inmediata que proporciona el desplazamiento.
            if (my_instruccion_->instruction.Mod_rm.R_M == 0b100) { // SIB
                    dest->string = get_build_SIB_format(my_instruccion_);
            } else {
                dest->string = get_string_mod_2(encoder_val, my_instruccion_);
                is_free = true; // si se marca en true, se a de marcar en la lista enlazada que el string del desplazamiento a de liberarse
                                // indicar que el desplazamiento debera liberarse al liberar la lista enlazada
                if (encoder_val == 0b0)  // desplazamiento de 16bits
                    val = *((uint16_t*)&(my_instruccion_->instruction.displacement));
                else if (encoder_val == 0b1)  // desplazamiento de 32bits
                    val = *((uint32_t*)&(my_instruccion_->instruction.displacement));
                size_len_register = (snprintf(NULL, 0, dest->string,  val) + 1) * sizeof(char);
                auxi->string = (char *)malloc(size_len_register); // almacenar el buffer para guardar la instruccion formateada
                sprintf(auxi->string, dest->string, val); // formatear la instruiccion
                dest->string = auxi->string; // poner el string formateado como destino.
            }

            fuent->string = get_string_register(encoder_val, get_bit_w(my_instruccion_), my_instruccion_->instruction.Mod_rm.reg);
            if (get_bit_d(my_instruccion_)) { // si d == 1 se cambia para que reg sea fuente y rm destino
                //auxi->string  = dest->string;  // c = a
                //dest->string  = fuent->string; // a = b
                //fuent->string = auxi->string;  // b = c
                #ifdef DEBUG_ENABLE
                printf("d = 1 -> %s %s\n", fuent->string, dest->string);
                #endif
                ptr = push_String(ptr, fuent->string, CALC_SIZE_STRING_FLAG); 
                ptr = push_String(ptr, ",", CALC_SIZE_STRING_FLAG); 
                // CALC_SIZE_STRING_FLAG especifica que se a de obtener la longitud del string
                ptr = push_String(ptr, dest->string, CALC_SIZE_STRING_FLAG);
                ptr->string_action.flags |= FLAG_FREE_Sll; // indiciar que se a de liberar el desplazamiento
                                                         
            } else {
                #ifdef DEBUG_ENABLE
                printf("d = 0 -> %s %s\n", dest->string, fuent->string);
                #endif
                ptr = push_String(ptr, dest->string, CALC_SIZE_STRING_FLAG); 
                ptr->string_action.flags |= FLAG_FREE_Sll; // indiciar que se a de liberar el desplazamiento
                                                         
                ptr = push_String(ptr, ",", CALC_SIZE_STRING_FLAG); 
                // CALC_SIZE_STRING_FLAG especifica que se a de obtener la longitud del string
                ptr = push_String(ptr, fuent->string, CALC_SIZE_STRING_FLAG);
            }
            ptr = join_list_to_String(ptr_org, " "); // unir todo usando un espacio
            free_String_list_link(ptr_org); // liberar la antigua lista enlazada
            break;
        case 0b11: // Modo 11 (11b): El operando se codifica como un registro de memoria. En este modo, el campo "R/M" especifica el 
                   // registro de memoria.
            // d = 0 -> <instruccion> <registro R/M>, <registro reg>
            // d = 1 -> <instruccion> <registro reg>, <registro R/M>
            destino = my_instruccion_->instruction.Mod_rm.R_M;
            fuente = my_instruccion_->instruction.Mod_rm.reg; // se mantiene asi si d == 0
            if (get_bit_d(my_instruccion_)) { // si d == 1 se cambia para que reg sea fuente y rm destino
                auxiliar = destino;  // c = a
                destino  = fuente;   // a = b
                fuente   = auxiliar; // b = c
            }
            string = get_string_register(encoder_val, get_bit_w(my_instruccion_), destino);
            ptr = push_String(ptr, string, CALC_SIZE_STRING_FLAG); 
            ptr = push_String(ptr, ",", CALC_SIZE_STRING_FLAG); 
            // CALC_SIZE_STRING_FLAG especifica que se a de obtener la longitud del string
            string = get_string_mod_3(encoder_val, get_bit_w(my_instruccion_), fuente);
            ptr = push_String(ptr, string, CALC_SIZE_STRING_FLAG);
            ptr = join_list_to_String(ptr_org, " "); // unir todo usando un espacio
            free_String_list_link(ptr_org); // liberar la antigua lista enlazada
            break;
        default: goto exit_get_string_instruction; // error al descodificar la instruccion

    }
    
    dest  = Start_Action_Native_String(dest);  // liberar las estructuturas en este caso y el string si se le indico
    fuent = Start_Action_Native_String(fuent); // liberar las estructuturas en este caso y el string si se le indico
    auxi  = Start_Action_Native_String(auxi);  // liberar las estructuturas en este caso y el string si se le indico

    exit_get_string_instruction:
    return ptr;
}

static inline size_t get_size_to_String(String_list_link *list) {
    /*
     *  
     * get_size_to_String(String_list_link *list):
     * Esta funcion genera realiza la suma del tamaño de todos los buffers(strings) contenidos en la lista enlazada.
     * 
     * Se espera recibir un list el cual sea la lista enlazada con los strings.
     * 
     * Se retorna el sumario de los tamaños de todos los strings, o en caso de error, -1.
     *  
     */
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(size_t, get_size_to_String)
                TYPE_DATA_DBG(list *, " = %p")
            END_TYPE_FUNC_DBG,
            list);
    #endif
    if (list == NULL) return -1; 

    size_t size = 0;
    for (String_list_link *i = list; i != NULL; i = i->next_string) size += i->size_string;
    return size;
}

static inline size_t get_number_nodes_String_list_link(String_list_link *list) {
    /*
     *  
     * get_number_nodes_String_list_link(String_list_link *list):
     * Esta funcion cuenta la cantidad de nodos en una lista enlazada.
     * 
     * Se espera recibir un list el cual sea la lista enlazada con los strings. En caso de ser NULL se retorna NULL
     * 
     * Se retorna numero de nodos encontrados.
     *  
     */
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(size_t, get_size_to_String)
                TYPE_DATA_DBG(list *, " = %p")
            END_TYPE_FUNC_DBG,
            list);
    #endif
    if (list == NULL) return -1; 

    size_t size = 0;
    for (String_list_link *i = list; i != NULL; i = i->next_string) size++;
    return size;
}

String_list_link *join_list_to_String(String_list_link *list, char* string_for_join) {
    /*
     *  
     * join_list_to_String(String_list_link *list, char* string_for_join):
     * Esta funcion une los strings de una lista enlazada en un solo strings, retornando una nueva lista enlazada, 
     * con el string nuevo contenido.
     * 
     * - Se espera recibir un list que sea un puntero valido, en caso de no serlo, o ser NULL se retorna NULL.
     * - Se espera recibir un string_for_join, el cual sea un string que usar de rellena para unir cada elementos,
     *      en caso de ser NULL o un puntero no valido, la union se realizara sin cadenas de relleno.
     * 
     * Se una lista enlazada con el nuevo string concatenado.
     * 
     * **Si se llama a esta funcion, se a de liberar la memoria de esta funcion, poner campo flags en 1
     *  
     */
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(String_list_link *, join_list_to_String)
                TYPE_DATA_DBG(String_list_link *, " list = %p")
                TYPE_DATA_DBG(char *, " string_for_join = \"%s\"")
            END_TYPE_FUNC_DBG,
            list, string_for_join);
    #endif
    if (list == NULL) return NULL; 
    // obtener el tamaño de todos los strings de la lista enlazada y reservar memoria:
    size_t size_all_string = get_size_to_String(list) + 1; // añadir uno para el caracter final \0
    char *string_join = NULL;
    size_t counter_string_join = 0;
    if (string_for_join == NULL) { // en caso de de no haya un string que usar para unir
        string_join = (char*)malloc(sizeof(char) * size_all_string); 
        for (String_list_link *i = list; i != NULL; i = i->next_string) {
            memcpy((string_join + counter_string_join), i->string_action.string,  i->size_string); // copiar bytes
            counter_string_join += i->size_string; // cambiar el cursor de inicio de posicion
        }
    } else {
        size_t size_of_string_for_join = strlen(string_for_join);
        size_all_string += ((get_number_nodes_String_list_link(list)-1) * size_of_string_for_join);
        string_join = (char*)malloc(sizeof(char) * size_all_string ); 
        for (String_list_link *i = list; i != NULL; i = i->next_string) {
            memcpy((string_join + counter_string_join), i->string_action.string,  i->size_string); // copiar bytes
            counter_string_join += i->size_string; // cambiar el cursor de inicio de posicion
            if ( i->next_string != NULL) { // revisar si este es el penultimo bloque
                memcpy(string_join + counter_string_join, string_for_join, size_of_string_for_join); // copiar bytes
                counter_string_join += size_of_string_for_join;
            }
        }
    }
    string_join[size_all_string-1] = '\0'; // poner caracter nulo

    String_list_link *new_list_string = Init_String(string_join, size_all_string-1);
    new_list_string->string_action.flags |= FLAG_FREE_Sll; // indicar que se a de liberar la memoria de size_string
                            // necesario para la funcion free_String_list_link
    return new_list_string;
}

#endif