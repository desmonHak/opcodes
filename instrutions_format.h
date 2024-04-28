#ifndef __INSTRUTTIONS_FORMAT_H__
#define __INSTRUTTIONS_FORMAT_H__

#include "opcodes_prefix.h"
#include "print_structs_format.h"

#include <stdlib.h>
#include <stdint.h>

typedef struct List_instrution {
    struct List_instrution *next_list_instrution;
    Instruction_info Instruction;
    size_t id;
} List_instrution;

size_t get_number_instrutions(uint8_t *instrutions, size_t size_in_bytes);
List_instrution *format_instruccion(uint8_t *instrutions, size_t size_in_bytes, encoder_x86 encoder_val);
List_instrution *init_List_instrution();
void pop_List_instrution(List_instrution *list_instrution);
List_instrution *push_List_instrution(List_instrution *list_instrution);

#include "instrutions_format.c"
#endif