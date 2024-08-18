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

static inline uint8_t get_bit_s(Instruction_info *instrucion) {
    /*
     *
     * obtiene el bit/campo s del opcode primario
     *
     */
    if (instrucion->posicion_s == 0) return instrucion->posicion_s;
    else return ((uint8_t)instrucion->instruction.opcode[2].opcode_byte.byte & (uint8_t)instrucion->posicion_s) >> (instrucion->posicion_s - 1);
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
    register size_t one_count = 0; // indicar que se use un registro si es posible
    for(; x ; x >>= 1) if(x % 2) ++one_count;
    return one_count;
}

uint64_t count_get_mask(uint64_t x)
{
    /*
     *
     * Cuenta 0's desde la derecha a izquierda hasta obtener algun 1
     *
     */
    for(register size_t one_count = 0; x; ++one_count, x >>= 1) 
        if(x & 1) 
            return one_count; 
    return 0;
}

static char *get_string_instruction_by_id(string_instrution_id id) {
    static char *strings[] = {
        [STRING_AAA]        = "aaa",
        [STRING_AAD]        = "aad",
        [STRING_AAM]        = "aam",
        [STRING_AAS]        = "aas",
        [STRING_ADC]        = "adc",
        [STRING_ADD]        = "add",
        [STRING_AND]        = "and",
        [STRING_ARPL]       = "arpl",
        [STRING_BOUND]      = "bound",
        [STRING_BSF]        = "bsf",
        [STRING_BSR]        = "bsr",
        [STRING_BSWAP]      = "bswap",
        [STRING_BT]         = "bt",
        [STRING_BTC]        = "btc",
        [STRING_BTR]        = "btr",
        [STRING_BTS]        = "bts",
        [STRING_CALL]       = "call",
        [STRING_CALL_FAR]   = "call far",
        [STRING_CBW]        = "cbw",
        [STRING_CDQ]        = "cdq",
        [STRING_CLC]        = "clc",
        [STRING_CLD]        = "cld",
        [STRING_CLI]        = "cli",
        [STRING_CLTS]       = "clts",
        [STRING_CMC]        = "cmc",
        [STRING_CMP]        = "cmp",
        [STRING_CMPS]       = "cmps",
        // [STRING_CMPSB]  = "cmpsb",
        // [STRING_CMPSW]  = "cmpsw",
        // [STRING_CMPSD]  = "cmpsd",
        [STRING_CMPXCHG]    = "cmpxchg",
        [STRING_CPUID]      = "cpuid",
        [STRING_CWD]        = "cwd",
        [STRING_CWDE]       = "cwde",
        [STRING_DAA]        = "daa",
        [STRING_DAS]        = "das",
        [STRING_DEC]        = "dec",
        [STRING_DIV]        = "div",
        [STRING_HLT]        = "hlt",
        [STRING_IDIV]       = "idiv",
        [STRING_IMUL]       = "imul",
        [STRING_IN]         = "in",
        [STRING_INC]        = "inc",
        [STRING_INS]        = "ins",
        [STRING_INT_n]      = "int",
        [STRING_INT_3]      = "int 3",
        [STRING_INTO]       = "into",
        [STRING_INVD]       = "invd",
        [STRING_INVLPG]     = "invlpg",
        [STRING_INVPCID]    = "invpcid",
        [STRING_IRET]       = "iret",
        // [STRING_IRETD]  = "iretd",
        [STRING_JCC]        = "jcc",
        [STRING_JCXZ]       = "jcxz",
        // [STRING_JECXZ]  = "jecxz",
        [STRING_JMP]        = "jmp",
        [STRING_JMP_FAR]    = "jmp far",
        [STRING_LAHF]       = "lahf",
        [STRING_LAR]        = "lar",
        [STRING_LDS]        = "lds",
        [STRING_LEA]        = "lea",
        [STRING_LEAVE]      = "leave",
        [STRING_LES]        = "les",
        [STRING_LFS]        = "lfs",
        [STRING_LGDT]       = "lgdt",
        [STRING_LGS]        = "lgs",
        [STRING_LIDT]       = "lidt",
        [STRING_LLDT]       = "lldt",
        [STRING_LMSW]       = "lmsw",
        [STRING_LOCK]       = "lock",
        [STRING_LODS]       = "lods",
        // [STRING_LODSB]  = "lodsb",
        // [STRING_LODSW]  = "lodsw",
        // [STRING_LODD]   = "lodsd",
        [STRING_LOOP]       = "loop",
        [STRING_LOOPZ]      = "loopz",
        // [STRING_LOOPE]  = "loope",
        [STRING_LOOPNZ]     = "loopnz",
        // [STRING_LOOPNE] = "loopne",
        [STRING_LSL]        = "lsl",
        [STRING_LSS]        = "lss",
        [STRING_LTR]        = "ltr",
        [STRING_MOV]        = "mov",
        [STRING_MOVBE]      = "movbe",
        [STRING_MOVS]       = "movs",
        // [STRING_MOVSB]  = "movsb",
        // [STRING_MOVSW]  = "movsw",
        // [STRING_MOVSD]  = "movsd",
        [STRING_MOVSX]      = "movsx",
        [STRING_MOVZX]      = "movzx",
        [STRING_MUL]        = "mul",
        [STRING_NEG]        = "neg",
        [STRING_NOP]        = "nop",
        [STRING_NOT]        = "not",
        [STRING_OR]         = "or",
        [STRING_OUT]        = "out",
        [STRING_OUTS]       = "outs",
        [STRING_POP]        = "pop",
        [STRING_POPA]       = "popa",
        // [STRING_POPAD]  = "popad",
        [STRING_POPF]       = "popf",
        // [STRING_POPFD]  = "popfd",
        [STRING_PUSH]       = "push",
        [STRING_PUSHA]      = "pusha",
        // [STRING_PUSHAD] = "pushad",
        [STRING_PUSHF]      = "pushf",
        // [STRING_PUSHFD] = "pushfd",
        [STRING_RCL]        = "rcl",
        [STRING_RCR]        = "rcr",
        [STRING_RDMSR]      = "rdmsr",
        [STRING_RDPMC]      = "rdpmc",
        [STRING_RDTSC]      = "rdtsc",
        [STRING_RDTSCP]     = "rdtscp",
        [STRING_REP_INS]    = "rep ins",
        [STRING_REP_LODS]   = "rep lods",
        [STRING_REP_MOVS]   = "rep movs",
        [STRING_REP_OUTS]   = "rep outs",
        [STRING_REP_STOS]   = "rep stos",
        [STRING_REPE_CMPS]  = "repe cmps",
        [STRING_REPE_SCAS]  = "repe scas",
        [STRING_REPNE_CMPS] = "repne cmps",
        [STRING_REPNE_SCAS] = "repne scas",
        [STRING_RET]        = "ret",
        [STRING_RETF]       = "retf",
        [STRING_ROL]        = "rol",
        [STRING_ROR]        = "ror",
        [STRING_RSM]        = "rsm",
        [STRING_SAHF]       = "sahf",
        [STRING_SAL]        = "sal",
        [STRING_SAR]        = "sar",
        [STRING_SBB]        = "sbb",
        [STRING_SCAS]       = "scas",
        // [STRING_SCASB]  = "scasb",
        // [STRING_SCASW]  = "scasw",
        // [STRING_SCASD]  = "scasd",
        [STRING_SETcc]      = "setcc",
        [STRING_SGDT]       = "sgdt",
        [STRING_SHL]        = "shl",
        [STRING_SHLD]       = "shld",
        [STRING_SHR]        = "shr",
        [STRING_SHRD]       = "shrd",
        [STRING_SIDT]       = "sidt",
        [STRING_SLDT]       = "sldt",
        [STRING_SMSW]       = "smsw",
        [STRING_STC]        = "stc",
        [STRING_STD]        = "std",
        [STRING_STI]        = "sti",
        [STRING_STOS]       = "stos",
        // [STRING_STOSB]  = "stosb",
        // [STRING_STOSW]  = "stosw",
        // [STRING_STOSD]  = "stosd",
        [STRING_STR]        = "str",
        [STRING_SUB]        = "sub",
        [STRING_TEST]       = "test",
        [STRING_UD]         = "ud",
        [STRING_VERR]       = "verr",
        [STRING_VERW]       = "verw",
        [STRING_WAIT]       = "wait",
        [STRING_WBINVD]     = "wbinvd",
        [STRING_WRMSR]      = "wrmsr",
        [STRING_XADD]       = "xadd",
        [STRING_XCHG]       = "xchg",
        [STRING_XOR]        = "xor",
    };

    if (id >= STRING_AAA && id <= STRING_XOR) return strings[id];
    else return "error - not exits this instruttion.";

    /*switch (id)
    {
        case STRING_AAA: return ;
        case STRING_AAD: return ;
        case STRING_AAM: return ;
        case STRING_AAS: return ;
        case STRING_ADC: return ;
        case STRING_ADD: return ;
        case STRING_AND: return ;
        case STRING_ARPL: return ;
        case STRING_BOUND: return ;
        case STRING_BSF: return ;
        case STRING_BSR: return ;
        case STRING_BSWAP: return ;
        case STRING_BT: return ;
        case STRING_BTC: return ;
        case STRING_BTC: return ;
        case STRING_BTC: return ;
        case STRING_BTC: return ;
        default: return "error - not exits this instruttion.";
    }*/
}

