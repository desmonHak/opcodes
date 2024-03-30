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
    uint8_t prefix[4]; // ?
    struct opcode {
        uint8_t opcode:6; // opcode de 1 byte
        uint8_t      d:1; /* 
                           * direcion:
                           * Si d=0, REG es la fuente, MOD R/M <- REG.
                           * Si d=1, REG es el destino, REG <- MOD R/M.
                           */
        uint8_t      s:1; /* 
                           * Longitud del operando(size?):
                           * Si s = 0, los operandos son registros de 8 bits y posiciones de memoria.
                           * Si s = 1, los operandos son de 16 bits o de 32 bits:
                           */
    } opcode[3];
    struct Mod_rm {
        uint8_t    mod:2; /*
                           * campo MOD:
                           *  - 00 Modo de direccionamiento indirecto de registro o SIB sin desplazamiento (cuando R/M = 100) o modo de direccionamiento de sólo desplazamiento (cuando R/M = 101).
                           *  - 01 El desplazamiento con signo de un byte sigue a los bytes del modo de direccionamiento.
                           *  - 10 El desplazamiento con signo de cuatro bytes sigue a los bytes del modo de direccionamiento.
                           *  - 11 Modo de direccionamiento de registro.
                           */
        uint8_t    reg:3; // direcion
        uint8_t    R_M:3; // Longitud del operando(size?)
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
    struct SIB {
        uint8_t   scale:2; 
        uint8_t   index:3; // direcion
        uint8_t    base:3; // Longitud del operando(size?)
    } SIB;
    uint8_t displacement[4]; // desplazamiento de 0, 1, 2, 4 bytes
    uint8_t    immediate[4]; // immediato de 0, 1, 2, 4 bytes
} Instruction;

// (page: 2845) Intel® 64 and IA-32 Architectures Software Developer’s Manual, Combined Volumes: 1, 2A, 2B, 2C, 2D, 3A, 3B,
typedef enum Opcodes_x86 {
    /*       0x0           0x1           0x2          0x3                0x4                0x5                          0x6                       0x7            0x8            0x9            0xA           0xB                 0xC         0xD             0xE              0xF       */
    ADD    = 0x00,                                                                                          PUSH_ES    = 0x06,            POP_ES = 0x07,     OR,                                                                                    PUSH_CS = 0x0E, TWO_BYTE = 0x0f, // 0x0
    ADC    = 0x10,                                                                                          PUSH_SS    = 0x16,            POP_ES = 0x17,    SBB,                                                                                    PUSH_DS = 0x1E, POP_DS,          // 0x1
    AND    = 0x20,                                                                                          _Prefix_ES = 0x26,            DAA,              SUB,                                                                                        _Prefix_CS, DAS,             // 0x2
    XOR    = 0x30,                                                                                          _Prefix_SS = 0x36,            AAA,              CMP,                                                                                        _Prefix_DS, ASS,             // 0x3
    INC    = 0x40,                                                                                                                                          DEC = 0x48,                                                                                                              // 0x4
    PUSH   = 0x50,                                                                                                                                          POP = 0x58,                                                                                                              // 0x5
    PUSHAD = 0x60, POPAD = 0x61, BOUND = 0x62, APRL = 0x63, _Prefix_FS = 0x64, _Prefix_GS = 0x65, _Prefix_operand_size = 0x66, _Prefix_addr_size = 0x67, PUSH_1 = 0x68, IMUL_1 = 0x69, PUSH_2 = 0x6a, IMUL_2 = 0x6b,        INS = 0x6c,                OUTS = 0x6e,                  // 0x6
    JO     = 0x70,          JNO,           JB,         JNB,                JE,               JNE,                         JBE,                       JA,            JS,           JNS,           JPE,           JPO,                JL, JGE = 0x7d,            JLE, JG,              // 0x7      
    // las instrucciones que llevan los bits 1001 0000 y son ADD, ADC, AND, XOR, OR, SBB, SUB, CMP, pertenezen al grupo de instrucciones inmediatas
    ADD_ADC_AND_XOR_OR_SBB_SUB_CMP = 0b10010000,                TEST_1 = 0x84,                                    XCHG = 0x86,                          MOV_REG = 0x88,                                              MOV_SREG_1 = 0x8c,        LEA,     MOV_SREG_2, POP,             // 0x8
    // XCHG_EAX_OR_NOP puede ser la instruccion XCHG EAX o un NOP
    XCHG_EAX_OR_NOP = 0x90, XCHG_EAX = 0x90,                                                                                                                CWD = 0x98,           CDQ,         CALLF,          WAIT,            PUSHFD,      POPFD,           SAHF, LAHF,            // 0x9
            MOV_EAX = 0xa0,                                       MOVS = 0xa4,                                    CMPS = 0xa6,                           TEST_2 = 0xa8,                  STOS = 0xaa,                      LODS = 0xac,                SCAS = 0xae,                  // 0xa
    // MOV byte inmediato en registro byte
                MOV = 0xb0,                                                                                                                                                                                                                                                          // 0xb
    //    Shift Grp 21A                                                                                           Grp 111A - MOV                                                                                                                            
          SHIFT_IMM = 0xc0,        RETN = 0xc2,                     LES = 0xc4,               LDS,                    MOV_IMM,                            ENTER = 0xc8,          LEAVE,         RETF,                      INT3 = 0xcc,     INT_IMM,           INTO, IRETD,          // 0xc
    //    Shift Grp 21A                                                                                                                                                                                                                 
} Opcodes_x86;

