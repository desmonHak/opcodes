#ifndef __OPCODES_PREFIX_H_
#define __OPCODES_PREFIX_H_
/* 
 * Formato general de instrucciones x86 (http://www.c-jump.com/CIS77/images/x86_instruction_format.png): 
 * 
 *      |-----------------|---------|--------|----------------------------------------------------------|-------|----------------|-------------|
 *      | numero de bytes |  0 a 4  | 1 a 3  |                                0 o 1                     | 0 o 1 |  0, 1,  2 o 4  | 0, 1, 2 o 4 |  
 *      | valores         | prefijo | opcode |                               Mod-RM                     |  SIB  | desplazamiento |  inmediato  |
 *      |-----------------|---------|------v-|-------------------------------v--------------------------|---v---|----------------|-------------|
 *           ______________________________|                                 |                              |
 *          |                                                                |                              |
 *        |-v---|------------------|-----------|-----------------------|  |--v--|-----|-------|-------|  |--v--|-------|-------|-------|
 *        | Bit |  7  6 5  4 3  2  |     1     |            0          |  | Bit | 7 6 | 5 4 3 | 2 1 0 |  | Bit |  7 6  | 5 4 3 | 2 1 0 |
 *        | uso | main bits opcode | direccion | Longitud del operando |  | Uso | MOD |  REG  |  R/M  |  | Uso | SCALE | INDEX |  BASE |
 *        |-----|------------------|-----------|-----------------------|  |-----|-----|-------|-------|  |-----|-------|-------|-------|
 *             
 */
#include <stdint.h>

#define DEBUG_ENABLE // debug activado

#include "debug_c.h"

typedef enum string_instrution_id {
    STRING_AAA,
    STRING_AAD,
    STRING_AAM,
    STRING_AAS,
    STRING_ADC,
} string_instrution_id;

typedef enum encoder_x86 { // se especifica el formato a encodificar / descodificar las instrucciones
    ENCODER_IN_16bits,
    ENCODER_IN_32bits,
    ENCODER_IN_64bits
} encoder_x86;

typedef enum register_id { // se especifica los registros
    REGISTER_A = 0b000, // AL, AX, EAX = 000
    REGISTER_C = 0b001, // CL, CX, ECX = 001
    REGISTER_B = 0b010, // BL, BX, EBX = 010
    REGISTER_D = 0b011, // DL, DX, EDX = 011
    REGISTER_S = 0b100, // AH, SP, ESX = 100
    REGISTER_BP = 0b101, // CH, BP, EBX = 101
    REGISTER_SP = 0b110, // DH, SI, ESX = 110
    REGISTER_DI = 0b111, // BH, DI, EDX = 111
} register_id;

#pragma pack(push, 1)

typedef union opcode
{
    struct opcode_bits_final {
        uint8_t      s:1; /* 
                        * Longitud del operando(size?):
                        * Si s = 0, los operandos son registros de 8 bits y posiciones de memoria.
                        * Si s = 1, los operandos son de 16 bits o de 32 bits:
                        */
        uint8_t      d:1; /* 
                        * direcion:
                        * Si d=0, REG es la fuente, MOD R/M <- REG.
                        * Si d=1, REG es el destino, REG <- MOD R/M.
                        */
        uint8_t opcode:6; // opcode de 1 byte
    } opcode_bits_final; // no siempre se da este formato en las instrucciones
    struct opcode_bits {
        uint8_t b1:1; uint8_t b2:1; uint8_t b3:1; uint8_t b4:1; 
        uint8_t b5:1; uint8_t b6:1; uint8_t b7:1; uint8_t b8:1; 
    } opcode_bits; // acceso a los campos de bit a bit (para los casos en los que el campo "w", "d", "s", puedan estar en diferentes ubicaciones)
    struct opcode_byte {
        uint8_t byte; 
    } opcode_byte; // acceso a todos los campos en forma de byte
} opcode;