char *get_build_SIB_format_for_data_inmediate(Instruction_info *my_instruccion_){
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
            INIT_TYPE_FUNC_DBG(char *, get_build_SIB_format_for_data_inmediate)
                TYPE_DATA_DBG(Instruction_info*, "my_instruccion_ = %p")
            END_TYPE_FUNC_DBG,
            my_instruccion_);
    if (my_instruccion_ == NULL) {
        #ifdef DEBUG_ENABLE
        printf("\t -> [get_build_SIB_format_for_data_inmediate] my_instruccion_ == NULL: Error\n");
        #endif
        return NULL;
    }
    // Esta funcion solo se usa para instrucciones con SIB y movimientos de datos inmediatos.
    // por tanto solo se puede usar con w = 1 en 32bits y w = 0 (valor de 8bits) en 32bits,
    // w = 1 16bits no se permite en el modo SIB

    char    *formatter      = NULL;
    uint32_t desplazamiento = 0;
    if (my_instruccion_->immediate_data == 0b1) {
        // verificar que la instruccion es de datos inmediatos
        switch (get_bit_w(my_instruccion_)) {
            case 0b0: // para 8bits
                formatter = "0x%02x";
                desplazamiento = *((uint8_t*)&(my_instruccion_->instruction.immediate));
                break;
            case 0b1: // para 32bits
                formatter = "0x%08x";
                desplazamiento = *((uint32_t*)&(my_instruccion_->instruction.immediate));
                break;
            default: return NULL;
        }
        uint32_t size_len_register  = (snprintf(NULL, 0, formatter, desplazamiento) + 1) * sizeof(char);
        char* formatter_building = (char *)malloc(size_len_register);
        sprintf(formatter_building, formatter, desplazamiento); 
        #ifdef DEBUG_ENABLE
        printf("\t -> [get_build_SIB_format_for_data_inmediate] La los datos inmediatos formateados es: %s\n", formatter_building);
        #endif
        return formatter_building;
    } else {
        // si la instruccion no es de datos inmediatos, ocurrio algo inesperado
        #ifdef DEBUG_ENABLE
        printf("\t -> [get_build_SIB_format_for_data_inmediate] La instruccion no es de datos inmediatos. ERROR!\n");
        #endif
        return NULL;
    }
}

