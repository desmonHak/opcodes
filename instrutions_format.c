#ifndef __INSTRUTTIONS_FORMAT_C__
#define __INSTRUTTIONS_FORMAT_C__

#include "instrutions_format.h"

List_instrution* init_List_instrution() {
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(List_instrution*, init_List_instrution)
            END_TYPE_FUNC_DBG);
    #endif
    List_instrution *list_instrution_resb = (List_instrution *)malloc(sizeof(List_instrution));
    *list_instrution_resb = (List_instrution) {
        .id = 0,
        .next_list_instrution = NULL,
        .Instruction = {
            .immediate_instrution = 0b0,  
            .opcode_size          = 0b00, 
            .instruction = { 
                .prefix = { 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
                .opcode = {
                    (opcode){ .opcode_byte.byte = 0b00000000 }, 
                    (opcode){ .opcode_byte.byte = 0b00000000 },
                    (opcode){ .opcode_byte.byte = 0b00000000 }, 
                },
                .Mod_rm = (Mod_rm){   .mod = 0b00,   .reg = 0b000,  .R_M = 0b000 },
                .SIB    =    (SIB){ .scale = 0b00, .index = 0b000, .base = 0b000 },
                .displacement = { 0b00000000, 0b00000000, 0b00000000, 0b00000000},
                .immediate    = { 0b00000000, 0b00000000, 0b00000000, 0b00000000}
            }
        }
    }; return list_instrution_resb;
}


size_t get_number_instrutions(uint8_t *instrutions, size_t size_in_bytes) {

}

void pop_List_instrution(List_instrution *list_instrution){
    /*
     *
     * Se recibe un list_instrution el cual es la lista enlazada, position = -1 elimina el ultimo elemento,
     * se puede especificar la posicion a eliminar con position
     * 
     */
}

List_instrution *push_List_instrution(List_instrution *list_instrution) {
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(List_instrution *, push_List_instrution)
                TYPE_DATA_DBG(List_instrution *, "list_instrution = %p")
            END_TYPE_FUNC_DBG,
            list_instrution);
    #endif
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

void print_List_instrution(List_instrution *list_instrution) {
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(void, print_List_instrution)
                TYPE_DATA_DBG(List_instrution *, "list_instrution = %p")
            END_TYPE_FUNC_DBG,
            list_instrution);
    #endif
    if (list_instrution == NULL) printf("finall list or invalid...");

    for (List_instrution *i = list_instrution; i->next_list_instrution != NULL; i = i->next_list_instrution) {
        printf("list_instrution[%d] = %p\n", i->id, i);
        print_instruccion_binary(&(i->Instruction.instruction));
        print_instruccion(&(i->Instruction.instruction));
        printf("list_instrution->next_list_instrution = %p\n\n", i->next_list_instrution);
    }
}

