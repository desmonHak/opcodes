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
#ifndef __INSTRUTTIONS_FORMAT_C__
#define __INSTRUTTIONS_FORMAT_C__

#include "instrutions_format.h"

List_instrution* init_List_instrution() {
    /*
     *  
     * init_List_instrution():
     * Esta funcion inicializa una lista enlazada haciendo uso de la estructura List_instrution.
     * 
     * 
     * En caso de que malloc falle al reservar memoria para la estructrua de lista enlazada,
     * el puntero devuelto por la estructura es NULL.
     *  
     */

    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(List_instrution*, init_List_instrution)
        END_TYPE_FUNC_DBG);
    
    List_instrution *list_instrution_resb = (List_instrution *)malloc(sizeof(List_instrution));
    if (list_instrution_resb == NULL) return NULL; // error malloc
    memset(list_instrution_resb, 0x00, sizeof(List_instrution)); // poner la estrcutura a 0
    return list_instrution_resb;
}


size_t get_number_instrutions(List_instrution *instrutions) {
    /*
     *  
     * get_number_instrutions(List_instrution *instrutions):
     * cuenta la cantidad de instrucciones encontradas, usando la lista enlazada.
     * 
     * Se espera recibir un puntero valido instrutions el cual sea la lista enlazada con las instrucciones
     * 
     * En caso de que instrutions no sea un puntero valido el valor retornado es 0.
     *  
     */
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(size_t, get_number_instrutions)
        TYPE_DATA_DBG(List_instrution *, "instrutions = %p")
        END_TYPE_FUNC_DBG,
        instrutions);

    size_t number_instrutions = 0;
    for (List_instrution *i = instrutions; i != NULL; i = i->next_list_instrution) number_instrutions++;
    return number_instrutions;
}

/*List_instrution * pop_nodo_List_instrution(List_instrution *list_instrution, List_instrution *nodo) {
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(List_instrution *, pop_nodo_List_instrution)
            TYPE_DATA_DBG(List_instrution *, "list_instrution = %p")
            TYPE_DATA_DBG(List_instrution *, "nodo = %p")
            END_TYPE_FUNC_DBG,
            list_instrution, nodo);
    #endif
    
    if ((list_instrution == NULL) || (nodo == NULL)) return NULL;
    //if (list_instrution->next_list_instrution == NULL) return NULL;

    List_instrution *last_block; // apuntar al ultimo bloque, para no perderle (A)
    for (List_instrution *i = list_instrution; i->next_list_instrution != NULL;i = i->next_list_instrution) {
        if (i->next_list_instrution == nodo) {
            #ifdef DEBUG_ENABLE
            printf_color("#{FG:lred} liberando nodo %p de lista %p#{FG:reset}", nodo, list_instrution);
            #endif
            List_instrution *next_block =  i->next_list_instrution; // obtener el bloque C
            free(i); // eliminar el bloque B
            last_block->next_list_instrution = next_block; // hacer que A apunte a C
            break;
        }
        #ifdef DEBUG_ENABLE 
        #endif
        else { 
            printf_color("#{FG:lred} nodo %p no se eliminara\n", i);
            printf_color("#{FG:lred} \t>> [b1](%02x) [b2](%02x) [b3](%02x)\n", 
            i->Instruction.instruction.opcode[0].opcode_byte.byte,
            i->Instruction.instruction.opcode[1].opcode_byte.byte,
            i->Instruction.instruction.opcode[2].opcode_byte.byte);
            printf_color("\t>> #{FG:lred} prefix: #{FG:white}%02X %02X %02X %02X#{FG:reset} = \n", 
                i->Instruction.instruction.prefix[0], 
                i->Instruction.instruction.prefix[1], 
                i->Instruction.instruction.prefix[2], 
                i->Instruction.instruction.prefix[3]);
        }
        
        
    }
    return list_instrution;
}*/

List_instrution * pop_List_instrution(List_instrution *list_instrution, size_t position){
    /*
     *
     * pop_List_instrution(List_instrution *list_instrution, size_t position):
     * Elimina el primer elemento de la lista enlazada si position es 0. en caso contrario se elimina el nodo
     *  de esa posicion
     * 
     * Se recibe un list_instrution el cual es la lista enlazada, en caso de ser un puntero no valido o
     *  NULL se retorna NULL.
     * Se espera recibir position, el cual indica la posicion del bloque a eliminar. En caso de que sea 0
     *  se eliminara el primer nodo. y se retornara el siguiente a este.
     * En caso se que no se pueda eliminar el nodo especificado por posicion, por que la lista enlazada
     *  no tiene un elemento "position", la funcion retornara NULL.
     * 
     */
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(List_instrution *, pop_List_instrution)
        TYPE_DATA_DBG(List_instrution *, "list_instrution = %p")
        TYPE_DATA_DBG(size_t, "position = %zu")
        END_TYPE_FUNC_DBG,
        list_instrution, position);

    if (list_instrution == NULL) return NULL;
    if (position == 0){
        List_instrution *next_block = list_instrution->next_list_instrution;
        free(list_instrution);
        return next_block;
    }
    size_t number_instrutions = 1; // empezar en 1 como si del siguiente bloque se tratara, pues 
    // eliminar el bloque 0 se hace en la parte superior.
    List_instrution *last_block = list_instrution; // apuntar al ultimo bloque, para no perderle (A)
    for (List_instrution *i = list_instrution; i->next_list_instrution != NULL; number_instrutions++) {
        if (number_instrutions == position) {
            // se obtiene la direccion del siguiente bloque, del bloque a eliminar. En el caso de A -> B -> C,
            // si se quiere eliminar B, desde la iteracion de A se obtiene la direccion de C para liberar el bloque B
            // y permitir que el bloque apunte a C == A -> C
            List_instrution *next_block =  i->next_list_instrution; // obtener el bloque C
            free(i); // eliminar el bloque B
            last_block->next_list_instrution = next_block; // hacer que A apunte a C
            break;
        } else {
            last_block = i;
            i = i->next_list_instrution; // hacer que A apunte al siguiente bloque si aun no se esta en la posicion a eliminar
        }
    }
    if (number_instrutions != position) return NULL;
    return list_instrution;
}

