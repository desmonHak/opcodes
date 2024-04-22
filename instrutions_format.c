#ifndef __INSTRUTTIONS_FORMAT_C__
#define __INSTRUTTIONS_FORMAT_C__

#include "instrutions_format.h"

List_instrution* init_List_instrution() {
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
    if (list_instrution == NULL) printf("finall list or invalid...");

    List_instrution *i = list_instrution;
    while (i->next_list_instrution != NULL) {
        printf("list_instrution[%d] = %p\n", i->id, i);
        print_instruccion_binary(&(i->Instruction.instruction));
        print_instruccion(&(i->Instruction.instruction));
        printf("list_instrution->next_list_instrution = %p\n\n", i->next_list_instrution);
        i = i->next_list_instrution;
        
    }

}

List_instrution *format_instruccion(uint8_t *instrutions, size_t size_in_bytes) {
    List_instrution *list_instrution_resb = init_List_instrution();
    push_List_instrution(list_instrution_resb);
    
    for (size_t i = 0; i < size_in_bytes; i++) {
        restart_for:
        switch (instrutions[i]) // buscar prefijos
        {
        case Prefix_SS:
            
            break;
        
        default:
            break;
        }
    }
    print_List_instrution(list_instrution_resb);
    return list_instrution_resb;
}

#endif