typedef enum Prefix_x86_Segment_Register {
    Prefix_SS = _Prefix_SS, // 0x36 Prefijo de anulación del segmento SS (segmento de pila)
    Prefix_CS = _Prefix_CS, // 0x2E Prefijo de anulación del segmento CS (segmento de codigo)
    Prefix_ES = _Prefix_ES, // 0x26 Prefijo de anulación del segmento ES
    Prefix_DS = _Prefix_DS, // 0x3E Prefijo de anulación del segmento DS (segmento de datos)
    Prefix_FS = _Prefix_FS, // 0x64 Prefijo de anulación del segmento FS
    Prefix_GS = _Prefix_GS, // 0x65 Prefijo de anulación del segmento GS
} Prefix_x86_Segment_Register;

typedef enum Prefix_x86_others {
    Prefix_operand_size  = _Prefix_operand_size, /* 0x66
                                                  * Segun el manual de intel vol1, El prefijo de "anulación de tamaño de operando"(operand-size override) permite a un programa 
                                                  * cambiar entre tamaños de operando de 16 y 32 bits. Cualquiera de los dos tamaños puede ser el 
                                                  * predeterminado; el uso del prefijo selecciona el tamaño no predeterminado. en este caso el modo 
                                                  * no predeterminado seria 32bits, es por eso que decimos que sea este. No se puede cambiar de 16 o de 32 a 64 ni al reves 
                                                  * mediante este prefijo, se a de usar la correspondiente instruccion para esta tarea
                                                  */
    Prefix_addr_size     = _Prefix_addr_size,     /* 0x67
                                                   * predefinido para el prefijo de "anulación de tamaño de dirección". Su significado es que si una instrucción ejecuta un tamaño 
                                                   * de dirección predeterminado, por ejemplo, 16 bits, el uso del prefijo permite que
                                                   * la instrucción utilice un tamaño de dirección distinto al predeterminado, por 
                                                   * ejemplo, 32 bits en modo de 16. Como se supone que el binario es de 16 bits, 0x67 
                                                   * cambia la instrucción al modo de 32 bits.
                                                   */
    Prefix_lock          = _Prefix_lock,          // 0xF0 Prefijo LOCK
    Prefix_repne_repnez  = _Prefix_repne_repnez,  // 0xF2 Prefijo REPNE/REPNZ (utilizado sólo con instrucciones de cadena)
    Prefix_repe_rep_repz = _Prefix_repe_rep_repz, // 0xF3 Prefijo REP         (utilizado sólo con instrucciones de string)
                                                  // 0xF3 Prefijo REPE/REPZ   (sólo se utiliza con instrucciones de string)     
} Prefix_x86_Segment_others;

#endif __OPCODES_PREFIX_H_