typedef struct Mod_rm {
    uint8_t    R_M:3; // Longitud del operando(size?)
    uint8_t    reg:3; // direcion
    uint8_t    mod:2; /*
                       * campo MOD:
                       *  - 00 Modo de direccionamiento indirecto de registro o SIB sin desplazamiento (cuando R/M = 100) o modo de direccionamiento de sólo desplazamiento (cuando R/M = 101).
                       *  - 01 El desplazamiento con signo de un byte sigue a los bytes del modo de direccionamiento.
                       *  - 10 El desplazamiento con signo de cuatro bytes sigue a los bytes del modo de direccionamiento.
                       *  - 11 Modo de direccionamiento de registro.
                       */
} Mod_rm;

/*
 * Byte SIB (Byte de índice escalado): ( Valor SIB = Index * scale + base ) 
 * El byte SIB es un byte opcional post-opcode en ensamblador x86 en el i386 y posteriores, utilizado 
 * para direccionamiento complejo. El modo de direccionamiento indexado escalado utiliza el segundo byte (es decir, el byte SIB) 
 * que sigue al byte MOD-REG-R/M en el formato de instrucción.
 * El campo MOD todavía especifica el tamaño de desplazamiento de cero, uno o cuatro bytes.
 * Los bytes MOD-REG-R/M y SIB son complejos, porque Intel reutilizó los circuitos de direccionamiento de 16 bits en el modo de 
 * 32 bits, en lugar de abandonar simplemente el formato de 16 bits en el modo de 32 bits.
 * Hay buenas razones de hardware para ello, pero el resultado final es un complejo esquema 
 * para especificar los modos de direccionamiento en los opcodes.
 */
typedef struct SIB {
    uint8_t    base:3; // Longitud del operando(size?)
    uint8_t   index:3; // direcion
    uint8_t   scale:2; 
} SIB;

/*
 * Cambios en 64 bits:
 * El byte ModR/M es fundamental para los cambios introducidos con la extensión de 64 bits de AMD al 
 * conjunto de instrucciones original. En modo largo, cualquier opcode cuyos cuatro bits más altos 
 * sean 0100 (decimal 4) se considera un nuevo prefijo, el prefijo REX
 * Los cuatro bits más bajos del byte de prefijo sirven para varios propósitos, 
 * incluyendo un bit extra para los campos REG y R/M del byte ModR/M que le sigue. 
 * Entre otros cambios, la ampliación de estos valores de tres bits a cuatro duplica el número 
 * de registros disponibles en el procesador de ocho a dieciséis.
 */
typedef struct Instruction {
    uint8_t prefix[4];  // ? // 4
    opcode  opcode[3];       // 7
    Mod_rm  Mod_rm;          // 8
    SIB     SIB;             // 9
    uint8_t displacement[4]; // desplazamiento de 0, 1, 2, 4 bytes
    uint8_t immediate[4];    // immediato de 0, 1, 2, 4 bytes
} Instruction;

/*
 *
 * El campo TTTN definen la condicion de la instruccion, en el caso de las instrucciones JCC (instrucciones de salto)
 * La instruccion JZ constiene como bit's tttn el valor 0100. siendo la codificacin del salto el  0111 tttn : 8-bit displacement
 * donde 0111 indica que se trata de las instruciones de salto y tttn siendo la condicion, 8-bit displacement siendo una cantidad
 * de bytes (un desplazamiento de 8bits) a saltar (maximos 0-255)
 * 
 * |-----------------------------------------------------------|
 * | tttn | Mnemonic |  Condition                              |  
 * |-----------------------------------------------------------|       
 * | 0000 | O        | Overflow                                |     
 * | 0001 | NO       | No overflow                             |         
 * | 0010 | B, NAE   | Below, Not above or equal               |                         
 * | 0011 | NB, AE   | Not below, Above or equal               |                         
 * | 0100 | E, Z     | Equal, Zero                             |         
 * | 0101 | NE, NZ   | Not equal, Not zero                     |                 
 * | 0110 | BE, NA   | Below or equal, Not above               |                         
 * | 0111 | NBE, A   | Not below or equal, Above               |                         
 * | 1000 | S        | Sign                                    | 
 * | 1001 | NS       | Not sign                                |     
 * | 1010 | P, PE    | Parity, Parity Even                     |                 
 * | 1011 | NP, PO   | Not parity, Parity Odd                  |                     
 * | 1100 | L, NGE   | Less than, Not greater than or equal to |                                     
 * | 1101 | NL, GE   | Not less than, Greater than or equal to |                                     
 * | 1110 | LE, NG   | Less than or equal to, Not greater than |                                     
 * | 1111 | NLE, G   | Not less than or equal to, Greater than |
 * |-----------------------------------------------------------|  
 */