char *get_data_inmediate_16bits(Instruction_info *my_instruccion_){
    
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(char *, get_data_inmediate_16bits)
            TYPE_DATA_DBG(Instruction_info*, "my_instruccion_ = %p")
        END_TYPE_FUNC_DBG,
        my_instruccion_);
    
    if (my_instruccion_ == NULL) {
        #ifdef DEBUG_ENABLE
        printf("\t -> [get_data_inmediate_16bits] my_instruccion_ == NULL: Error\n");
        #endif
        return NULL;
    }

    char    *formatter      = NULL;
    uint16_t desplazamiento = 0;
    if (my_instruccion_->immediate_data == 0b1) {
        // verificar que la instruccion es de datos inmediatos
        switch (get_bit_w(my_instruccion_)) {
            case 0b0: // para 8bits
                format_to_8bits:
                formatter = "0x%02x";
                desplazamiento = *((uint8_t*)&(my_instruccion_->instruction.immediate));
                break;
            case 0b1: // para 16bits
                if (get_bit_s(my_instruccion_)) goto format_to_8bits;
                // si el campo w y el s estan activo, se usa un valor de 8bits que se extiende a 16
                
                formatter = "0x%04x";
                desplazamiento = *((uint16_t*)&(my_instruccion_->instruction.immediate));
                break;
            default: return NULL;
        }
        uint16_t size_len_register  = (snprintf(NULL, 0, formatter, desplazamiento) + 1) * sizeof(char);
        char* formatter_building = (char *)malloc(size_len_register);
        sprintf(formatter_building, formatter, desplazamiento); 
        #ifdef DEBUG_ENABLE
        printf("\t -> [get_data_inmediate_16bits] La los datos inmediatos formateados es: %s\n", formatter_building);
        #endif
        return formatter_building;
    } else {
        // si la instruccion no es de datos inmediatos, ocurrio algo inesperado
        #ifdef DEBUG_ENABLE
        printf("\t -> [get_data_inmediate_16bits] La instruccion no es de datos inmediatos. ERROR!\n");
        #endif
        return NULL;
    }
}