List_instrution *format_instruccion(uint8_t *instrutions, size_t size_in_bytes) {

    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(List_instrution *, format_instruccion)
                TYPE_DATA_DBG(uint8_t *, "instrutions = %hhu")
                TYPE_DATA_DBG(size_t, "size_in_bytes = %zu")
            END_TYPE_FUNC_DBG,
            instrutions, size_in_bytes);
    #endif
    List_instrution *list_instrution_resb = init_List_instrution();
    
    for (size_t i = 0; i < size_in_bytes; i++) {
        List_instrution *actual_node = push_List_instrution(list_instrution_resb);
        uint8_t prefix_actual; // si es distinto de 0x00 hay prefijo
        uint8_t prefix_actual_n = 0; // cantidad de prefijos encontrados para una instruccion (maximo se soporta 4 en esta implementacion para cada  instruccion)

        restart_for:
        prefix_actual = 0x00;
        switch (instrutions[i]) // buscar prefijos
        {
            case Prefix_SS: prefix_actual = Prefix_SS; break;
            case Prefix_CS: prefix_actual = Prefix_CS; break;
            case Prefix_ES: prefix_actual = Prefix_ES; break;
            case Prefix_DS: prefix_actual = Prefix_DS; break;
            case Prefix_FS: prefix_actual = Prefix_FS; break;
            case Prefix_GS: prefix_actual = Prefix_GS; break;

            case Prefix_operand_size:  prefix_actual = Prefix_operand_size; break;
            case Prefix_addr_size:     prefix_actual = Prefix_addr_size; break;
            case Prefix_lock:          prefix_actual = Prefix_lock; break;
            case Prefix_repne_repnez:  prefix_actual = Prefix_repne_repnez; break;
            case Prefix_repe_rep_repz: prefix_actual = Prefix_repe_rep_repz; break;
        }
        
        // si se encontro un prefijo se agrega a la instruccion
        if (prefix_actual) {
            actual_node->Instruction.instruction.prefix[prefix_actual_n] = prefix_actual; 
            i++; // pasar al siguiente byte si se encontro un prefijo
            prefix_actual_n++; // aumentar la cantidad de prefijos encontrados para la instruccion actual en 1. 
            // if (prefix_actual > 0b11) // error
            goto restart_for; // volver al inicio del for en busca de otro prefijo, conservando los datos de las variables
        }

        // si no se encontro mas prefijos, se analiza el opcode
        for (size_t j = 0; j < sizeof(my_instruccion)/sizeof(Instruction_info); j++){
            /*for (uint8_t k = 0; k < my_instruccion[i].opcode_size; k++){ // recorrer cada opcode de la instruccion en busca de coincidencias (max 3 opcodes por instruccion)
                
                if (my_instruccion[i].instruction.opcode[k] & instrutions[i])
            }*/
            unsigned int Avalue1, Avalue2, Avalue3; // generar un par de 3 colores para imprimir cada instruccion con un color diferente usando rgb
            int values[] = {
                my_instruccion[j].instruction.opcode[0].opcode_byte.byte + 0x0f00, 
                my_instruccion[j].instruction.opcode[1].opcode_byte.byte + 0xf0010a0, 
                my_instruccion[j].instruction.opcode[2].opcode_byte.byte, 
                0xa0, 0xf1, 
                my_instruccion[j].instruction.opcode[2].opcode_byte.byte + 1 +
                my_instruccion[j].instruction.opcode[1].opcode_byte.byte +
                my_instruccion[j].instruction.opcode[0].opcode_byte.byte + 20
            };
            //shuffle_array(values, sizeof(values)/sizeof(int));
            unsigned int seed = my_instruccion[j].instruction.opcode[2].opcode_byte.byte * 
                                my_instruccion[j].instruction.opcode[1].opcode_byte.byte * 
                                my_instruccion[j].instruction.opcode[0].opcode_byte.byte ;
            Avalue1 = jenkins_hash(seed,    values[0], values[1], values[3], values[4], values[5], values[6]);
            Avalue2 = jenkins_hash(Avalue1, values[0], values[1], values[3], values[4], values[5], values[6]);
            Avalue3 = jenkins_hash(Avalue2, values[0], values[1], values[3], values[4], values[5], values[6]);
            #ifdef DEBUG_ENABLE
            printf(">>> buscando.... %02x\n ", instrutions[i]);
            #endif
            //printf("2bytes; i = %d, j = %d\n", i, j);
            switch (my_instruccion[j].opcode_size)
            {
                case 0b00: // un byte de opcode
                    //puts("1bytes");
                    
                    if (my_instruccion[j].instruction.opcode[2].opcode_byte.byte & instrutions[i] == my_instruccion[j].instruction.opcode[2].opcode_byte.byte) {
                        actual_node->Instruction.instruction.opcode[2].opcode_byte = my_instruccion[i].instruction.opcode[2].opcode_byte;
                        actual_node->Instruction.immediate_instrution = my_instruccion[j].immediate_instrution;
                        actual_node->Instruction.opcode_size = my_instruccion[j].opcode_size;
                        actual_node->Instruction.string = my_instruccion[j].string;
                        #ifdef DEBUG_ENABLE
                        printf_color("instruccion actual -> #{FG:lcyan}%s#{FG:reset} -> ", get_string_instrution(actual_node->Instruction.string));
                        printf_color("#{BG:%d;%d;%d} %s#{FG:reset} (color: %u %u %u)\n", (unsigned char)Avalue1, (unsigned char)Avalue2, (unsigned char)Avalue3,  get_string_instrution(actual_node->Instruction.string), (unsigned char)Avalue1, (unsigned char)Avalue2, (unsigned char)Avalue3);
                        #endif
                        if (actual_node->Instruction.immediate_instrution) goto the_end_for; // si se trata de una instruccion inmediata no hay nada mas que analizar
                    } break;
                case 0b01: // dos bytes de opcode
                    /*printf("( %x & %x = %x ) && ( %x & %x = %x )\n", 
                    my_instruccion[j].instruction.opcode[2].opcode_byte.byte, instrutions[i], my_instruccion[j].instruction.opcode[2].opcode_byte.byte & instrutions[i],
                    my_instruccion[j].instruction.opcode[1].opcode_byte.byte, instrutions[i+1], my_instruccion[j].instruction.opcode[1].opcode_byte.byte & instrutions[i+1]);*/
                    if (
                        ((my_instruccion[j].instruction.opcode[2].opcode_byte.byte & instrutions[i]) == my_instruccion[j].instruction.opcode[2].opcode_byte.byte) &&
                        ((my_instruccion[j].instruction.opcode[1].opcode_byte.byte & instrutions[i+1]) == my_instruccion[j].instruction.opcode[1].opcode_byte.byte)
                       ) {
                        actual_node->Instruction.instruction.opcode[2].opcode_byte = my_instruccion[i].instruction.opcode[2].opcode_byte;
                        actual_node->Instruction.instruction.opcode[1].opcode_byte = my_instruccion[i+1].instruction.opcode[1].opcode_byte;
                        actual_node->Instruction.immediate_instrution = my_instruccion[j].immediate_instrution;
                        actual_node->Instruction.opcode_size = my_instruccion[j].opcode_size; 
                        actual_node->Instruction.string = my_instruccion[j].string; i+=1;
                        #ifdef DEBUG_ENABLE
                        printf_color("instruccion actual -> #{FG:lpurple}%s#{FG:reset} -> ", get_string_instrution(actual_node->Instruction.string));
                        printf_color("#{BG:%d;%d;%d} %s#{FG:reset} (color: %u %u %u)\n", (unsigned char)Avalue1, (unsigned char)Avalue2, (unsigned char)Avalue3,  get_string_instrution(actual_node->Instruction.string), (unsigned char)Avalue1, (unsigned char)Avalue2, (unsigned char)Avalue3);
                        #endif
                        if (actual_node->Instruction.immediate_instrution) goto the_end_for; // si se trata de una instruccion inmediata no hay nada mas que analizar
                    } 
                    break;
                case 0b10: // tres byte de opcode
                    if (my_instruccion[j].instruction.opcode[2].opcode_byte.byte & instrutions[i] == my_instruccion[j].instruction.opcode[2].opcode_byte.byte) {
                        actual_node->Instruction.instruction.opcode[2].opcode_byte = my_instruccion[i].instruction.opcode[2].opcode_byte;
                        actual_node->Instruction.immediate_instrution = my_instruccion[j].immediate_instrution;
                        actual_node->Instruction.opcode_size = my_instruccion[j].opcode_size;
                        if (actual_node->Instruction.immediate_instrution) goto the_end_for; // si se trata de una instruccion inmediata no hay nada mas que analizar
                    } break;
                default: break; // error 0b11 se define como no existe
            }
        }
        the_end_for: {} // el compilador me obliga a poner algun codigo por ser el final del estamento, sino da error :'v
    }
    print_List_instrution(list_instrution_resb);
    return list_instrution_resb;
}

#endif