typedef struct Instruction_info
{
    Instruction instruction; // estructura con informacion del opcode y la instruccion
    string_instrution_id string;      // string a obtener

    // 8 bits
    uint8_t position_rm:2;    // lugar donde se encuentra los bits "rm"  del opcode  -> 00(primer byte), 
                              //    01(segundo byte). 10(tercer byte), 4 (no hay campo rm)
    uint8_t position_reg:2;   // lugar donde se encuentra los bits "reg"  del opcode -> 00(primer byte), 
                              //    01(segundo byte). 10(tercer byte), 4 (no hay campo reg)
    uint8_t position_mod:2;   // lugar donde se encuentra los bits "mod"  del opcode -> 00(primer byte), 
                              //    01(segundo byte). 10(tercer byte), 4 (no hay campo mod)
    uint8_t position_tttn:2;  // lugar donde se encuentra los bits "tttn" del opcode -> 00(primer byte), 
                              //    01(segundo byte). 10(tercer byte), 4 (no hay campo tttn)
    uint8_t posicion_w:4;     /* Este campo indica la posicion del bit "w" es los 4bits posterioes de los primeros 4bits, posibles valores: 1000 (bit w en el bit 4), 
                                                                                                                                            0100 (bit w en el bit 3), 
                                                                                                                                            0010 (bit w en el bit 2), 
                                                                                                                                            0001 (bit w en el bit 1), 
                                                                                                                                            0000 (no hay bit w)
                                                                                                                                            Los demas estados no se definen */
    uint8_t posicion_d:4;     /* Este campo indica la posicion del bit "d" en los 4bits posterioes de los primeros 4bits, posibles valores: 1000 (bit d en el bit 4), 
                                                                                                                                                0100 (bit d en el bit 3), 
                                                                                                                                                0010 (bit d en el bit 2), 
                                                                                                                                                0001 (bit d en el bit 1), 
                                                                                                                                                0000 (no hay bit d)
                                                                                                                                                Los demas estados no se definen */

    uint8_t mask_rm;   // 8bits (16 bits)
    uint8_t mask_reg;  // 8bits (24 bits)
    uint8_t mask_mod;  // 8bits (32 bits)
    // 8bits (40 bits)
    uint8_t mask_tttn:6;  // 6bits para mascara tttn
    uint8_t number_reg:2; // algunas instrucciones no usan Mod/rm y usan este campo(R_m) directamente para almacenar el registro
                          // 0b00 - no usa campo reg
                          // 0b01 - usa un campo reg
                          // 0b10 - usa dos campos reg (rm se convierte en un campo registro)
                          // 0b11 - no se define

    // 8 bits (48 bits)
    uint8_t posicion_s:4;     /* Este campo indica la posicion del bit "s" es los 4bits posterioes de los primeros 4bits, posibles valores: 1000 (bit s en el bit 4), 
                                                                                                                                            0100 (bit s en el bit 3), 
                                                                                                                                            0010 (bit s en el bit 2), 
                                                                                                                                            0001 (bit s en el bit 1), 
                                                                                                                                            0000 (no hay bit s)
                                                                                                                                            Los demas estados no se definen */
 
    uint8_t opcode_size:2;    // tamaño del opcode de 1 a 3 bytes -> 00(1byte), 01(2byte). 10(3byte), 4(sin definir)
    uint8_t immediate_data:1; // datos inmediatos, 1 para si, 0 para no
    uint8_t immediate_instrution:1; // instruccion inmediata, 1 para si, 0 para no

} Instruction_info;

#pragma pack(pop)


inline uint8_t get_bit_w(Instruction_info *instrucion);
inline uint8_t get_bit_d(Instruction_info *instrucion);
inline uint64_t count_get_mask(uint64_t x);
static char *get_string_instrution(string_instrution_id id);
static char *get_string_register(encoder_x86 size_word, uint8_t bit_w, register_id id);
uint64_t popcnt_software(uint64_t x);


