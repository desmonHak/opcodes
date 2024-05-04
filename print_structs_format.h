#ifndef __PRINT_STRUCTS_FORMAT_H_
#define __PRINT_STRUCTS_FORMAT_H_

#include <stdio.h>
#include <stdint.h>

#include "opcodes_prefix.h"
#include "colors-C-C-plus-plus\colors.h"

#define WORD_SIZE 256
#define BLOCK_SLICES (256*4)/8
#define COLOR_WITH_NUMBER(strig_color, num) "#{FG:"#strig_color"}"#num"#{FG:reset}"

void print_binary(unsigned int num, uint16_t num_bits, uint16_t init_count);
//void print_binary(unsigned int num, int num_bits);
void print_opcode(Instruction_info *my_instruccion_, opcode opcode, uint8_t number_opcode);
void print_instruccion_binary(Instruction *my_instruccion);
void print_instruccion(Instruction_info *my_instruccion, encoder_x86 encoder_val);

#include "print_structs_format.c"
#endif