#ifndef __FORMAT_OPCODE_MAROS_H_
#define __FORMAT_OPCODE_MAROS_H_


typedef enum exception_format {
    
    /*
     * se usa en el mimebro mask mask_tttn con .position_tttn = 0b11
     * para indicar que la variante actual, solo opera con los registros
     * AL/AX/EAX.
     */
    EXCEPTION_AL_AX_EAX   = 0b000100,

    // indica que la instruccion solo usa registros de 16bits
    EXCEPTION_REGS_16BITS = 0b01010101,

    /*
     * indica que es una excepcion de tipo instruccion add
     * pues dicha instruccion al ocupara un opcode 0x00, necesita ser analizada de otra forma
     */
    EXCEPTION_ADD_OPCODE = 0b11111111,
} exception_format;

// tamaño del opcode de 1 a 3 bytes -> 00(1byte), 01(2byte). 10(3byte), 4(sin definir)
typedef enum number_bytes_opcode {

    // para opcodes de un byte:
    SIZE_OF_OPCODE_1_BYTE,

    // para opcodes de dos bytes:
    SIZE_OF_OPCODE_2_BYTE,

    // para opcodes de tres bytes:
    SIZE_OF_OPCODE_3_BYTE,

    // SIZE_OF_OPCODE_4_BYTES, // 4 bytes, no se utiliza en este caso.
    SIZE_OF_OPCODE_UNDEFINED_BYTE
} number_bytes_opcode;

/*
 * algunas instrucciones no usan Mod/rm y usan este campo(R_m) directamente para almacenar el 
 * registro 0b00 
 * - no usa campo reg 0b01 
 * - usa un campo reg 0b10 
 * - usa dos campos reg (rm se convierte en un campo registro) 
 * - 0b11 - no se define
 */
typedef enum number_registers {

    // instruccion sin registros
    INSTRUCTION_OF_ZERO_REGISTER,

    // instruccion con un solo registro
    INSTRUCTION_OF_ONE_REGISTER,

    // instruccion con dos solo registro
    INSTRUCTION_OF_TWO_REGISTER,

    INSTRUCTION_OF_UNDEFINED_REGISTER
} number_registers;

/*
 * 0100 (bit d en el bit 3).
 * 0010 (bit d en el bit 2).
 * 0001 (bit d en el bit 1).
 * 0000 (no hay bit d) Los demas estados no se definen.
 */
typedef enum positions_bit_d {
    d_not_in_bit       = 0b0000,
    d_in_bit_1         = 0b0001,
    d_in_bit_2         = 0b0010,
    d_in_bit_3         = 0b0100,
    d_in_bit_undefined = 0b1000,
} positions_bit_d;

/*
 * 0100 (bit w en el bit 3).
 * 0010 (bit w en el bit 2).
 * 0001 (bit w en el bit 1).
 * 0000 (no hay bit w) Los demas estados no se definen.
 */
typedef enum positions_bit_w {
    w_not_in_bit       = 0b0000,
    w_in_bit_1         = 0b0001,
    w_in_bit_2         = 0b0010,
    w_in_bit_3         = 0b0100,
    w_in_bit_undefined = 0b1000,
} positions_bit_w;

/*
 * 0100 (bit s en el bit 3).
 * 0010 (bit s en el bit 2).
 * 0001 (bit s en el bit 1).
 * 0000 (no hay bit d) Los demas estados no se definen.
 */
typedef enum positions_bit_s {
    s_not_in_bit       = 0b0000,
    s_in_bit_1         = 0b0001,
    s_in_bit_2         = 0b0010,
    s_in_bit_3         = 0b0100,
    s_in_bit_undefined = 0b1000,
} positions_bit_s;

/*
 * lugar donde se encuentra los bits "tttn" del opcode.
 * 00 (primer byte).
 * 01 (segundo byte).
 * 10 (tercer byte).
 * 11 (no hay campo tttn). (se activa las excepciones de instrucciones)
 */
typedef enum positions_bit_tttn {
    tttn_in_byte_1         = 0b00,
    tttn_in_byte_2         = 0b01,
    tttn_in_byte_3         = 0b10,

    // se usa para indicar que no es una instruccion de salto pero ademas para activar las excepciones
    NOT_JMP_ACTIVATE_EXCEPTIONS = 0b11,
} positions_bit_tttn;

/*
 * lugar donde se encuentra los bits "mod"  del opcode.
 * 00 (primer byte).
 * 01 (segundo byte).
 * 10 (tercer byte).
 * 11 (no hay campo mod).
 */
typedef enum positions_byte_mod {
    mod_in_byte_1         = 0b00,
    mod_in_byte_2         = 0b01,
    mod_in_byte_3         = 0b10,

    // se usa para indicar que no hay campo MOD
    NOT_MOD = 0b11,
} positions_byte_mod;

/*
 * lugar donde se encuentra los bits "reg"  del opcode.
 * 00 (primer byte).
 * 01 (segundo byte).
 * 10 (tercer byte).
 * 11 (no hay campo mod).
 */
typedef enum positions_byte_reg {
    reg_in_byte_1         = 0b00,
    reg_in_byte_2         = 0b01,
    reg_in_byte_3         = 0b10,

    // se usa para indicar que no hay campo REG
    NOT_REG = 0b11,
} positions_byte_reg;

/*
 * lugar donde se encuentra los bits "r/m"  del opcode.
 * 00 (primer byte).
 * 01 (segundo byte).
 * 10 (tercer byte).
 * 11 (no hay campo mod).
 */
typedef enum positions_byte_rm {
    rm_in_byte_1         = 0b00,
    rm_in_byte_2         = 0b01,
    rm_in_byte_3         = 0b10,

    // se usa para indicar que no hay campo rm
    NOT_RM = 0b11,
} positions_byte_rm;

// se usa para indicar que los datos son inmediatos 
#define HAS_IMMEDIATE_DATA 0b1

// se usa para indicar que los datos no son inmediatos 
#define NO_IMMEDIATE_DATA  0b0

// se usa para indicar que la instruccion es inmediata
#define IMMEDIATE_INSTRUCTION 0b1

// se usa para indicar que instruccion no es inmediata 
#define NO_IMMEDIATE_INSTRUCTION  0b0

// no tiene prefijo
#define NOT_PREFIX 0x0

// no tiene opcode
#define NOT_OPCODE 0x0

// no se definio aun el valor
#define NOT_VALUE 0x0

#define NOT_TTTN 0b000000

#endif