List_instrution *push_List_instrution(List_instrution *list_instrution) {

    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(List_instrution *, push_List_instrution)
            TYPE_DATA_DBG(List_instrution *, "list_instrution = %p")
        END_TYPE_FUNC_DBG,
        list_instrution);

    /*
     *
     * Se recibe un list_instrution el cual es la lista enlazada, y se retorna un
     * puntero de la misma lista enlazada, apuntando al ultimo elemento añadido.
     * 
     */
    if (list_instrution == NULL) return NULL;

    List_instrution *list_instrution_resb = init_List_instrution(); // crear un nuevo nodo para la lista enlazada

    // recorrer la lista enlazada en busca del ultimo elemento existente para agregar el nuevo nodo
    List_instrution *i = list_instrution;
    while (i->next_list_instrution != NULL) i = i->next_list_instrution;
    
    i->next_list_instrution = list_instrution_resb; // asignar el nuevo nodo al ultimo elemento, conviertiendo a este nuevo nodo en el ultimo elemento
    list_instrution_resb->id = i->id + 1;
    list_instrution_resb->next_list_instrution = NULL;

    return list_instrution_resb;
}


void print_List_instrution(List_instrution *list_instrution, encoder_x86 encoder_val) {
    /*
     *
     * print_List_instrution(List_instrution *list_instrution, encoder_x86 encoder_val):
     * Imprime una lista enlazada de instrucciones.
     * 
     * Se recibe un list_instrution el cual es la lista enlazada, en caso de ser un puntero no valido o
     *  NULL imprime nada.
     * 
     * Se espera recibir encoder_val, el cual indique que tamaño descodificar las instrucciones
     * 
     */

    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(void, print_List_instrution)
            TYPE_DATA_DBG(List_instrution *, "list_instrution = %p")
        END_TYPE_FUNC_DBG,
        list_instrution);

    if (list_instrution == NULL) printf("finall list or invalid...");

    for (List_instrution *i = list_instrution; i != NULL; i = i->next_list_instrution) {
        printf("\n\nlist_instrution->next_list_instrution = %p\n", i->next_list_instrution);
        printf("list_instrution[%zu] = %p\n", i->id, i);
        print_instruccion(&(i->Instruction), encoder_val);
        //print_instruccion_binary(&(i->Instruction.instruction));
        print_instruccion_hex(&(i->Instruction.instruction), encoder_val);
        char text[] = " ";
        print_table_hex(text, (char*)&(i->Instruction), sizeof(Instruction_info), encoder_val);
        
    }
}

uint8_t get_rm_form_byte(Instruction_info *Instruction, uint8_t* bytes) {

    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(uint8_t, get_rm_form_byte)
        TYPE_DATA_DBG(Instruction_info *, "Instruction = %p")
        TYPE_DATA_DBG(uint8_t, "bytes = %02x")
        END_TYPE_FUNC_DBG,
        Instruction, bytes);

    if (Instruction == NULL) return -1;
    uint8_t byte = *((bytes + 1) - Instruction->position_rm); // obtiene el byte donde esta el campo mod
    Instruction->instruction.Mod_rm.R_M =  ((byte & (uint8_t)Instruction->mask_rm)) >> count_get_mask(Instruction->mask_rm); // obtiene los bits del campo mod
    return Instruction->instruction.Mod_rm.R_M;
}

uint8_t get_mod_form_byte(Instruction_info *Instruction, uint8_t* bytes) {

    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(uint8_t, get_mod_form_byte)
        TYPE_DATA_DBG(Instruction_info *, "Instruction = %p")
        TYPE_DATA_DBG(uint8_t, "bytes = %p")
        END_TYPE_FUNC_DBG,
        Instruction, bytes);

    if (Instruction == NULL) return -1;
    uint8_t byte = *((bytes + 1) - Instruction->position_mod); // obtiene el byte donde esta el campo mod
    Instruction->instruction.Mod_rm.mod =  ((byte & (uint8_t)Instruction->mask_mod)) >> count_get_mask(Instruction->mask_mod); // obtiene los bits del campo mod
    return Instruction->instruction.Mod_rm.mod;
}

uint8_t get_registers_form_byte(Instruction_info *Instruction, uint8_t* bytes) {
    /*
     *
     * get_registers_form_byte(Instruction_info *Instruction, uint8_t* bytes):
     * Se usa para obtener los registros desde "bytes" y obetenerlos segun una estructura Instruction_info(mask).
     *  Los mismos valores son almacenados en la estructura de entrada.
     * 
     * Se recibe un Instruction el cual contiene informacion acerca de la indtruccion a descodificar, la ubicacion de los registros
     *  etc. En caso de que sea NULL o un puntero no valido la funcion retorna -1
     * 
     * Se espera recibir un bytes donde el primer valor sea el primer byte del opcode,
     * el segundo valor de bytes sea el segundo byte del opcode, etc...
     * 
     */

    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(uint8_t, get_registers_form_byte)
        TYPE_DATA_DBG(Instruction_info *, "Instruction = %p")
        TYPE_DATA_DBG(uint8_t, "bytes = %p")
        END_TYPE_FUNC_DBG,
        Instruction, bytes);

    if (Instruction == NULL) return -1;

    uint8_t byte = *((bytes + 1) - Instruction->position_reg); 
    #ifdef DEBUG_ENABLE
    printf("[[>> Instruction->mask_reg(%02x) count_get_mask(Instruction->mask_reg)(%02llx)", Instruction->mask_reg, count_get_mask(Instruction->mask_reg));
    Instruction->instruction.Mod_rm.reg = (byte & Instruction->mask_reg) >> count_get_mask(Instruction->mask_reg);
    printf(" [[>> byte(%02x)      Instruction->instruction.Mod_rm.reg(%02x)\n", byte, Instruction->instruction.Mod_rm.reg);
    #else
    Instruction->instruction.Mod_rm.reg = (byte & Instruction->mask_reg) >> count_get_mask(Instruction->mask_reg);
    #endif
    return Instruction->instruction.Mod_rm.reg;
}

