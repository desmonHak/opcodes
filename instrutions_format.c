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
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(List_instrution*, init_List_instrution)
            END_TYPE_FUNC_DBG);
    #endif
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
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(size_t, get_number_instrutions)
            TYPE_DATA_DBG(List_instrution *, "instrutions = %p")
            END_TYPE_FUNC_DBG,
            instrutions);
    #endif
    size_t number_instrutions = 0;
    for (List_instrution *i = instrutions; i->next_list_instrution != NULL; i = i->next_list_instrution) number_instrutions++;
    return number_instrutions;
}

List_instrution * pop_List_instrution(List_instrution *list_instrution, size_t position){
    /*
     *
     * pop_List_instrution(List_instrution *list_instrution):
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
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(List_instrution *, pop_List_instrution)
            TYPE_DATA_DBG(List_instrution *, "list_instrution = %p")
            TYPE_DATA_DBG(size_t, "position = %zu")
            END_TYPE_FUNC_DBG,
            list_instrution, position);
    #endif
    if (list_instrution == NULL) return NULL;
    if (position == 0){
        List_instrution *next_block = list_instrution->next_list_instrution;
        free(list_instrution);
        return next_block;
    }
    size_t number_instrutions = 1; // empezar en 1 como si del siguiente bloque se tratara, pues 
    // eliminar el bloque 0 se hace en la parte superior.
    List_instrution *last_block; // apuntar al ultimo bloque, para no perderle (A)
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
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(void, print_List_instrution)
                TYPE_DATA_DBG(List_instrution *, "list_instrution = %p")
            END_TYPE_FUNC_DBG,
            list_instrution);
    #endif
    if (list_instrution == NULL) printf("finall list or invalid...");

    for (List_instrution *i = list_instrution; i->next_list_instrution != NULL; i = i->next_list_instrution) {
        printf("\n\nlist_instrution->next_list_instrution = %p\n", i->next_list_instrution);
        printf("list_instrution[%d] = %p\n", i->id, i);
        print_instruccion(&(i->Instruction), encoder_val);
        //print_instruccion_binary(&(i->Instruction.instruction));
        print_instruccion_hex(&(i->Instruction.instruction), encoder_val);
        char text[] = " ";
        print_table_hex(text, (char*)&(i->Instruction), sizeof(Instruction_info), encoder_val);
        
    }
}

uint8_t get_rm_form_byte(Instruction_info *Instruction, uint8_t* bytes) {
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(uint8_t, get_rm_form_byte)
            TYPE_DATA_DBG(Instruction_info *, "Instruction = %p")
            TYPE_DATA_DBG(uint8_t, "bytes = %02x")
            END_TYPE_FUNC_DBG,
            Instruction, bytes);
    #endif
    if (Instruction == NULL) return -1;
    uint8_t byte = *((bytes + 1) - Instruction->position_rm); // obtiene el byte donde esta el campo mod
    Instruction->instruction.Mod_rm.R_M =  ((byte & (uint8_t)Instruction->mask_rm)) >> count_get_mask(Instruction->mask_rm); // obtiene los bits del campo mod
    return Instruction->instruction.Mod_rm.R_M;
}

uint8_t get_mod_form_byte(Instruction_info *Instruction, uint8_t* bytes) {
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(uint8_t, get_mod_form_byte)
            TYPE_DATA_DBG(Instruction_info *, "Instruction = %p")
            TYPE_DATA_DBG(uint8_t, "bytes = %p")
            END_TYPE_FUNC_DBG,
            Instruction, bytes);
    #endif
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
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(uint8_t, get_registers_form_byte)
            TYPE_DATA_DBG(Instruction_info *, "Instruction = %p")
            TYPE_DATA_DBG(uint8_t, "bytes = %p")
            END_TYPE_FUNC_DBG,
            Instruction, bytes);
    #endif
    if (Instruction == NULL) return -1;
    #endifz
    uint8_t byte = *((bytes + 1) - Instruction->position_reg); 
    #ifdef DEBUG_ENABLE
    printf("[[>> Instruction->mask_reg(%02x) count_get_mask(Instruction->mask_reg)(%02x)", Instruction->mask_reg, count_get_mask(Instruction->mask_reg));
    Instruction->instruction.Mod_rm.reg = (byte & Instruction->mask_reg) >> count_get_mask(Instruction->mask_reg);
    printf(" [[>> byte(%02x)      Instruction->instruction.Mod_rm.reg(%02x)\n", byte, Instruction->instruction.Mod_rm.reg);
    #endif
    return Instruction->instruction.Mod_rm.reg;
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
            printf(">>>%02x & %02x = %02x\n",my_instruccion[j].instruction.opcode[2].opcode_byte.byte, instrutions[i], my_instruccion[j].instruction.opcode[2].opcode_byte.byte & instrutions[i]);
            #endif
            //printf("2bytes; i = %d, j = %d\n", i, j);
            switch (my_instruccion[j].opcode_size)
            {
                case 0b00: // un byte de opcode
                    #ifdef DEBUG_ENABLE
                    puts("1byte opcode");
                    #endif
                    if ((my_instruccion[j].instruction.opcode[2].opcode_byte.byte & instrutions[i]) == my_instruccion[j].instruction.opcode[2].opcode_byte.byte) {
                        memcpy(&(actual_node->Instruction), &(my_instruccion[j]), sizeof(Instruction_info));
                        actual_node->Instruction.instruction.opcode[2].opcode_byte.byte = instrutions[i];
                        #ifdef DEBUG_ENABLE
                        DEBUG_PRINT(DEBUG_LEVEL_INFO, "#{FG:reset}instruccion actual -> #{FG:lpurple}%s#{FG:reset} -> ", get_string_instrution(actual_node->Instruction.string));
                        printf_color("#{BG:%d;%d;%d} %s#{FG:reset} (color: %u %u %u)\n", (unsigned char)Avalue1, (unsigned char)Avalue2, (unsigned char)Avalue3,  get_string_instrution(actual_node->Instruction.string), (unsigned char)Avalue1, (unsigned char)Avalue2, (unsigned char)Avalue3);
                        #endif
                        if (actual_node->Instruction.immediate_instrution) goto the_end_for; // si se trata de una instruccion inmediata no hay nada mas que analizar
                        uint8_t a = get_mod_form_byte(&(actual_node->Instruction), instrutions + i);
                        uint8_t b = get_rm_form_byte(&(actual_node->Instruction), instrutions + i);
                        uint8_t c = get_registers_form_byte(&(actual_node->Instruction), instrutions + i);
                        goto the_end_for;
                    } break;
                case 0b01: // dos bytes de opcode
                    #ifdef DEBUG_ENABLE
                    puts("2byte opcode");
                    printf(">>> (%02x & %02x = %02x) && ",my_instruccion[j].instruction.opcode[2].opcode_byte.byte, instrutions[i], my_instruccion[j].instruction.opcode[2].opcode_byte.byte & instrutions[i]);
                    printf("(%02x & %02x = %02x)\n",my_instruccion[j].instruction.opcode[1].opcode_byte.byte, instrutions[i+1], my_instruccion[j].instruction.opcode[1].opcode_byte.byte & instrutions[i+1]);
                    #endif
                    if (
                        ((my_instruccion[j].instruction.opcode[2].opcode_byte.byte & instrutions[i]) == my_instruccion[j].instruction.opcode[2].opcode_byte.byte) &&
                        ((my_instruccion[j].instruction.opcode[1].opcode_byte.byte & instrutions[i+1]) == my_instruccion[j].instruction.opcode[1].opcode_byte.byte)
                       ) {
                        //actual_node->Instruction.instruction.opcode[2].opcode_byte.byte = my_instruccion[j].instruction.opcode[2].opcode_byte.byte;
                        //copy_info_instruction(&(actual_node->Instruction), &(my_instruccion[j]), encoder_val);
                        memcpy(&(actual_node->Instruction), &(my_instruccion[j]), sizeof(Instruction_info));
                        actual_node->Instruction.instruction.opcode[2].opcode_byte.byte = instrutions[i];
                        actual_node->Instruction.instruction.opcode[1].opcode_byte.byte = instrutions[i+1];
                        
                        #ifdef DEBUG_ENABLE
                        printf("encontrada, byte1(%02x), byte2(%02x)\n", instrutions[i], instrutions[i+1]);
                        DEBUG_PRINT(DEBUG_LEVEL_INFO, "#{FG:reset}instruccion actual -> #{FG:lpurple}%s#{FG:reset} -> ", get_string_instrution(actual_node->Instruction.string));
                        printf_color("#{BG:%d;%d;%d} %s#{FG:reset} (color: %u %u %u)\n", (unsigned char)Avalue1, (unsigned char)Avalue2, (unsigned char)Avalue3,  get_string_instrution(actual_node->Instruction.string), (unsigned char)Avalue1, (unsigned char)Avalue2, (unsigned char)Avalue3);
                        #endif
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
                        printf(">> %02x %02x\n", 
                        actual_node->Instruction.instruction.opcode[2].opcode_byte.byte,
                        actual_node->Instruction.instruction.opcode[1].opcode_byte.byte
                        );
                        //get_mod_form_byte(&(actual_node->Instruction), instrutions[i]);
                        uint8_t a = get_mod_form_byte(&(actual_node->Instruction), instrutions + i);
                        uint8_t b = get_rm_form_byte(&(actual_node->Instruction), instrutions + i);
                        uint8_t c = get_registers_form_byte(&(actual_node->Instruction), instrutions + i);
                        printf(">> mod(%02x) rm(%02x) reg(%02x), instrutions[i] = %02x\n",  a, b, c, instrutions[i] );
                        goto the_end_for;
                    } break;
                case 0b10: // tres byte de opcode
                    #ifdef DEBUG_ENABLE
                    puts("3byte opcode");
                    #endif
                    if ((my_instruccion[j].instruction.opcode[2].opcode_byte.byte & instrutions[i]) == my_instruccion[j].instruction.opcode[2].opcode_byte.byte) {
                        //copy_info_instruction(&(actual_node->Instruction), &(my_instruccion[j]), encoder_val);
                        memcpy(&(actual_node->Instruction), &(my_instruccion[j]), sizeof(Instruction_info));
                        actual_node->Instruction.instruction.opcode[0].opcode_byte.byte = instrutions[i+2];
                        actual_node->Instruction.instruction.opcode[1].opcode_byte.byte = instrutions[i+1];
                        actual_node->Instruction.instruction.opcode[2].opcode_byte.byte = instrutions[i];
                        if (actual_node->Instruction.immediate_instrution) goto the_end_for; // si se trata de una instruccion inmediata no hay nada mas que analizar
                        uint8_t a = get_mod_form_byte(&(actual_node->Instruction), instrutions + i);
                        uint8_t b = get_rm_form_byte(&(actual_node->Instruction), instrutions + i);
                        uint8_t c = get_registers_form_byte(&(actual_node->Instruction), instrutions + i);
                        goto the_end_for;
                    } break;
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