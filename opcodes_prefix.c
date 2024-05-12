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
#ifndef __OPCODES_PREFIX_C_
#define __OPCODES_PREFIX_C_

#include "opcodes_prefix.h"

static inline uint8_t get_bit_w(Instruction_info *instrucion) {
    /*
     *
     * obtiene el bit/campo w del opcode primario
     *
     */
    return ((uint8_t)instrucion->instruction.opcode[2].opcode_byte.byte & (uint8_t)instrucion->posicion_w);
}

static inline uint8_t get_bit_d(Instruction_info *instrucion) {
    /*
     *
     * obtiene el bit/campo d del opcode primario
     *
     */
    return instrucion->instruction.opcode[2].opcode_byte.byte & instrucion->posicion_d;
}

static inline uint64_t popcnt_software(uint64_t x)
{
    /*
     *
     * Cuenta la cantidad de 1's que hay en un valor x
     *
     */
    uint64_t one_count = 0;
    while(x) {
        if(x % 2) ++one_count;
        x >>= 1;
    }
    return one_count;
}
uint64_t count_get_mask(uint64_t x)
{
    /*
     *
     * Cuenta 0's desde la derecha a izquierda hasta obtener algun 1
     *
     */
    for(uint64_t one_count = 0; x; ++one_count, x >>= 1) 
        if(x & 1) 
            return one_count; 
    return 0;
}

static char *get_string_instruction_by_id(string_instrution_id id) {
    switch (id)
    {
        case STRING_AAA: return "aaa";
        case STRING_AAD: return "aad";
        case STRING_AAM: return "aam";
        case STRING_AAS: return "aas";
        case STRING_ADC: return "adc";
        
        default: return "error - not exits this instruttion.";
    }
}

static char *get_string_mod_0(encoder_x86 size_word, register_id id, uint32_t disp32) {
    /*
     *
     *  Se recibe un register_id el cual especifica un registro. Se espera recibir un bit "w"
     *  El cual especifique si se trata de registros de 8bits u de 16/32bits.
     *  Se espera recibir un bit "size_word" el cual indique si se usa operaciones de datos
     *  de 16bits (size_word = 0b0) o si se trata de operaciones de datos de 32bits (size_word = 0b1)
     * 
     */
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(static char *, get_string_mod_0)
                TYPE_DATA_DBG(encoder_x86, "encoder_x86 = %02x")
                TYPE_DATA_DBG(register_id, "id = %02x")
                TYPE_DATA_DBG(uint32_t, "disp32 = 0x%08x")
            END_TYPE_FUNC_DBG,
            size_word, id, disp32);
    #endif
    switch (size_word){ // si hay un bit, el campo w es 1, si hay 0 bits 1. el campo w esta en 0. si hay mas de 1, error
        case 0b0: // para 16 bits
            switch (id) { // registros de 16 bits
                case 0b000: return "[bx + si]";
                case 0b001: return "[bx + di]";
                case 0b010: return "[bp + si]";
                case 0b011: return "[bp + di]";
                case 0b100: return "[si]";
                case 0b101: return "[di]";
                case 0b110: return "[0x%04x]"; // desplazamiento de 16bits
                case 0b111: return "[bx]";
                default: return  "error - Este mod 0 en con rm en 16bits no se define.";
            }
        case 0b1: // para 32 bits
            switch (id) { // registros de 32 bits
                case 0b000: return "[eax]";
                case 0b001: return "[ecx]";
                case 0b010: return "[edx]";
                case 0b011: return "[ebx]";
                case 0b100: return "sib";
                case 0b101: return "[0x%08x]"; // desplazamiento de 32bits
                case 0b110: return "[esi]";
                case 0b111: return "[edi]";
                default: return  "error - Este mod 0 en con rm en 32bits no se define.";
            }
        default: return  "error - No se puede operar en este size de datos.";
    }
}

static char *get_string_mod_1(encoder_x86 size_word, register_id id, uint8_t disp8){
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(static char *, get_string_mod_1)
                TYPE_DATA_DBG(encoder_x86, "encoder_x86 = %02x")
                TYPE_DATA_DBG(register_id, "id = %02x")
                TYPE_DATA_DBG(uint8_t, "disp8 = 0x%02x")
            END_TYPE_FUNC_DBG,
            size_word, id, disp8);
    #endif
    switch (size_word){ // si hay un bit, el campo w es 1, si hay 0 bits 1. el campo w esta en 0. si hay mas de 1, error
        case 0b0: // para 16 bits
            switch (id) { // registros de 16 bits
                case 0b000: return "[bx + si + 0x%02x]";
                case 0b001: return "[bx + di + 0x%02x]";
                case 0b010: return "[bp + si + 0x%02x]";
                case 0b011: return "[bp + di + 0x%02x]";
                case 0b100: return "[si + 0x%02x]";
                case 0b101: return "[di + 0x%02x]";
                case 0b110: return "[bp + 0x%02x]";
                case 0b111: return "[bx + 0x%02x]";
                default: return  "error - Este mod 0 en con rm en 16bits no se define.";
            }
        case 0b1: // para 32 bits
            switch (id) { // registros de 32 bits
                case 0b000: return "[eax + 0x%02x]";
                case 0b001: return "[ecx + 0x%02x]";
                case 0b010: return "[edx + 0x%02x]";
                case 0b011: return "[ebx + 0x%02x]";
                case 0b100: return "sib + 0x%02x";
                case 0b101: return "[ebp + 0x%02x]";
                case 0b110: return "[esi + 0x%02x]";
                case 0b111: return "[edi + 0x%02x]";
                default: return  "error - Este mod 0 en con rm en 32bits no se define.";
            }
        default: return  "error - No se puede operar en este size de datos.";
    }
}

