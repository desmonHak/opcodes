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
#ifndef __INSTRUTTIONS_FORMAT_H__
#define __INSTRUTTIONS_FORMAT_H__

#include "opcodes_prefix.h"

#include <stdlib.h>
#include <stdint.h>

typedef struct List_instrution {
    struct List_instrution *next_list_instrution;
    Instruction_info Instruction;
    size_t id;
} List_instrution;

void print_List_instrution(List_instrution *list_instrution, encoder_x86 encoder_val);
uint8_t get_registers_form_byte(Instruction_info *Instruction, uint8_t* bytes);
size_t get_number_instrutions(List_instrution *instrutions);

List_instrution *init_List_instrution();
List_instrution *push_List_instrution(List_instrution *list_instrution);
List_instrution *pop_List_instrution(List_instrution *list_instrution, size_t position);
List_instrution *format_instruccion(uint8_t *instrutions, size_t size_in_bytes, encoder_x86 encoder_val);

#include "print_structs_format.h"

#include "instrutions_format.c"
#endif