char *get_build_SIB_format(Instruction_info *my_instruccion_){
    
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(char *, get_build_SIB_format)
            TYPE_DATA_DBG(Instruction_info*, "my_instruccion_ = %p")
        END_TYPE_FUNC_DBG,
        my_instruccion_);
    
    if (my_instruccion_ == NULL) {
        #ifdef DEBUG_ENABLE
        printf("\t -> [get_build_SIB_format] my_instruccion_ == NULL: Error\n");
        #endif
        return NULL;
    }

    char    *formatter      = NULL, *formatter_building = NULL;
    uint32_t desplazamiento = 0,     size_len_register  = 0;
    switch (my_instruccion_->instruction.Mod_rm.mod) {
        //  MOD R/M Modo de direccionamiento
        //  --- --- --------------------------- 
        //   00 100 SIB
        //   01 100 SIB + disp8
        //   10 100 SIB + disp32
        case 0b00: // 00 100 SIB
            formatter = "[%s * %u + %s]";
            break;
        case 0b01: // 00 100 SIB + disp8
            formatter = "[%s * %u + %s + 0x%02x]";
            desplazamiento = *((uint8_t*)&(my_instruccion_->instruction.displacement));
            break;
        case 0b10: // 00 100 SIB + disp32
            formatter = "[%s * %u + %s + 0x%08x]";
            desplazamiento = *((uint32_t*)&(my_instruccion_->instruction.displacement));
            break;
        default: return NULL;
    }
    #ifdef DEBUG_ENABLE
    printf_color("\t -> [get_build_SIB_format] formatter(#{FG:cyan}%s#{FG:reset}), desplazamiento(%08x)\n", formatter, desplazamiento);
    #endif

    if (my_instruccion_->instruction.Mod_rm.R_M != 0b00) {

        // [    %s     *    %u      +     %s    +    0x%x    ]
        // [Index(reg) * Scale(val) + Base(reg) + value_disp ] == [2 * eax + ebx + 0xaabbccdd]
        size_len_register = (
            snprintf(NULL, 0, formatter,
                // sustitur get_string_mod_3 por una func para el sib, registros ilegales
                get_string_mod_3(0b01, 0b01, my_instruccion_->instruction.SIB.index), // Index(reg)
                0b01 << my_instruccion_->instruction.SIB.scale,                       // Scale(val)
                // Scale = 00; -> index * 1   (0b01 << 0b00) == 0b0001 (1)
                // Scale = 01; -> index * 2   (0b01 << 0b01) == 0b0010 (2)
                // Scale = 10; -> index * 4   (0b01 << 0b10) == 0b0100 (4)
                // Scale = 11; -> index * 8   (0b01 << 0b11) == 0b1000 (8)
                //                            (0b01 << scale_binary_val) == scale_binary_decimal
                get_string_mod_3(0b01, 0b01, my_instruccion_->instruction.SIB.base),  // Base(reg)
                // se pasa 0b01, 0b01 pues la instruccion que se codifica es de 32bits con registros de 32bits
                desplazamiento // solo se añade para mod != 0b00 o para 0b01 y 0b10
            ) + 1
        ) * sizeof(char);

        formatter_building = (char *)malloc(size_len_register);
        sprintf(formatter_building, formatter, 
            get_string_mod_3(0b01, 0b01, my_instruccion_->instruction.SIB.index), // Index(reg)
            0b01 << my_instruccion_->instruction.SIB.scale,                       // Scale(val)
            get_string_mod_3(0b01, 0b01, my_instruccion_->instruction.SIB.base),  // Base(reg)
            desplazamiento
        ); 

    } else {

            size_len_register = (
            snprintf(NULL, 0, formatter,
                get_string_mod_3(0b01, 0b01, my_instruccion_->instruction.SIB.index), // Index(reg)
                0b01 << my_instruccion_->instruction.SIB.scale,                       // Scale(val)
                get_string_mod_3(0b01, 0b01, my_instruccion_->instruction.SIB.base)   // Base(reg)
            ) + 1
        ) * sizeof(char);

        formatter_building = (char *)malloc(size_len_register);
        sprintf(formatter_building, formatter, 
            get_string_mod_3(0b01, 0b01, my_instruccion_->instruction.SIB.index), // Index(reg)
            0b01 << my_instruccion_->instruction.SIB.scale,                       // Scale(val)
            get_string_mod_3(0b01, 0b01, my_instruccion_->instruction.SIB.base)   // Base(reg)
        ); 

    }
    #ifdef DEBUG_ENABLE
    printf_color("\t -> [get_build_SIB_format] formatter_building(#{FG:cyan}%s#{FG:reset})\n", formatter_building);
    #endif
    //printf(">>> %s\n", formatter_building);
    return formatter_building;
}

