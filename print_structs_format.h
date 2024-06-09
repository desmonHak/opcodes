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
#ifndef __PRINT_STRUCTS_FORMAT_H_
#define __PRINT_STRUCTS_FORMAT_H_

#include <stdio.h>
#include <stdint.h>

#include "stringx.h"
#include "instrutions_format.h"
#include "opcodes_prefix.h"
#include "colors.h"

#define WORD_SIZE 256
#define BLOCK_SLICES (256*4)/8
#define COLOR_WITH_NUMBER(strig_color, num) "#{FG:"#strig_color"}"#num"#{FG:reset}"

char *get_addr_to_encoder_x86(uint64_t addr, encoder_x86 encoder_val);

void print_instruccion_binary(Instruction *my_instruccion);
void print_binary(unsigned int num, uint16_t num_bits, uint16_t init_count);
void print_instruccion_hex(Instruction *my_instruccion, encoder_x86 encoder_val);
void print_instruccion(Instruction_info *my_instruccion, encoder_x86 encoder_val);
void print_opcode(Instruction_info *my_instruccion_, opcode opcode, uint8_t number_opcode);
void print_table_hex(char *string_init, char *string_text_for_printing, size_t size_string_text_for_printing, encoder_x86 encoder_val);

#include "print_structs_format.c"
#endif