static char *get_string_mod_2(encoder_x86 size_word, register_id id, uint32_t disp32){
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(static char *, get_string_mod_1)
                TYPE_DATA_DBG(encoder_x86, "encoder_x86 = %02x")
                TYPE_DATA_DBG(register_id, "id = %02x")
                TYPE_DATA_DBG(uint32_t, "disp32 = 0x%08x")
            END_TYPE_FUNC_DBG,
            size_word, id, disp32);
    #endif
    switch (size_word){ // si hay un bit, el campo w es 1, si hay 0 bits 1. el campo w esta en 0. si hay mas de 1, error
        case 0b0: // para 16 bits
            switch (id) { // registros de 16 bits
                case 0b000: return "[bx + si + 0x%04x]";
                case 0b001: return "[bx + di + 0x%04x]";
                case 0b010: return "[bp + si + 0x%04x]";
                case 0b011: return "[bp + di + 0x%04x]";
                case 0b100: return "[si + 0x%04x]";
                case 0b101: return "[di + 0x%04x]";
                case 0b110: return "[bp + 0x%04x]";
                case 0b111: return "[bx + 0x%04x]";
                default: return  "error - Este mod 0 en con rm en 16bits no se define.";
            }
        case 0b1: // para 32 bits
            switch (id) { // registros de 32 bits
                case 0b000: return "[eax] + 0x%08x";
                case 0b001: return "[ecx] + 0x%08x";
                case 0b010: return "[edx] + 0x%08x";
                case 0b011: return "[ebx] + 0x%08x";
                case 0b100: return "sib + 0x%08x";
                case 0b101: return "[ebp] + 0x%08x";
                case 0b110: return "[esi] + 0x%08x";
                case 0b111: return "[edi] + 0x%08x";
                default: return  "error - Este mod 0 en con rm en 32bits no se define.";
            }
        default: return  "error - No se puede operar en este size de datos.";
    }
}

static char *get_string_mod_3(encoder_x86 size_word, uint8_t bit_w, register_id id) {
    /*
     *  Esta funcion tambien puede ser llamada get_string_register
     *  Se recibe un register_id el cual especifica un registro. Se espera recibir un bit "w"
     *  El cual especifique si se trata de registros de 8bits u de 16/32bits.
     *  Se espera recibir un bit "size_word" el cual indique si se usa operaciones de datos
     *  de 16bits (size_word = 0b0) o si se trata de operaciones de datos de 32bits (size_word = 0b1)
     * 
     */
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(static char *, get_string_mod_3)
                TYPE_DATA_DBG(encoder_x86, "encoder_x86 = %02x")
                TYPE_DATA_DBG(uint8_t, "bit_w = %02x")
                TYPE_DATA_DBG(register_id, "id = 0x%02x")
            END_TYPE_FUNC_DBG,
            size_word, bit_w, id);
    #endif
    switch (popcnt_software(bit_w)){ // si hay un bit, el campo w es 1, si hay 0 bits 1. el campo w esta en 0. si hay mas de 1, error
        case 0b0:  // si el campo "w" fue definido como 0 (8bits)
            switch (id) { // registros de 8 bits
                case 0b000: return "al";
                case 0b001: return "cl";
                case 0b010: return "dl";
                case 0b011: return "bl";
                case 0b100: return "ah";
                case 0b101: return "ch";
                case 0b110: return "dh";
                case 0b111: return "bh";
                default: return  "error - Este registro no esta definido en los 8bits.";
            }
        case 0b1:  // si el campo "w" fue definido como 1
            switch (size_word)
            {
                case 0b0: // para 16 bits
                    switch (id) { // registros de 16 bits
                        case 0b000: return "ax";
                        case 0b001: return "cx";
                        case 0b010: return "dx";
                        case 0b011: return "bx";
                        case 0b100: return "sp";
                        case 0b101: return "bp";
                        case 0b110: return "si";
                        case 0b111: return "di";
                        default: return  "error - Este registro no esta definido en los 16bits.";
                    }
                case 0b1: // para 32 bits
                    switch (id) { // registros de 32 bits
                        case 0b000: return "eax";
                        case 0b001: return "ecx";
                        case 0b010: return "edx";
                        case 0b011: return "ebx";
                        case 0b100: return "esp";
                        case 0b101: return "ebp";
                        case 0b110: return "esi";
                        case 0b111: return "edi";
                        default: return  "error - Este registro no esta definid en los 32bits.";
                    }
                default: return  "error - No se puede operar en este size de datos.";
            }
        default: return "error - campo 'w' excede un bit.";
    }
}


#endif