static char *get_string_mod_0(encoder_x86 size_word, Instruction_info *my_instruccion_) {
    /*
     *
     *  Se recibe un register_id el cual especifica un registro. Se espera recibir un bit "w"
     *  El cual especifique si se trata de registros de 8bits u de 16/32bits.
     *  Se espera recibir un bit "size_word" el cual indique si se usa operaciones de datos
     *  de 16bits (size_word = 0b0) o si se trata de operaciones de datos de 32bits (size_word = 0b1)
     * 
     */
    
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(static char *, get_string_mod_0)
            TYPE_DATA_DBG(encoder_x86, "encoder_x86 = %02x")
            TYPE_DATA_DBG(Instruction_info*, "my_instruccion_ = %p")
        END_TYPE_FUNC_DBG,
        size_word, my_instruccion_);
    
    switch (size_word){ // si hay un bit, el campo w es 1, si hay 0 bits 1. el campo w esta en 0. si hay mas de 1, error
        case 0b0: // para 16 bits
            switch (my_instruccion_->instruction.Mod_rm.R_M) { // registros de 16 bits
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
            switch (my_instruccion_->instruction.Mod_rm.R_M) { // registros de 32 bits
                case 0b000: return "[eax]";
                case 0b001: return "[ecx]";
                case 0b010: return "[edx]";
                case 0b011: return "[ebx]";
                case 0b100: return "[sib]";
                case 0b101: return "[0x%08x]"; // desplazamiento de 32bits
                case 0b110: return "[esi]";
                case 0b111: return "[edi]";
                default: return  "error - Este mod 0 en con rm en 32bits no se define.";
            }
        default: return  "error - No se puede operar en este size de datos.";
    }
}