List_instrution *format_instruccion(uint8_t *instrutions, size_t size_in_bytes, encoder_x86 encoder_val) {

    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(List_instrution *, format_instruccion)
            TYPE_DATA_DBG(uint8_t *, "instrutions = %hhu")
            TYPE_DATA_DBG(size_t, "size_in_bytes = %zu")
            TYPE_DATA_DBG(encoder_x86, "encoder_x86 = %02x")
        END_TYPE_FUNC_DBG,
        instrutions, size_in_bytes, encoder_val);

    List_instrution *list_instrution_resb = init_List_instrution();
    
    for (size_t i = 0; i <= size_in_bytes; i++) {
        
        uint8_t prefix_actual; // si es distinto de 0x00 hay prefijo
        uint8_t prefix_actual_n = 0; // cantidad de prefijos encontrados para una instruccion (maximo se soporta 4 en esta implementacion para cada  instruccion)
        uint8_t prefi[4]; // almacenar prefijos
        restart_for:
        prefix_actual = 0x00;
        switch (instrutions[i]) // buscar prefijos
        {
            case Prefix_SS: 
            case Prefix_CS: 
            case Prefix_ES: 
            case Prefix_DS: 
            case Prefix_FS: 
            case Prefix_GS: 

            case Prefix_operand_size:  
            case Prefix_addr_size:     
            case Prefix_lock:          
            case Prefix_repne_repnez:  
            case Prefix_repe_rep_repz: 
                prefix_actual = instrutions[i]; 
                //actual_node->Instruction.instruction.prefix[prefix_actual_n] = prefix_actual; 
                prefi[prefix_actual_n] = prefix_actual;
                i++; // pasar al siguiente byte si se encontro un prefijo
                prefix_actual_n++; // aumentar la cantidad de prefijos encontrados para la instruccion actual en 1. 
                // if (prefix_actual > 0b11) // error
                goto restart_for; // volver al inicio del for en busca de otro prefijo, conservando los datos de las variables
            default: break; // no se encontraron prefijos
        }

        // si se encontro un prefijo se agrega a la instruccion
        /*if (prefix_actual) {
            //actual_node->Instruction.instruction.prefix[prefix_actual_n] = prefix_actual; 
            prefi[prefix_actual_n] = prefix_actual;
            i++; // pasar al siguiente byte si se encontro un prefijo
            prefix_actual_n++; // aumentar la cantidad de prefijos encontrados para la instruccion actual en 1. 
            // if (prefix_actual > 0b11) // error
            goto restart_for; // volver al inicio del for en busca de otro prefijo, conservando los datos de las variables
        }*/

        // si no se encontro mas prefijos, se analiza el opcode
        for (size_t j = 0; j < sizeof(my_instruccion)/sizeof(Instruction_info); j++){
            /*for (uint8_t k = 0; k < my_instruccion[i].opcode_size; k++){ // recorrer cada opcode de la instruccion en busca de coincidencias (max 3 opcodes por instruccion)
                
                if (my_instruccion[i].instruction.opcode[k] & instrutions[i])
            }*/
            #ifdef DEBUG_ENABLE
            // generar un par de 3 colores para imprimir cada instruccion con un color diferente usando rgb
            unsigned int Avalue1, Avalue2, Avalue3, seed, values[] = {
                my_instruccion[i].instruction.opcode[0].opcode_byte.byte + 0x0f01, 
                my_instruccion[i].instruction.opcode[1].opcode_byte.byte + 0xa00100, 
                my_instruccion[i].instruction.opcode[2].opcode_byte.byte, 
                0xa0, 0xf1, 
                my_instruccion[i].instruction.opcode[2].opcode_byte.byte + 1 +
                my_instruccion[i].instruction.opcode[1].opcode_byte.byte +
                my_instruccion[i].instruction.opcode[0].opcode_byte.byte + 20
            };
            regenerate_keys:
                //shuffle_array(values, sizeof(values)/sizeof(int));
                seed = my_instruccion[i].instruction.opcode[2].opcode_byte.byte * 
                                    my_instruccion[i].instruction.opcode[1].opcode_byte.byte * 
                                    my_instruccion[i].instruction.opcode[0].opcode_byte.byte ;
                Avalue1 = jenkins_hash(seed,    values[0], values[1], values[2], values[3], values[4], values[5]);
                Avalue2 = jenkins_hash(Avalue1, values[0], values[1], values[2], values[3], values[4], values[5]);
                Avalue3 = jenkins_hash(Avalue2, values[0], values[1], values[2], values[3], values[4], values[5]);
                if ((Avalue1 & Avalue2 & Avalue3) == Avalue1) { 
                    values[0] ^= 0b10101010; values[1] |= values[0];
                    values[2] ^= values[1];  values[3] &= values[2];
                    values[4] |= values[3];  values[5] ^= values[4];
                    goto regenerate_keys;
                }
            DEBUG_PRINT(DEBUG_LEVEL_INFO, "#{FG:reset} >>> buscando byte.... #{BG:%d;%d;%d}0x%02x#{FG:reset}\n",(unsigned char)Avalue1, (unsigned char)Avalue2, (unsigned char)Avalue3, instrutions[i]);
            printf(">>>%02x & %02x = %02x\n",my_instruccion[j].instruction.opcode[2].opcode_byte.byte, instrutions[i], my_instruccion[j].instruction.opcode[2].opcode_byte.byte & instrutions[i]);
            #endif

            
            List_instrution *actual_node;
            //printf("2bytes; i = %d, j = %d\n", i, j);
            switch (my_instruccion[j].opcode_size)
            {
                /*
                 *
                 * Este switch case permite determinar si la instruccion del mapa de instrucciones con la que se va a comparar es de 3, 2 o 1 byte,
                 * La examinacion de los casos a de hacerse en este orden, puse si se hicera de 1, 2 a 3 bytes, los opcodes de las instrucciones
                 * de un byte pueden confundirse con los de 3 o dos bytes.
                 *
                 */
                case 0b10: // tres byte de opcode
                    #ifdef DEBUG_ENABLE
                    puts("3byte opcode");
                    #endif
                    if ((my_instruccion[j].instruction.opcode[2].opcode_byte.byte & instrutions[i]) == my_instruccion[j].instruction.opcode[2].opcode_byte.byte) {
                        actual_node = push_List_instrution(list_instrution_resb); // añadir nodo 
                        memcpy(&(actual_node->Instruction), &(my_instruccion[j]), sizeof(Instruction_info));
                        actual_node->Instruction.instruction.opcode[0].opcode_byte.byte = instrutions[i+2];
                        actual_node->Instruction.instruction.opcode[1].opcode_byte.byte = instrutions[i+1];
                        actual_node->Instruction.instruction.opcode[2].opcode_byte.byte = instrutions[i];
                        *((uint32_t*)(&actual_node->Instruction.instruction.prefix)) = *((uint32_t*)(prefi));
                        if (actual_node->Instruction.immediate_instrution) goto the_end_for; // si se trata de una instruccion inmediata no hay nada mas que analizar
                        //uint8_t a = get_mod_form_byte(&(actual_node->Instruction), instrutions + i);
                        //uint8_t b = get_rm_form_byte(&(actual_node->Instruction), instrutions + i);
                        //uint8_t c = get_registers_form_byte(&(actual_node->Instruction), instrutions + i);
                        goto the_end_for;
                    } break;
                case 0b01: // dos bytes de opcode
                    #ifdef DEBUG_ENABLE
                    printf(">>> (((%02x & %02x)[ = %02x] ==  %02x) && ",
                        my_instruccion[j].instruction.opcode[2].opcode_byte.byte, 
                        instrutions[i], 
                        my_instruccion[j].instruction.opcode[2].opcode_byte.byte & instrutions[i],
                        my_instruccion[j].instruction.opcode[2].opcode_byte.byte
                    );
                    printf("((%02x & %02x)[ = %02x] ==  %02x))) = %d\n",
                        my_instruccion[j].instruction.opcode[1].opcode_byte.byte, 
                        instrutions[i+1], 
                        my_instruccion[j].instruction.opcode[1].opcode_byte.byte & instrutions[i+1],
                        my_instruccion[j].instruction.opcode[1].opcode_byte.byte,
                        ((my_instruccion[j].instruction.opcode[2].opcode_byte.byte & instrutions[i]) == my_instruccion[j].instruction.opcode[2].opcode_byte.byte) &&
                        ((my_instruccion[j].instruction.opcode[1].opcode_byte.byte & instrutions[i+1]) == my_instruccion[j].instruction.opcode[1].opcode_byte.byte)
                    );
                    #endif
                    if (
                        (
                            ((my_instruccion[j].instruction.opcode[2].opcode_byte.byte & instrutions[i]) == my_instruccion[j].instruction.opcode[2].opcode_byte.byte) &&
                            ((my_instruccion[j].instruction.opcode[1].opcode_byte.byte & instrutions[i+1]) == my_instruccion[j].instruction.opcode[1].opcode_byte.byte)
                        ) /*|| ((
                            ((my_instruccion[j].instruction.opcode[2].opcode_byte.byte & instrutions[i]) == instrutions[i]) &&
                            ((my_instruccion[j].instruction.opcode[1].opcode_byte.byte & instrutions[i+1]) == my_instruccion[j].instruction.opcode[1].opcode_byte.byte)
                        ) && my_instruccion[j].immediate_instrution != 0b1)*/
                       ) {
                        parse_as_a_2_byte_instruction:

                        actual_node = push_List_instrution(list_instrution_resb); // añadir nodo 
                        memcpy(&(actual_node->Instruction), &(my_instruccion[j]), sizeof(Instruction_info));
                        actual_node->Instruction.instruction.opcode[2].opcode_byte.byte = instrutions[i];
                        actual_node->Instruction.instruction.opcode[1].opcode_byte.byte = instrutions[i+1];
                        *((uint32_t*)(&actual_node->Instruction.instruction.prefix)) = *((uint32_t*)(prefi));
                        
                        #ifdef DEBUG_ENABLE
                        printf("encontrada, byte1(%02x), byte2(%02x)\n", instrutions[i], instrutions[i+1]);
                        DEBUG_PRINT(DEBUG_LEVEL_INFO, "#{FG:reset}instruccion actual -> #{FG:lpurple}%s#{FG:reset} -> ", get_string_instruction_by_id(actual_node->Instruction.string));
                        printf_color("#{BG:%d;%d;%d} %s#{FG:reset} (color: %u %u %u)\n", (unsigned char)Avalue1, (unsigned char)Avalue2, (unsigned char)Avalue3,  get_string_instruction_by_id(actual_node->Instruction.string), (unsigned char)Avalue1, (unsigned char)Avalue2, (unsigned char)Avalue3);
                        #endif
                        if (
                            actual_node->Instruction.position_tttn == 0b11 && 
                            actual_node->Instruction.mask_tttn != 0x0 && 
                            instrutions[i] & actual_node->Instruction.mask_tttn
                            ) {
                            // si se indico en position_tttn que no hay bits tttn pero sin embargo mask_tttn no es 0x0
                            // en ese caso, la instruccion tiene una variante donde se usa el campo tttn de las instrucciones JCC
                            // o de salto para identificar algun tipo de variante de esta instruccion actualmente examinada.
                            // en el caso de la instruccion ADC, se usa para la variante 
                            // immediate to AL, AX, or EAX 0001 010w : immediate data
                            // donde los campos tttn seran 00tttn  : immediate data
                            // 14 10          (16 y 32bits) == adc al,  0x10
                            // 15 10 00 00 00 (32bits)      == adc eax, 0x10
                            // 66 15 10 00    (32bits)      == adc ax,  0x10 
                            //        (el prefijo 66 cambia la forma de trabajar de la cpu momentaneamente 
                            //          haciendo que trabaje en 16bits y por tanto la instruccion que 
                            //          normalmente operaria con valores de 32bits pasaria a usar 16bits)
                            //
                            // 15 10 00       (16bits)      == adc ax,  0x10 
                            //
                            // todas las instrucciones en 16 y 32bits del tipo "immediate to AL, AX, or EAX"
                            // solo usan campo W.
                            //if (actual_node->Instruction.posicion_w != 0) 
                            actual_node->Instruction.immediate_data = 0b1;
                            goto jump_because_there_is_no_mod_rm;
                        }
                        if (actual_node->Instruction.immediate_instrution) goto the_end_for; // si se trata de una instruccion inmediata no hay nada mas que analizar
                        i+=1;
                        //printf("actual_node->Instruction.number_reg %d, %02x & %02x = %02x", actual_node->Instruction.number_reg, instrutions[i], actual_node->Instruction.mask_reg, instrutions[i] & actual_node->Instruction.mask_reg);
                        /*switch (actual_node->Instruction.number_reg)
                        {
                        case 0b00: break; // 0b00 - no usa campo reg
                        case 0b01: break; // 0b01 - usa un campo reg
                        case 0b10:  // 0b10 - usa dos campos reg (rm se convierte en un campo registro)
                            // obetenemos
                            actual_node->Instruction.instruction.Mod_rm.reg = (instrutions[i] & actual_node->Instruction.mask_reg) >> count_get_mask(actual_node->Instruction.mask_reg);
                            actual_node->Instruction.instruction.Mod_rm.R_M =  (instrutions[i] & actual_node->Instruction.mask_rm) >> count_get_mask(actual_node->Instruction.mask_rm); 
                            break; // 0b10 - usa dos campos reg (rm se convierte en un campo registro)
                        default:   break; // 0b11 - no se define(error)
                        }*/
                        #ifdef DEBUG_ENABLE
                        printf(">> %02x %02x\n", 
                        actual_node->Instruction.instruction.opcode[2].opcode_byte.byte,
                        actual_node->Instruction.instruction.opcode[1].opcode_byte.byte
                        );
                        #endif
                        #ifdef DEBUG_ENABLE
                        uint8_t a = get_mod_form_byte(&(actual_node->Instruction), instrutions + i);
                        uint8_t b = get_rm_form_byte(&(actual_node->Instruction), instrutions + i);
                        uint8_t c = get_registers_form_byte(&(actual_node->Instruction), instrutions + i);
                        printf(">> mod(%02x) rm(%02x) reg(%02x), instrutions[i] = %02x\n",  a, b, c, instrutions[i] );
                        printf("[>>>>>>>>] %08x\n", *((uint32_t*)(instrutions + i)));
                        printf("popcnt_software (%02llx) R_M (%02x) w(%02x)\n", popcnt_software(get_bit_w(&(actual_node->Instruction))),
                            actual_node->Instruction.instruction.Mod_rm.R_M, get_bit_w(&(actual_node->Instruction)));
                        #else
                        //get_mod_form_byte(&(actual_node->Instruction), instrutions[i]);
                        get_mod_form_byte(&(actual_node->Instruction), instrutions + i);
                        get_rm_form_byte(&(actual_node->Instruction), instrutions + i);
                        get_registers_form_byte(&(actual_node->Instruction), instrutions + i);
                        #endif
                        
                        switch(actual_node->Instruction.instruction.Mod_rm.mod) {
                            case 0b00: // obtener un desplazamiento de 16bits o 32bits
                                if ((encoder_val == 0b0) 
                                    && (actual_node->Instruction.instruction.Mod_rm.R_M == 0b110)
                                ) { // desplazamiento de 16bits
                                    *((uint16_t*)&(actual_node->Instruction.instruction.displacement)) = *((uint16_t*)(instrutions + i + 1)); // obtener el desplazamiento de 16bits
                                    #ifdef DEBUG_ENABLE
                                    printf("[16] %04x\n", *((uint16_t*)&(actual_node->Instruction.instruction.displacement)));
                                    printf("[16.8]{0} %02x\n", actual_node->Instruction.instruction.displacement[0]);
                                    printf("[16.8]{1} %02x\n", actual_node->Instruction.instruction.displacement[1]);
                                    #endif
                                    i+=2; // avanzar 2bytes en el array de instrucciones maquina
                                } else if (encoder_val == 0b1) {  // desplazamiento de 32bits o SIB
                                    if (actual_node->Instruction.instruction.Mod_rm.R_M == 0b101) { // desplazamiento de 32bits
                                        *((uint32_t*)&(actual_node->Instruction.instruction.displacement)) = *((uint32_t*)(instrutions + i + 1)); // obtener el desplazamiento de 32bits
                                        #ifdef DEBUG_ENABLE
                                        printf("[32] %08x\n", *((uint32_t*)&(actual_node->Instruction.instruction.displacement)));
                                        printf("[32.8]{0} %02x\n", actual_node->Instruction.instruction.displacement[0]);
                                        printf("[32.8]{2} %02x\n", actual_node->Instruction.instruction.displacement[1]);
                                        printf("[32.8]{3} %02x\n", actual_node->Instruction.instruction.displacement[2]);
                                        printf("[32.8]{4} %02x\n", actual_node->Instruction.instruction.displacement[3]);
                                        #endif
                                        i+=4; // avanzar 4bytes en el array de instrucciones maquina
                                    } else if (actual_node->Instruction.instruction.Mod_rm.R_M == 0b100) { // SIB
                                        *((uint8_t*)&(actual_node->Instruction.instruction.SIB)) = instrutions[i + 1]; // obtener el SIB como si un valor de 8bits se tratase
                                        i+=1; // avanzar 1byte en el array de instrucciones maquina
                                        #ifdef DEBUG_ENABLE
                                        printf("[SIB] %08x\n", *((uint8_t*)&(actual_node->Instruction.instruction.SIB)));
                                        printf("[scale]{0} %02x\n", actual_node->Instruction.instruction.SIB.scale);
                                        printf("[index]{2} %02x\n", actual_node->Instruction.instruction.SIB.index);
                                        printf("[base]{3} %02x\n", actual_node->Instruction.instruction.SIB.base);
                                        #endif
                                    }
                                }
                                break;
                            case 0b01: // obtener un desplazamiento de 8bits en cualquiera de los casos
                                if ((encoder_val == 0b1) && (actual_node->Instruction.instruction.Mod_rm.R_M == 0b100)) { // si se esta descodificando en 32bits, se a de obtener el SIB primero si RM es 100
                                    *((uint8_t*)&(actual_node->Instruction.instruction.SIB)) = instrutions[i + 1]; // obtener el SIB como si un valor de 8bits se tratase
                                        i+=1; // avanzar 1byte en el array de instrucciones maquina
                                        #ifdef DEBUG_ENABLE
                                        printf("[SIB] %08x\n", *((uint8_t*)&(actual_node->Instruction.instruction.SIB)));
                                        printf("[scale]{0} %02x\n", actual_node->Instruction.instruction.SIB.scale);
                                        printf("[index]{2} %02x\n", actual_node->Instruction.instruction.SIB.index);
                                        printf("[base]{3} %02x\n", actual_node->Instruction.instruction.SIB.base);
                                        #endif
                                }
                                actual_node->Instruction.instruction.displacement[0] = *((instrutions + i +1)); // obtener desplazamiento de 1 byte
                                #ifdef DEBUG_ENABLE
                                printf("[8] %02x\n", *((instrutions + i +1)));
                                printf("[8.8]{0} %02x\n", actual_node->Instruction.instruction.displacement[0]);
                                #endif
                                i++; // avanzar en un byte pues se obtuvo es desplazamiento
                                break;
                            case 0b10: // obtener un desplazamiento de 16bits o 32bits
                                if (encoder_val == 0b0) { // desplazamiento de 16bits
                                    *((uint16_t*)&(actual_node->Instruction.instruction.displacement)) = *((uint16_t*)(instrutions + i + 1)); // obtener el desplazamiento de 16bits
                                    #ifdef DEBUG_ENABLE
                                    printf("[16] %04x\n", *((uint16_t*)&(actual_node->Instruction.instruction.displacement)));
                                    printf("[16.8]{0} %02x\n", actual_node->Instruction.instruction.displacement[0]);
                                    printf("[16.8]{1} %02x\n", actual_node->Instruction.instruction.displacement[1]);
                                    #endif
                                    i+=2; // avanzar 2bytes en el array de instrucciones maquina
                                } else if (encoder_val == 0b1) { // desplazamiento de 32bits
                                    if (actual_node->Instruction.instruction.Mod_rm.R_M == 0b100) { // si se esta descodificando en 32bits, se a de obtener el SIB primero si RM es 100
                                        *((uint8_t*)&(actual_node->Instruction.instruction.SIB)) = instrutions[i + 1]; // obtener el SIB como si un valor de 8bits se tratase
                                        i+=1; // avanzar 1byte en el array de instrucciones maquina
                                        #ifdef DEBUG_ENABLE
                                        printf("[SIB] %08x\n", *((uint8_t*)&(actual_node->Instruction.instruction.SIB)));
                                        printf("[scale]{0} %02x\n", actual_node->Instruction.instruction.SIB.scale);
                                        printf("[index]{2} %02x\n", actual_node->Instruction.instruction.SIB.index);
                                        printf("[base]{3} %02x\n", actual_node->Instruction.instruction.SIB.base);
                                        #endif
                                    }
                                    *((uint32_t*)&(actual_node->Instruction.instruction.displacement)) = *((uint32_t*)(instrutions + i + 1)); // obtener el desplazamiento de 32bits
                                    #ifdef DEBUG_ENABLE
                                    printf("[32] %08x\n", *((uint32_t*)&(actual_node->Instruction.instruction.displacement)));
                                    printf("[32.8]{0} %02x\n", actual_node->Instruction.instruction.displacement[0]);
                                    printf("[32.8]{2} %02x\n", actual_node->Instruction.instruction.displacement[1]);
                                    printf("[32.8]{3} %02x\n", actual_node->Instruction.instruction.displacement[2]);
                                    printf("[32.8]{4} %02x\n", actual_node->Instruction.instruction.displacement[3]);
                                    #endif
                                    i+=4; // avanzar 4bytes en el array de instrucciones maquina
                                }
                                break;
                            default: break; // para 0b11 no de obtiene desplazamientos
                        }

                        // Solo se salta aqui si la instruccion no tiene mod/rm
                        jump_because_there_is_no_mod_rm:

                        // este apartado se usa para obtener los datos de instrucciones con SIB o los datos de instrucciones que van a guardar
                        // valores en registro o memoria. Por ejemplo la instruccion adc siguiente:
                        // immediate to register 1000 00sw : 11 010 R/M(reg en este caso) : immediate data
                        // no hace uso de SIB, sin invergo tiene datos inmediatos. En este caso el campo R/M
                        // para a ser un campo reg, quedando reg2 invalidado como 010.
                        // si la instruccion tuviera el campo w activo en este caso y se descodificara en 32bits, el registro seria de 32bits
                        // segun el campo W, mientras que estando el campo S activo, indicara que se extendara el signo y valor a 32bits estando
                        // W = 1
                        printf("immediate_data: %x\n", actual_node->Instruction.immediate_data);
                        if (actual_node->Instruction.immediate_data == 0b1) {
                            #ifdef DEBUG_ENABLE
                            printf("Obteniendo datos inmediatos para la instruccion");
                            printf("bit w: %x\n", get_bit_w(&(actual_node->Instruction)));
                            #endif
                            if (get_bit_w(&(actual_node->Instruction)) == 0b1) { // para 16 y 32bits
                                if (encoder_val == 0b0) { // inmediato de 16bits
                                    if (get_bit_s(&(actual_node->Instruction))) {
                                        *((uint8_t*)&(actual_node->Instruction.instruction.immediate)) = *((uint8_t*)(instrutions + i + 1));
                                        i+=1;
                                        #ifdef DEBUG_ENABLE
                                        printf("Inmediato de 8bits en 16bits %02x\n", *((uint8_t*)&(actual_node->Instruction.instruction.immediate)));
                                        #endif
                                    } else {
                                        *((uint16_t*)&(actual_node->Instruction.instruction.immediate)) = *((uint16_t*)(instrutions + i + 1));
                                        i+=2;
                                        #ifdef DEBUG_ENABLE
                                        printf("Inmediato de 16bits %04x\n", *((uint16_t*)&(actual_node->Instruction.instruction.immediate)));
                                        #endif
                                    }
                                } else {   // para 32bits
                                    if (actual_node->Instruction.position_tttn == 0b11 && actual_node->Instruction.mask_tttn != 0x0){
                                        // solo para variantes de instrucciones de tipo no salto (JCC)
                                        *((uint32_t*)&(actual_node->Instruction.instruction.immediate)) = *((uint32_t*)(instrutions + i + 1));
                                        i+=4;
                                        #ifdef DEBUG_ENABLE
                                        printf("Inmediato de 32bits para inmediato de registro AL, AX, EAX: %08x\n", *((uint32_t*)&(actual_node->Instruction.instruction.immediate)));
                                        #endif
                                    } else if (actual_node->Instruction.instruction.Mod_rm.mod == 0b11) { // para no SIB
                                        // para teoricamente cualqueir caso que no incluya SIB, para eso igual a un MOD 0b11 o 
                                        // diferente a un Mod(00) R/M(100) o Mod(01) R/M(100) o Mod(10) R/M(100)
                                        #ifdef DEBUG_ENABLE
                                        printf("bit s: %x\n", get_bit_s(&(actual_node->Instruction)));
                                        #endif
                                        // si el bit "w" esta activado  y el "s" desactivado, el valor a almacenar es de 32bits
                                        if (get_bit_s(&(actual_node->Instruction)) == 0b1) {
                                            *((uint8_t*)&(actual_node->Instruction.instruction.immediate)) = *((uint8_t*)(instrutions + i + 1));
                                            i+=1;
                                            #ifdef DEBUG_ENABLE
                                            printf("Inmediato de 8bits sin SIB %02x\n", *((uint8_t*)&(actual_node->Instruction.instruction.immediate)));
                                            #endif
                                        } else {
                                            *((uint32_t*)&(actual_node->Instruction.instruction.immediate)) = *((uint32_t*)(instrutions + i + 1));
                                            i+=4;
                                            #ifdef DEBUG_ENABLE
                                            printf("Inmediato de 32bits sin SIB %08x\n", *((uint32_t*)&(actual_node->Instruction.instruction.immediate)));
                                            #endif
                                        }
                                    } else { // con SIB 32bits
                                        // inmediato de 32bits si es SIB
                                        // para instrucciones SIB con inmediatos, solo se puede usar valores de 8 y 32bits
                                        *((uint32_t*)&(actual_node->Instruction.instruction.immediate)) = *((uint32_t*)(instrutions + i + 1));
                                        i+=4;
                                        #ifdef DEBUG_ENABLE
                                        printf("Inmediato de 32bits para SIB %08x\n", *((uint32_t*)&(actual_node->Instruction.instruction.immediate)));
                                        #endif
                                    }
                                }
                            } else { // para las instruccion con datos inmediatos de 8bits
                                *((uint8_t*)&(actual_node->Instruction.instruction.immediate)) = *((uint8_t*)(instrutions + i + 1));
                                i+=1;
                                #ifdef DEBUG_ENABLE
                                printf("Inmediato de 8bits %02x\n", *((uint8_t*)&(actual_node->Instruction.instruction.immediate)));
                                #endif
                            }
                        }
                        goto the_end_for;
                    } break; // seguir buscando la instruccion en el mapa de instrucciones
                case 0b00: // un byte de opcode
                    #ifdef DEBUG_ENABLE
                    puts("1byte opcode");
                    DEBUG_PRINT(DEBUG_LEVEL_INFO, 
                        "#{FG:reset}[Opcode 1byte] #{FG:lpurple}my_instruccion[j].instruction.opcode[1].opcode_byte.byte( = %02x) == 0b11111111 #{FG:reset}\n",
                            my_instruccion[j].instruction.opcode[1].opcode_byte.byte);
                    DEBUG_PRINT(DEBUG_LEVEL_INFO, 
                        "#{FG:reset}[Opcode 1byte] #{FG:lpurple}instrutions[i] (= %02x) <= 0b00000011#{FG:reset}\n", 
                        instrutions[i]);
                    #endif
                    if (
                        (my_instruccion[j].instruction.opcode[1].opcode_byte.byte == 0b11111111) &&
                        (instrutions[i] <= 0b00000101) // && ((instrutions[i] & 0b00000011) != 0b00000000)
                        /* la instruccion add usa el valor 0b11111111 en su segundo valor de opcode
                         * para indicar que usa una forma distinta para identificarse, pues el valor
                         * 0x00 que puede usar dicha instruccion, no puede ser identificada via AND, como
                         * se hizo con otras instruccion como ADC, por tanto necesitamos saber si la instruccion
                         * a anlizar esta entre el rango de 0x0 a 0x3(0b00000011) definido en el manual de intel:
                         *      register1 to register2             0000 000w : 11 reg1 reg2
                         *      register2 to register1             0000 001w : 11 reg1 reg2
                         *      memory to register                 0000 001w : mod reg r/m
                         *      register to memory                 0000 000w : mod reg r/m
                         * 
                         * Tambien se decidio usar la misma solucion para estas variante:
                         *      immediate to AL, AX, or EAX        0000 010w : immediate data
                         * 
                         * Pues solo abria que verificar tambien si el valor 0x4(0b00000100) y el 0x5 adicionalmente, por tanto el
                         * comprobar el rango 0x0 a 0x5(0b00000101) abrcara las 5 variantes de instrucciones
                         */
                        // tambien se i
                    ) {
                        // si se indico que la instruccion solo tiene un byte de opcode pero el segundo byte
                        // o tercero de opcode, no son 0x0, entonces, se esta usando estos campos para procesar
                        // otras instrucciones de manera distintas.

                        // solo para instrucciones ADD
                        if (instrutions[i] <= 0b00000101) { 
                            DEBUG_PRINT(DEBUG_LEVEL_INFO, "#{FG:reset}excepcion ADD encontrada(instrutions[i] < 0b00000011) #{FG:lpurple}%02x#{FG:reset}\n", instrutions[i]);
                            
                            goto parse_as_a_2_byte_instruction;
                        }

                    } 
                    else if ( // las instrucciones de un byte de opcode (no tiene mod/rm ni reg) no necesitan realizar la operacion and
                        (instrutions[i] == my_instruccion[j].instruction.opcode[2].opcode_byte.byte)
                    ) {
                        actual_node = push_List_instrution(list_instrution_resb);// añadir nodo 
                        memcpy(&(actual_node->Instruction), &(my_instruccion[j]), sizeof(Instruction_info));
                        actual_node->Instruction.instruction.opcode[2].opcode_byte.byte = instrutions[i];
                        *((uint32_t*)(&actual_node->Instruction.instruction.prefix)) = *((uint32_t*)(prefi));
                        #ifdef DEBUG_ENABLE
                        DEBUG_PRINT(DEBUG_LEVEL_INFO, "#{FG:reset}instruccion actual -> #{FG:lpurple}%s#{FG:reset} -> ", get_string_instruction_by_id(actual_node->Instruction.string));
                        printf_color("#{BG:%d;%d;%d} %s#{FG:reset} (color: %u %u %u)\n", (unsigned char)Avalue1, (unsigned char)Avalue2, (unsigned char)Avalue3,  get_string_instruction_by_id(actual_node->Instruction.string), (unsigned char)Avalue1, (unsigned char)Avalue2, (unsigned char)Avalue3);
                        printf("encontrada, byte1(%02x)\n", instrutions[i]);
                        #endif
                        if (actual_node->Instruction.immediate_instrution) goto the_end_for; // si se trata de una instruccion inmediata no hay nada mas que analizar
                        get_mod_form_byte(&(actual_node->Instruction), instrutions + i);
                        get_rm_form_byte(&(actual_node->Instruction), instrutions + i);
                        get_registers_form_byte(&(actual_node->Instruction), instrutions + i);
                        goto the_end_for; // dejar de buscar instrucciones en el mapa de instrucciones

                    } 
                    /*if (pop_nodo_List_instrution(list_instrution_resb, actual_node) == NULL) {
                        puts("pop_nodo_List_instrution - Error");
                        exit(1); // liberar el nodo actual si la instruccion no es encontrada
                    }*/
                    break; // seguir buscando la instruccion en el mapa de instrucciones
                default: 
                    puts("ERRRRRRRRoR");
                    exit(0);
                    break; // error 0b11 se define como no existe
            }
        }
        the_end_for: {} // el compilador me obliga a poner algun codigo por ser el final del estamento, sino da error :'v
    }
    
    return pop_List_instrution(list_instrution_resb, 0); // eliminar el primer bloque, pues se incio vacio
}

#endif