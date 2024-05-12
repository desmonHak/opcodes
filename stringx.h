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
#ifndef __STRINGX_H__
#define __STRINGX_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "opcodes_prefix.h"
#include "colors-C-C-plus-plus\colors.h"

#define CALC_SIZE_STRING_FLAG 0

typedef enum String_list_link_flags {
    FLAG_NONE_Sll = 0b00000000, // ninguna flag activada
    FLAG_FREE_Sll = 0b00000001, // esta flag indica que a de liberarse la memoria de size_string
} String_list_link_flags;

typedef struct String_list_link {
    size_t size_string;                   // tamaño del string contenido
    char   *actual_string;                // punteo del string actual
    struct String_list_link *next_string; // puntero al siguiente string
    String_list_link_flags flags;         // en caso de necesitarte liberar actual_string, se marca como 1 esta flag
} String_list_link;



void print_String_list_link(String_list_link *list);
String_list_link *Init_String(char *string, size_t size_string);
String_list_link *free_String_list_link(String_list_link* list);
static inline size_t get_size_to_String(String_list_link *list);
static inline size_t get_number_nodes_String_list_link(String_list_link *list);
String_list_link *join_list_to_String(String_list_link *list, char* string_for_join);
String_list_link *get_string_instruction(Instruction_info *my_instruccion_, encoder_x86 encoder_val);
static inline String_list_link *push_String(String_list_link *list, char* string, size_t size_string);


#include "stringx.c"
#endif