static char *get_string_mod_1(encoder_x86 size_word, Instruction_info *my_instruccion_){
    
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(static char *, get_string_mod_1)
            TYPE_DATA_DBG(encoder_x86, "encoder_x86 = %02x")
            TYPE_DATA_DBG(Instruction_info*, "my_instruccion_ = %p")
        END_TYPE_FUNC_DBG,
        size_word, my_instruccion_);
    
    switch (size_word){ // si hay un bit, el campo w es 1, si hay 0 bits 1. el campo w esta en 0. si hay mas de 1, error
        case 0b0: // para 16 bits
            switch (my_instruccion_->instruction.Mod_rm.R_M) { // registros de 16 bits
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
            switch (my_instruccion_->instruction.Mod_rm.R_M) { // registros de 32 bits
                case 0b000: return "[eax + 0x%02x]";
                case 0b001: return "[ecx + 0x%02x]";
                case 0b010: return "[edx + 0x%02x]";
                case 0b011: return "[ebx + 0x%02x]";
                case 0b100: return "[sib + 0x%02x]";
                case 0b101: return "[ebp + 0x%02x]";
                case 0b110: return "[esi + 0x%02x]";
                case 0b111: return "[edi + 0x%02x]";
                default: return  "error - Este mod 0 en con rm en 32bits no se define.";
            }
        default: return  "error - No se puede operar en este size de datos.";
    }
}

static char *get_string_mod_2(encoder_x86 size_word, Instruction_info *my_instruccion_){
    
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(static char *, get_string_mod_2)
            TYPE_DATA_DBG(encoder_x86, "encoder_x86 = %02x")
            TYPE_DATA_DBG(Instruction_info*, "my_instruccion_ = %p")
        END_TYPE_FUNC_DBG,
        size_word, my_instruccion_);
    
    switch (size_word){ // si hay un bit, el campo w es 1, si hay 0 bits 1. el campo w esta en 0. si hay mas de 1, error
        case 0b0: // para 16 bits
            switch (my_instruccion_->instruction.Mod_rm.R_M) { // registros de 16 bits
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
            switch (my_instruccion_->instruction.Mod_rm.R_M) { // registros de 32 bits
                case 0b000: return "[eax + 0x%08x]";
                case 0b001: return "[ecx + 0x%08x]";
                case 0b010: return "[edx + 0x%08x]";
                case 0b011: return "[ebx + 0x%08x]";
                case 0b100: return "[sib + 0x%08x]";
                case 0b101: return "[ebp + 0x%08x]";
                case 0b110: return "[esi + 0x%08x]";
                case 0b111: return "[edi + 0x%08x]";
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
    
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(static char *, get_string_mod_3)
            TYPE_DATA_DBG(encoder_x86, "encoder_x86 = %02x")
            TYPE_DATA_DBG(uint8_t, "bit_w = %02x")
            TYPE_DATA_DBG(register_id, "id = 0x%02x")
        END_TYPE_FUNC_DBG,
        size_word, bit_w, id);
    
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