// (Instrucciones en page: 2845/2875) Intel® 64 and IA-32 Architectures Software Developer’s Manual, Combined Volumes: 1, 2A, 2B, 2C, 2D, 3A, 3B,

__attribute__((__section__(".instruccion"))) static Instruction_info my_instruccion[] = {
    {
        .string               = STRING_AAA,
        .immediate_instrution = 0b1,         // es una instruccion inmediata
        .opcode_size          = 0b00,        // un byte de opcode
        .posicion_w           = 0b0000,      // no hay bit "w"
        .posicion_s           = 0b0000,      // no hay bit "s"
        .position_rm          = 0b11,        // no hay r/m
        .position_reg         = 0b11,        // no hay reg
        .position_mod         = 0b11,        // no hay mod
        .position_tttn        = 0b11,        // no hay tttn
        .mask_mod             = 0b00000000,
        .mask_reg             = 0b00000000,
        .mask_rm              = 0b00000000,
        .mask_tttn            = 0b000000,
        .number_reg           = 0b00,         // 0b00 - no usa campo reg
        .immediate_data       = 0b0,          // no tiene datos inmediatos
        .instruction = { // AAA – ASCII Adjust after Addition
            .prefix = { 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
            .opcode = {
                (opcode){ .opcode_byte.byte = 0b00000000 }, 
                (opcode){ .opcode_byte.byte = 0b00000000 },
                (opcode){ .opcode_byte.byte = 0b00110111 }, // opcode primario
            },
            .Mod_rm = (Mod_rm){   .mod = 0b00,   .reg = 0b000,  .R_M = 0b000 },
            .SIB    =    (SIB){ .scale = 0b00, .index = 0b000, .base = 0b000 },
            .displacement = { 0b00000000, 0b00000000, 0b00000000, 0b00000000},
            .immediate    = { 0b00000000, 0b00000000, 0b00000000, 0b00000000}
        }
    },
    {
        .string               = STRING_AAD,
        .immediate_instrution = 0b1,         // es una instruccion inmediata
        .opcode_size          = 0b01,        // dos byte's de opcode
        .posicion_w           = 0b0000,      // no hay bit "w"
        .posicion_s           = 0b0000,      // no hay bit "s"
        .position_rm          = 0b11,        // no hay r/m
        .position_reg         = 0b11,        // no hay reg
        .position_mod         = 0b11,        // no hay mod
        .position_tttn        = 0b11,        // no hay tttn
        .mask_mod             = 0b00000000,
        .mask_reg             = 0b00000000,
        .mask_rm              = 0b00000000,
        .mask_tttn            = 0b000000,
        .number_reg           = 0b00,         // 0b00 - no usa campo reg
        .immediate_data       = 0b0,          // no tiene datos inmediatos
        .instruction = { // AAD – ASCII Adjust AX before Division
            .prefix = { 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
            .opcode = {
                (opcode){ .opcode_byte.byte = 0b00000000 },
                (opcode){ .opcode_byte.byte = 0b00001010 }, // opcode secundario
                (opcode){ .opcode_byte.byte = 0b11010101 }, // opcode primario
            },
            .Mod_rm = (Mod_rm){   .mod = 0b00,   .reg = 0b000,  .R_M = 0b000 },
            .SIB    =    (SIB){ .scale = 0b00, .index = 0b000, .base = 0b000 },
            .displacement = { 0b00000000, 0b00000000, 0b00000000, 0b00000000},
            .immediate    = { 0b00000000, 0b00000000, 0b00000000, 0b00000000}
        }
    },
    {
        .string               = STRING_AAM,
        .immediate_instrution = 0b1,         // es una instruccion inmediata
        .opcode_size          = 0b01,        // dos byte's de opcode
        .posicion_w           = 0b0000,      // no hay bit "w"
        .posicion_s           = 0b0000,      // no hay bit "s"
        .position_rm          = 0b11,        // no hay r/m
        .position_reg         = 0b11,        // no hay reg
        .position_mod         = 0b11,        // no hay mod
        .position_tttn        = 0b11,        // no hay tttn
        .mask_mod             = 0b00000000,
        .mask_reg             = 0b00000000,
        .mask_rm              = 0b00000000,
        .mask_tttn            = 0b000000,
        .number_reg           = 0b00,         // 0b00 - no usa campo reg
        .immediate_data       = 0b0,          // no tiene datos inmediatos
        .instruction = { // AAM – ASCII Adjust AX after Multiply
            .prefix = { 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
            .opcode = {
                (opcode){ .opcode_byte.byte = 0b00000000 },
                (opcode){ .opcode_byte.byte = 0b00001010 }, // opcode secundario
                (opcode){ .opcode_byte.byte = 0b11010100 }, // opcode primario
            },
            .Mod_rm = (Mod_rm){   .mod = 0b00,   .reg = 0b000,  .R_M = 0b000 },
            .SIB    =    (SIB){ .scale = 0b00, .index = 0b000, .base = 0b000 },
            .displacement = { 0b00000000, 0b00000000, 0b00000000, 0b00000000},
            .immediate    = { 0b00000000, 0b00000000, 0b00000000, 0b00000000}
        }
    },
    {
        .string               = STRING_AAS,
        .immediate_instrution = 0b1,         // es una instruccion inmediata
        .opcode_size          = 0b00,        // un byte de opcode
        .posicion_w           = 0b0000,      // no hay bit "w"
        .posicion_s           = 0b0000,      // no hay bit "s"
        .position_rm          = 0b11,        // no hay r/m
        .position_reg         = 0b11,        // no hay reg
        .position_mod         = 0b11,        // no hay mod
        .position_tttn        = 0b11,        // no hay tttn
        .mask_mod             = 0b00000000,
        .mask_reg             = 0b00000000,
        .mask_rm              = 0b00000000,
        .mask_tttn            = 0b000000,
        .number_reg           = 0b00,         // 0b00 - no usa campo reg
        .immediate_data       = 0b0,          // no tiene datos inmediatos
        .instruction = { // AAS – ASCII Adjust AL after Subtraction
            .prefix = { 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
            .opcode = {
                (opcode){ .opcode_byte.byte = 0b00000000 },
                (opcode){ .opcode_byte.byte = 0b00000000 }, // opcode secundario
                (opcode){ .opcode_byte.byte = 0b00111111 }, // opcode primario
            },
            .Mod_rm = (Mod_rm){   .mod = 0b00,   .reg = 0b000,  .R_M = 0b000 },
            .SIB    =    (SIB){ .scale = 0b00, .index = 0b000, .base = 0b000 },
            .displacement = { 0b00000000, 0b00000000, 0b00000000, 0b00000000},
            .immediate    = { 0b00000000, 0b00000000, 0b00000000, 0b00000000}
        }
    },
    {
        .string               = STRING_ADC,
        .immediate_instrution = 0b0,          // es una instruccion inmediata
        .opcode_size          = 0b01,         // dos byte's de opcode
        .posicion_w           = 0b0001,       // hay bit "w" en el bit 1
        .posicion_d           = 0b0010,       // hay bit "d" el bit 2 del opcode primario
        .posicion_s           = 0b0000,       // no hay bit "s"
        .position_rm          = 0b01,         // hay r/m en el segundo byte (se convierte en reg2 si number_reg == 0b10)
        .position_reg         = 0b01,         // hay reg en el segundo byte
        .position_mod         = 0b11,         // no hay mod
        .position_tttn        = 0b11,         // no hay tttn
        .mask_mod             = 0b00000000,   // -
        .mask_reg             = 0b00111000,   // con esta mascara se obtiene los bits 5, 4 y 3 de 7,6,5,4,3,2,1,0
        .mask_rm              = 0b00000111,   // con esta mascara se obtiene los bits 2, 1 y 0 de 7,6,5,4,3,2,1,0
        .mask_tttn            = 0b000000,
        .number_reg           = 0b10,         // 0b10 - usa dos campos reg (rm se convierte en un campo registro)
        .immediate_data       = 0b0,          // no tiene datos inmediatos
        .instruction = { // ADC – ADD with Carry
            .prefix = { 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
            .opcode = {
                (opcode){ .opcode_byte.byte = 0b00000000 },
                (opcode){ .opcode_byte.byte = 0b11000000 }, // opcode secundario
                (opcode){ .opcode_byte.byte = 0b00010000 }, // opcode primario
            },
            .Mod_rm = (Mod_rm){   .mod = 0b11,   .reg = 0b000,  .R_M = 0b000 },
            .SIB    =    (SIB){ .scale = 0b00, .index = 0b000, .base = 0b000 },
            .displacement = { 0b00000000, 0b00000000, 0b00000000, 0b00000000},
            .immediate    = { 0b00000000, 0b00000000, 0b00000000, 0b00000000}
        }
    }
};

typedef enum Prefix_x86_Segment_Register {
    Prefix_SS = 0x36, // 0x36 Prefijo de anulación del segmento SS (segmento de pila)
    Prefix_CS = 0x2E, // 0x2E Prefijo de anulación del segmento CS (segmento de codigo)
    Prefix_ES = 0x26, // 0x26 Prefijo de anulación del segmento ES
    Prefix_DS = 0x3E, // 0x3E Prefijo de anulación del segmento DS (segmento de datos)
    Prefix_FS = 0x64, // 0x64 Prefijo de anulación del segmento FS
    Prefix_GS = 0x65, // 0x65 Prefijo de anulación del segmento GS
} Prefix_x86_Segment_Register;

typedef enum Prefix_x86_others {
    Prefix_operand_size  = 0x66,                 /* 0x66
                                                  * Segun el manual de intel vol1, El prefijo de "anulación de tamaño de operando"(operand-size override) permite a un programa 
                                                  * cambiar entre tamaños de operando de 16 y 32 bits. Cualquiera de los dos tamaños puede ser el 
                                                  * predeterminado; el uso del prefijo selecciona el tamaño no predeterminado. en este caso el modo 
                                                  * no predeterminado seria 32bits, es por eso que decimos que sea este. No se puede cambiar de 16 o de 32 a 64 ni al reves 
                                                  * mediante este prefijo, se a de usar la correspondiente instruccion para esta tarea
                                                  */
    Prefix_addr_size     = 0x67,                  /* 0x67
                                                   * predefinido para el prefijo de "anulación de tamaño de dirección". Su significado es que si una instrucción ejecuta un tamaño 
                                                   * de dirección predeterminado, por ejemplo, 16 bits, el uso del prefijo permite que
                                                   * la instrucción utilice un tamaño de dirección distinto al predeterminado, por 
                                                   * ejemplo, 32 bits en modo de 16. Como se supone que el binario es de 16 bits, 0x67 
                                                   * cambia la instrucción al modo de 32 bits.
                                                   */
    Prefix_lock          = 0xF0,                   // 0xF0 Prefijo LOCK
    Prefix_repne_repnez  = 0xF2,                   // 0xF2 Prefijo REPNE/REPNZ (utilizado sólo con instrucciones de cadena)
    Prefix_repe_rep_repz = 0xF3,                   // 0xF3 Prefijo REP         (utilizado sólo con instrucciones de string)
                                                   // 0xF3 Prefijo REPE/REPZ   (sólo se utiliza con instrucciones de string)     
} Prefix_x86_Segment_others;

/*
 *
 *  Tablas "A" de la documentacion intel:
 *      - A02       = opcodes de 1byte de opcode
 *      - A03       = opcodes de 2byte de opcode (pagina: 2847) 
 *      - A04 a A05 = opcodes de 3byte de opcode (pagina: 2851)
 *      - A06       = extensiones para instrucciones de A2(de 1byte de opcode) y A3(2byte's de opcode)
 *      - A07       = opcode de instrucciones de punto flotante x87
 *      - A07 y A08 = contienen mapas para los opcodes de las instrucciones de escape que empiezan por 0xD8 (pagina: 2858)
 *      - A09       = ¿tabla de instrucciones FLD (double-real)?
 *      - A09 y A10 = contienen mapas para los opcodes de las instrucciones de escape que empiezan por 0xD9 (pagina: 2859)
 *      - A11 y A12 = contienen mapas para los opcodes de las instrucciones de escape que comienzan por 0xDA (pagina: 2860)
 * 
 */


#include "opcodes_prefix.c"
#endif 