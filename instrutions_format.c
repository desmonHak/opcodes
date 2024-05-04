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


void print_List_instrution(List_instrution *list_instrution, encoder_x86 encoder_val) {
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
        print_instruccion(&(i->Instruction), encoder_val);
        print_instruccion_binary(&(i->Instruction.instruction));
        printf("list_instrution->next_list_instrution = %p\n\n", i->next_list_instrution);
    }
}

void copy_info_instruction(Instruction_info *instrutions, Instruction_info *my_instruccion, encoder_x86 encoder_val){
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(List_instrution *, copy_info_instruction)
                TYPE_DATA_DBG(Instruction_info *, "instrutions = %p")
                TYPE_DATA_DBG(Instruction_info *, "my_instruccion = %p")
                TYPE_DATA_DBG(encoder_x86, "encoder_val = %u")
            END_TYPE_FUNC_DBG,
            instrutions, my_instruccion, encoder_val);
    #endif
    instrutions->instruction.opcode[2].opcode_byte.byte = my_instruccion->instruction.opcode[2].opcode_byte.byte; // opcode priamrio
    instrutions->instruction.opcode[1].opcode_byte.byte = my_instruccion->instruction.opcode[1].opcode_byte.byte;
    instrutions->instruction.opcode[0].opcode_byte.byte = my_instruccion->instruction.opcode[0].opcode_byte.byte;
    
    instrutions->immediate_instrution = my_instruccion->immediate_instrution;
    instrutions->opcode_size          = my_instruccion->opcode_size; 
    instrutions->string               = my_instruccion->string;
    instrutions->number_reg           = my_instruccion->number_reg;
    instrutions->mask_reg             = my_instruccion->mask_reg;
    instrutions->mask_rm              = my_instruccion->mask_rm;
    instrutions->posicion_w           = my_instruccion->posicion_w;
    instrutions->posicion_d           = my_instruccion->posicion_d;
    instrutions->posicion_s           = my_instruccion->posicion_s;
}

uint8_t get_registers_form_byte(Instruction_info *Instruction, uint8_t byte) {
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(uint8_t, get_registers_form_byte)
            TYPE_DATA_DBG(Instruction_info *, "Instruction = %p")
            TYPE_DATA_DBG(uint8_t, "byte = %02x")
            END_TYPE_FUNC_DBG,
            Instruction, byte);
    #endif
    switch (Instruction->number_reg)
    {
        case 0b00: break; // 0b00 - no usa campo reg
        case 0b01: break; // 0b01 - usa un campo reg
        case 0b10:  // 0b10 - usa dos campos reg (rm se convierte en un campo registro)
            // obetenemos
            Instruction->instruction.Mod_rm.reg = (byte & Instruction->mask_reg) >> count_get_mask(Instruction->mask_reg);
            Instruction->instruction.Mod_rm.R_M =  (byte & Instruction->mask_rm) >> count_get_mask(Instruction->mask_rm); 
            break; // 0b10 - usa dos campos reg (rm se convierte en un campo registro)
        default:   break; // 0b11 - no se define(error)
    }
    return Instruction->number_reg;
}

List_instrution *format_instruccion(uint8_t *instrutions, size_t size_in_bytes, encoder_x86 encoder_val) {

    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(List_instrution *, format_instruccion)
                TYPE_DATA_DBG(uint8_t *, "instrutions = %hhu")
                TYPE_DATA_DBG(size_t, "size_in_bytes = %zu")
                TYPE_DATA_DBG(encoder_x86, "encoder_x86 = %02x")
            END_TYPE_FUNC_DBG,
            instrutions, size_in_bytes, encoder_val);
    #endif
    List_instrution *list_instrution_resb = init_List_instrution();
    
    for (size_t i = 0; i <= size_in_bytes; i++) {
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
            #endif

            //printf("2bytes; i = %d, j = %d\n", i, j);
            switch (my_instruccion[j].opcode_size)
            {
                case 0b00: // un byte de opcode
                    if (my_instruccion[j].instruction.opcode[2].opcode_byte.byte & instrutions[i] == my_instruccion[j].instruction.opcode[2].opcode_byte.byte) {
                        copy_info_instruction(&(actual_node->Instruction), &(my_instruccion[j]), encoder_val);
                        actual_node->Instruction.instruction.opcode[2].opcode_byte.byte = instrutions[i];
                        #ifdef DEBUG_ENABLE
                        DEBUG_PRINT(DEBUG_LEVEL_INFO, "#{FG:reset}instruccion actual -> #{FG:lpurple}%s#{FG:reset} -> ", get_string_instrution(actual_node->Instruction.string));
                        printf_color("#{BG:%d;%d;%d} %s#{FG:reset} (color: %u %u %u)\n", (unsigned char)Avalue1, (unsigned char)Avalue2, (unsigned char)Avalue3,  get_string_instrution(actual_node->Instruction.string), (unsigned char)Avalue1, (unsigned char)Avalue2, (unsigned char)Avalue3);
                        #endif
                        if (actual_node->Instruction.immediate_instrution) goto the_end_for; // si se trata de una instruccion inmediata no hay nada mas que analizar
                        get_registers_form_byte(&(actual_node->Instruction), instrutions[i]);
                    } break;
                case 0b01: // dos bytes de opcode
                    if (
                        ((my_instruccion[j].instruction.opcode[2].opcode_byte.byte & instrutions[i]) == my_instruccion[j].instruction.opcode[2].opcode_byte.byte) &&
                        ((my_instruccion[j].instruction.opcode[1].opcode_byte.byte & instrutions[i+1]) == my_instruccion[j].instruction.opcode[1].opcode_byte.byte)
                       ) {
                        //actual_node->Instruction.instruction.opcode[2].opcode_byte.byte = my_instruccion[j].instruction.opcode[2].opcode_byte.byte;
                        copy_info_instruction(&(actual_node->Instruction), &(my_instruccion[j]), encoder_val);
                        actual_node->Instruction.instruction.opcode[2].opcode_byte.byte = instrutions[i];
                        actual_node->Instruction.instruction.opcode[1].opcode_byte.byte = instrutions[i+1];
                        i+=1;
                        #ifdef DEBUG_ENABLE
                        DEBUG_PRINT(DEBUG_LEVEL_INFO, "#{FG:reset}instruccion actual -> #{FG:lpurple}%s#{FG:reset} -> ", get_string_instrution(actual_node->Instruction.string));
                        printf_color("#{BG:%d;%d;%d} %s#{FG:reset} (color: %u %u %u)\n", (unsigned char)Avalue1, (unsigned char)Avalue2, (unsigned char)Avalue3,  get_string_instrution(actual_node->Instruction.string), (unsigned char)Avalue1, (unsigned char)Avalue2, (unsigned char)Avalue3);
                        #endif
                        if (actual_node->Instruction.immediate_instrution) goto the_end_for; // si se trata de una instruccion inmediata no hay nada mas que analizar
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
                        get_registers_form_byte(&(actual_node->Instruction), instrutions[i]);
                    } break;
                case 0b10: // tres byte de opcode
                    if (my_instruccion[j].instruction.opcode[2].opcode_byte.byte & instrutions[i] == my_instruccion[j].instruction.opcode[2].opcode_byte.byte) {
                        copy_info_instruction(&(actual_node->Instruction), &(my_instruccion[j]), encoder_val);
                        actual_node->Instruction.instruction.opcode[2].opcode_byte.byte = instrutions[i];
                        actual_node->Instruction.instruction.opcode[1].opcode_byte.byte = instrutions[i+1];
                        actual_node->Instruction.instruction.opcode[0].opcode_byte.byte = instrutions[i+2];
                        if (actual_node->Instruction.immediate_instrution) goto the_end_for; // si se trata de una instruccion inmediata no hay nada mas que analizar
                        get_registers_form_byte(&(actual_node->Instruction), instrutions[i]);
                    } break;
                default: break; // error 0b11 se define como no existe
            }
        }
        the_end_for: {} // el compilador me obliga a poner algun codigo por ser el final del estamento, sino da error :'v
    }
    print_List_instrution(list_instrution_resb, encoder_val);
    return list_instrution_resb;
}

#endif