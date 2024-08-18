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

#include "format_opcode_macros.h"
#include "debug_c.h"

typedef enum string_instrution_id {

    // AAA – ASCII Adjust after Addition 0011 0111
    STRING_AAA,

    // AAD – ASCII Adjust AX before Division 1101 0101 : 0000 1010
    STRING_AAD,

    // AAM – ASCII Adjust AX after Multiply 1101 0100 : 0000 1010
    STRING_AAM,

    // AAS – ASCII Adjust AL after Subtraction 0011 1111
    STRING_AAS,

    /*
     * ADC – ADD with Carry.
     * register1 to register2 0001 000w : 11 reg1 reg2.
     * register2 to register1 0001 001w : 11 reg1 reg2.
     * memory to register 0001 001w : mod reg r/m.
     * register to memory 0001 000w : mod reg r/m.
     * immediate to register 1000 00sw : 11 010 reg : immediate data.
     * immediate to AL, AX, or EAX 0001 010w : immediate data.
     * immediate to memory 1000 00sw : mod 010 r/m : immediate data.
     */
    STRING_ADC,

    /*
     * ADD – Add.
     * register1 to register2 0000 000w : 11 reg1 reg2.
     * register2 to register1 0000 001w : 11 reg1 reg2.
     * memory to register 0000 001w : mod reg r/m.
     * register to memory 0000 000w : mod reg r/m.
     * immediate to register 1000 00sw : 11 000 reg : immediate data.
     * immediate to AL, AX, or EAX 0000 010w : immediate data.
     * immediate to memory 1000 00sw : mod 000 r/m : immediate data.
     */
    STRING_ADD,

    /*
     * AND – Logical AND.
     * register1 to register2 0010 000w : 11 reg1 reg2.
     * register2 to register1 0010 001w : 11 reg1 reg2.
     * memory to register 0010 001w : mod reg r/m.
     * register to memory 0010 000w : mod reg r/m.
     * immediate to register 1000 00sw : 11 100 reg : immediate data.
     * immediate to AL, AX, or EAX 0010 010w : immediate data.
     * immediate to memory 1000 00sw : mod 100 r/m : immediate data.
     */
    STRING_AND,

    /*
     * ARPL – Adjust RPL Field of Selector.
     * from register 0110 0011 : 11 reg1 reg2.
     * from memory 0110 0011 : mod reg r/m.
     */
    STRING_ARPL,

    // BOUND – Check Array Against Bounds 0110 0010 : modA reg r/m
    STRING_BOUND,

    /*
     * BSF – Bit Scan Forward.
     * register1, register2 0000 1111 : 1011 1100 : 11 reg1 reg2.
     * memory, register 0000 1111 : 1011 1100 : mod reg r/m.
     */
    STRING_BSF,

    /*
     * BSR – Bit Scan Reverse.
     * register1, register2 0000 1111 : 1011 1101 : 11 reg1 reg2.
     * memory, register 0000 1111 : 1011 1101 : mod reg r/m.
     */
    STRING_BSR,

    // BSWAP – Byte Swap 0000 1111 : 1100 1 reg
    STRING_BSWAP,

    /*
     * BT – Bit Test.
     * register, immediate 0000 1111 : 1011 1010 : 11 100 reg: imm8 data.
     * memory, immediate 0000 1111 : 1011 1010 : mod 100 r/m : imm8 data.
     * register1, register2 0000 1111 : 1010 0011 : 11 reg2 reg1.
     * memory, reg 0000 1111 : 1010 0011 : mod reg r/m.
     */
    STRING_BT,

    /*
     * BTC – Bit Test and Complement.
     * register, immediate 0000 1111 : 1011 1010 : 11 111 reg: imm8 data.
     * memory, immediate 0000 1111 : 1011 1010 : mod 111 r/m : imm8 data.
     * register1, register2 0000 1111 : 1011 1011 : 11 reg2 reg1.
     * memory, reg 0000 1111 : 1011 1011 : mod reg r/m.
     */
    STRING_BTC,

    /*
     * BTR – Bit Test and Reset,
     * register, immediate 0000 1111 : 1011 1010 : 11 110 reg: imm8 data,
     * memory, immediate 0000 1111 : 1011 1010 : mod 110 r/m : imm8 data,
     * register1, register2 0000 1111 : 1011 0011 : 11 reg2 reg1,
     * memory, reg 0000 1111 : 1011 0011 : mod reg r/m,
     */
    STRING_BTR,

    /*
     * BTS – Bit Test and Set.
     * register, immediate 0000 1111 : 1011 1010 : 11 101 reg: imm8 data.
     * memory, immediate 0000 1111 : 1011 1010 : mod 101 r/m : imm8 data.
     * register1, register2 0000 1111 : 1010 1011 : 11 reg2 reg1.
     * memory, reg 0000 1111 : 1010 1011 : mod reg r/m.
     */
    STRING_BTS,

    /*
     * CALL – Call Procedure (in same segment).
     * direct 1110 1000 : full displacement.
     * register indirect 1111 1111 : 11 010 reg.
     * memory indirect 1111 1111 : mod 010 r/m.
     */
    STRING_CALL,      

    /*
     * CALL – Call Procedure (in other segment).
     * direct 1001 1010 : unsigned full offset, selector.
     * indirect 1111 1111 : mod 011 r/m.
     */
    STRING_CALL_FAR, 

    // CBW – Convert Byte to Word 1001 1000
    STRING_CBW,

    // CDQ – Convert Doubleword to Qword 1001 1001
    STRING_CDQ,

    // CLC – Clear Carry Flag 1111 1000
    STRING_CLC,

    // CLD – Clear Direction Flag 1111 1100
    STRING_CLD,

    // CLI – Clear Interrupt Flag 1111 1010
    STRING_CLI,

    // CLTS – Clear Task-Switched Flag in CR0 0000 1111 : 0000 0110
    STRING_CLTS,

    // CMC – Complement Carry Flag 1111 0101
    STRING_CMC,

    /*
     * CMP – Compare Two Operands.
     * register1 with register2 0011 100w : 11 reg1 reg2.
     * register2 with register1 0011 101w : 11 reg1 reg2.
     * memory with register 0011 100w : mod reg r/m.
     * register with memory 0011 101w : mod reg r/m.
     * immediate with register 1000 00sw : 11 111 reg : immediate data.
     * immediate with AL, AX, or EAX 0011 110w : immediate data.
     * immediate with memory 1000 00sw : mod 111 r/m : immediate data.
     */
    STRING_CMP,

    // CMPS/CMPSB/CMPSW/CMPSD – Compare String Operands 1010 011w
    STRING_CMPS, 
                STRING_CMPSB = STRING_CMPS,
                STRING_CMPSW = STRING_CMPS,
                STRING_CMPSD = STRING_CMPS,
    
    /*
     * CMPXCHG – Compare and Exchange.
     * register1, register2 0000 1111 : 1011 000w : 11 reg2 reg1.
     * memory, register 0000 1111 : 1011 000w : mod reg r/m.
     */
    STRING_CMPXCHG,

    // CPUID – CPU Identification 0000 1111 : 1010 0010
    STRING_CPUID,

    // CWD – Convert Word to Doubleword 1001 1001
    STRING_CWD,

    // CWDE – Convert Word to Doubleword 1001 1000
    STRING_CWDE,

    // DAA – Decimal Adjust AL after Addition 0010 0111
    STRING_DAA,

    // DAS – Decimal Adjust AL after Subtraction 0010 1111
    STRING_DAS,

    /*
     * DEC – Decrement by 1.
     * register 1111 111w : 11 001 reg.
     * register (alternate encoding) 0100 1 reg.
     * memory 1111 111w : mod 001 r/m.
     */
    STRING_DEC,

    /*
     * DIV – Unsigned Divide.
     * AL, AX, or EAX by register 0100 000B 1111 011w : 11 110 reg.
     * Divide RDX:RAX by qwordregister 0100 100B 1111 0111 : 11 110 qwordreg.
     * AL, AX, or EAX by memory 0100 00XB 1111 011w : mod 110 r/m.
     * Divide RDX:RAX by memory64 0100 10XB 1111 0111 : mod 110 r/m.
     */
    STRING_DIV,

    // HLT – Halt 1111 0100
    STRING_HLT,

    /* 
     * IDIV – Signed Divide.
     * AL, AX, or EAX by register 1111 011w : 11 111 reg.
     * AL, AX, or EAX by memory 1111 011w : mod 111 r/m.
     */
    STRING_IDIV,

    /*
     * IMUL – Signed Multiply.
     * AL, AX, or EAX with register 1111 011w : 11 101 reg.
     * AL, AX, or EAX with memory 1111 011w : mod 101 reg.
     * register1 with register2 0000 1111 : 1010 1111 : 11 : reg1 reg2.
     * register with memory 0000 1111 : 1010 1111 : mod reg r/m.
     * register1 with immediate to register2 0110 10s1 : 11 reg1 reg2 : immediate data.
     * memory with immediate to register 0110 10s1 : mod reg r/m : immediate data.
     */
    STRING_IMUL,

    /*
     * IN – Input From Port.
     * fixed port 1110 010w : port number.
     * variable port 1110 110w.
     */
    STRING_IN,

    /*
     * INC – Increment by 1.
     * reg 1111 111w : 11 000 reg.
     * reg (alternate encoding) 0100 0 reg.
     * memory 1111 111w : mod 000 r/m.
     */
    STRING_INC,

    // INS – Input from DX Port 0110 110w
    STRING_INS,

    // INT n – Interrupt Type n 1100 1101 : type
    STRING_INT_n, 

    // INT   – Single-Step Interrupt 3 1100 1100
    STRING_INT_3, 

    // INTO – Interrupt 4 on Overflow 1100 1110
    STRING_INTO,  

    // INVD – Invalidate Cache 0000 1111 : 0000 1000
    STRING_INVD,

    // INVLPG – Invalidate TLB Entry 0000 1111 : 0000 0001 : mod 111 r/m
    STRING_INVLPG,

    // INVPCID – Invalidate Process-Context Identifier 0110 0110:0000 1111:0011 1000:1000 0010: mod reg r/m
    STRING_INVPCID,

    // IRET/IRETD – Interrupt Return 1100 1111
    STRING_IRET,
        STRING_IRETD = STRING_IRET,

    /*
     * Jcc – Jump if Condition is Met.
     * 8-bit displacement 0111 tttn : 8-bit displacement.
     * full displacement 0000 1111 : 1000 tttn : full displacement.
     */
    STRING_JCC,

    /*
     * JCXZ/JECXZ – Jump on CX/ECX Zero
     *  Address-size prefix differentiates JCXZ
     *  and JECXZ.
     * 1110 0011 : 8-bit displacement.
     */
    STRING_JCXZ,
        STRING_JECXZ = STRING_JCXZ,

    /*
     * JMP – Unconditional Jump (to same segment).
     * short 1110 1011 : 8-bit displacement.
     * direct 1110 1001 : full displacement.
     * register indirect 1111 1111 : 11 100 reg.
     * memory indirect 1111 1111 : mod 100 r/m.
     */
    STRING_JMP,    

    /*
     * JMP – Unconditional Jump (to other segment).
     * direct intersegment 1110 1010 : unsigned full offset, selector.
     * indirect intersegment 1111 1111 : mod 101 r/m.
     */  
    STRING_JMP_FAR,  
    
    // LAHF – Load Flags into AHRegister 1001 1111
    STRING_LAHF,

    /*
     * LAR – Load Access Rights Byte.
     * from register 0000 1111 : 0000 0010 : 11 reg1 reg2.
     * from memory 0000 1111 : 0000 0010 : mod reg r/m.
     */
    STRING_LAR,

    // LDS – Load Pointer to DS 1100 0101 : modA,B reg r/m
    STRING_LDS,

    // LEA – Load Effective Address 1000 1101 : modA reg r/m
    STRING_LEA,

    // LEAVE – High Level Procedure Exit 1100 1001
    STRING_LEAVE,

    // LES – Load Pointer to ES 1100 0100 : modA,B reg r/m
    STRING_LES,

    // LFS – Load Pointer to FS 0000 1111 : 1011 0100 : modA reg r/m
    STRING_LFS,

    // LGDT – Load Global Descriptor Table Register 0000 1111 : 0000 0001 : modA 010 r/
    STRING_LGDT,

    // LGS – Load Pointer to GS 0000 1111 : 1011 0101 : modA reg r/m
    STRING_LGS,

    // LIDT – Load Interrupt Descriptor Table Register 0000 1111 : 0000 0001 : modA 011 r/m
    STRING_LIDT,

    /*
     * LLDT – Load Local Descriptor Table Register.
     * LDTR from register 0000 1111 : 0000 0000 : 11 010 reg.
     * LDTR from memory 0000 1111 : 0000 0000 : mod 010 r/m.
     */
    STRING_LLDT,

    /*
     * LMSW – Load Machine Status Word.
     * from register 0000 1111 : 0000 0001 : 11 110 reg.
     * from memory 0000 1111 : 0000 0001 : mod 110 r/m.
     */
    STRING_LMSW,

    // LOCK – Assert LOCK# Signal Prefix 1111 0000    
    STRING_LOCK,

    // LODS/LODSB/LODSW/LODSD – Load String Operand 1010 110w
    STRING_LODS,
        STRING_LODSB = STRING_LODS,
        STRING_LODSW = STRING_LODS,
        STRING_LODD  = STRING_LODS,

    // LOOP – Loop Count 1110 0010 : 8-bit displacement
    STRING_LOOP,

    // LOOPZ/LOOPE – Loop Count while Zero/Equal 1110 0001 : 8-bit displacement
    STRING_LOOPZ,
        STRING_LOOPE = STRING_LOOPZ,
        
    // LOOPNZ/LOOPNE – Loop Count while not Zero/Equal 1110 0000 : 8-bit displacement
    STRING_LOOPNZ,
        STRING_LOOPNE = STRING_LOOPNZ,

    /*
     * LSL – Load Segment Limit.
     * from register 0000 1111 : 0000 0011 : 11 reg1 reg2.
     * from memory 0000 1111 : 0000 0011 : mod reg r/m.
     */
    STRING_LSL,

    // LSS – Load Pointer to SS 0000 1111 : 1011 0010 : modA reg r/m
    STRING_LSS,

    /*
     * LTR – Load Task Register.
     * from register 0000 1111 : 0000 0000 : 11 011 reg.
     * from memory 0000 1111 : 0000 0000 : mod 011 r/m.
     */
    STRING_LTR,

    /*
     * MOV – Move Data.
     * register1 to register2 1000 100w : 11 reg1 reg2.
     * register2 to register1 1000 101w : 11 reg1 reg2.
     * memory to reg 1000 101w : mod reg r/m.
     * reg to memory 1000 100w : mod reg r/m.
     * immediate to register 1100 011w : 11 000 reg : immediate data.
     * immediate to register (alternate encoding) 1011 w reg : immediate data.
     * immediate to memory 1100 011w : mod 000 r/m : immediate data.
     * memory to AL, AX, or EAX 1010 000w : full displacement.
     * AL, AX, or EAX to memory 1010 001w : full displacement.
     * 
     * MOV – Move to/from Control Registers.
     * CR0 from register 0000 1111 : 0010 0010 : -- 000 reg.
     * CR2 from register 0000 1111 : 0010 0010 : -- 010reg.
     * CR3 from register 0000 1111 : 0010 0010 : -- 011 reg.
     * CR4 from register 0000 1111 : 0010 0010 : -- 100 reg.
     * register from CR0-CR4 0000 1111 : 0010 0000 : -- eee reg.
     * 
     * MOV – Move to/from Debug Registers.
     * DR0-DR3 from register 0000 1111 : 0010 0011 : -- eee reg.
     * DR4-DR5 from register 0000 1111 : 0010 0011 : -- eee reg.
     * DR6-DR7 from register 0000 1111 : 0010 0011 : -- eee reg.
     * register from DR6-DR7 0000 1111 : 0010 0001 : -- eee reg.
     * register from DR4-DR5 0000 1111 : 0010 0001 : -- eee reg.
     * register from DR0-DR3 0000 1111 : 0010 0001 : -- eee reg.
     * 
     * MOV – Move to/from Segment Registers.
     * register to segment register 1000 1110 : 11 sreg3 reg.
     * register to SS 1000 1110 : 11 sreg3 reg.
     * memory to segment reg 1000 1110 : mod sreg3 r/m.
     * memory to SS 1000 1110 : mod sreg3 r/m.
     * segment register to register 1000 1100 : 11 sreg3 reg.
     * segment register to memory 1000 1100 : mod sreg3 r/m.
    */
    STRING_MOV,

    /*
     * MOVBE – Move data after swapping bytes.
     * memory to register 0000 1111 : 0011 1000:1111 0000 : mod reg r/m.
     * register to memory 0000 1111 : 0011 1000:1111 0001 : mod reg r/m.
     */
    STRING_MOVBE,

    // MOVS/MOVSB/MOVSW/MOVSD – Move Data from String to String 1010 010w
    STRING_MOVS,
        STRING_MOVSB = STRING_MOVS,
        STRING_MOVSW = STRING_MOVS,
        STRING_MOVSD = STRING_MOVS,

    /*
     * MOVSX – Move with Sign-Extend.
     * memory to reg 0000 1111 : 1011 111w : mod reg r/m.
     */
    STRING_MOVSX,

    /*
     * MOVZX – Move with Zero-Extend.
     * register2 to register1 0000 1111 : 1011 011w : 11 reg1 reg2.
     * memory to register 0000 1111 : 1011 011w : mod reg r/m.
     */
    STRING_MOVZX,

    /*
     * MUL – Unsigned Multiply.
     * AL, AX, or EAX with register 1111 011w : 11 100 reg.
     * AL, AX, or EAX with memory 1111 011w : mod 100 r/m.
     */
    STRING_MUL,

    /*
     * NEG – Two's Complement Negation.
     * register 1111 011w : 11 011 reg.
     * memory 1111 011w : mod 011 r/m.
     */
    STRING_NEG,

    /*
     * NOP – No Operation 1001 0000.
     * NOP – Multi-byte No Operation1.
     * register 0000 1111 0001 1111 : 11 000 reg.
     * memory 0000 1111 0001 1111 : mod 000 r/m.
     */
    STRING_NOP,

    /* 
     * NOT – One's Complement Negation.
     * register 1111 011w : 11 010 reg.
     * memory 1111 011w : mod 010 r/m.
     */
    STRING_NOT,

    /*
     * OR – Logical Inclusive OR.
     * register1 to register2 0000 100w : 11 reg1 reg2.
     * register2 to register1 0000 101w : 11 reg1 reg2.
     * memory to register 0000 101w : mod reg r/m.
     * register to memory 0000 100w : mod reg r/m.
     * immediate to register 1000 00sw : 11 001 reg : immediate data.
     * immediate to AL, AX, or EAX 0000 110w : immediate data.
     * immediate to memory 1000 00sw : mod 001 r/m : immediate data.
     */
    STRING_OR,

    /*
     * OUT – Output to Port.
     * fixed port 1110 011w : port number.
     * variable port 1110 111w.
     */
    STRING_OUT,

    // OUTS – Output to DX Port 0110 111w
    STRING_OUTS,

    /*
     * POP – Pop a Word from the Stack.
     * register 1000 1111 : 11 000 reg.
     * register (alternate encoding) 0101 1 reg.
     * memory 1000 1111 : mod 000 r/m.
     * 
     * POP – Pop a Segment Register from the Stack (Note: CS cannot be sreg2 in this usage.).
     * segment register DS, ES 000 sreg2 111.
     * segment register SS 000 sreg2 111.
     * segment register FS, GS 0000 1111: 10 sreg3 001.
     */
    STRING_POP,

    // POPA/POPAD – Pop All General Registers 0110 0001
    STRING_POPA,
        STRING_POPAD = STRING_POPA,

    // POPF/POPFD – Pop Stack into FLAGS or EFLAGS Register 1001 1101
    STRING_POPF,
        STRING_POPFD = STRING_POPF,

    /*
     * PUSH – Push Operand onto the Stack.
     * register 1111 1111 : 11 110 reg.
     * register (alternate encoding) 0101 0 reg.
     * memory 1111 1111 : mod 110 r/m.
     * immediate 0110 10s0 : immediate data.
     * 
     * PUSH – Push Segment Register onto the Stack.
     * segment register CS,DS,ES,SS 000 sreg2 110.
     * segment register FS,GS 0000 1111: 10 sreg3 000.
     */
    STRING_PUSH,

    // PUSHA/PUSHAD – Push All General Registers 0110 0000
    STRING_PUSHA,
        STRING_PUSHAD = STRING_PUSHA,

    // PUSHF/PUSHFD – Push Flags Register onto the Stack 1001 1100
    STRING_PUSHF,
        STRING_PUSHFD = STRING_PUSHF,
    
    /*
     * RCL – Rotate thru Carry Left.
     * register by 1 1101 000w : 11 010 reg.
     * memory by 1 1101 000w : mod 010 r/m.
     * register by CL 1101 001w : 11 010 reg.
     * memory by CL 1101 001w : mod 010 r/m.
     * register by immediate count 1100 000w : 11 010 reg : imm8 data.
     * memory by immediate count 1100 000w : mod 010 r/m : imm8 data.
     */
    STRING_RCL,

    /*
     * RCR – Rotate thru Carry Right.
     * register by 1 1101 000w : 11 011 reg.
     * memory by 1 1101 000w : mod 011 r/m.
     * register by CL 1101 001w : 11 011 reg.
     * memory by CL 1101 001w : mod 011 r/m.
     * register by immediate count 1100 000w : 11 011 reg : imm8 data.
     * memory by immediate count 1100 000w : mod 011 r/m : imm8 data.
     */
    STRING_RCR,

    // RDMSR – Read from Model-Specific Register 0000 1111 : 0011 0010
    STRING_RDMSR,

    // RDPMC – Read Performance Monitoring Counters 0000 1111 : 0011 0011
    STRING_RDPMC,

    // RDTSC – Read Time-Stamp Counter 0000 1111 : 0011 0001
    STRING_RDTSC,

    // RDTSCP – Read Time-Stamp Counter and Processor ID 0000 1111 : 0000 0001: 1111 1001
    STRING_RDTSCP,

    // REP INS – Input String 1111 0011 : 0110 110w
    STRING_REP_INS,

    // REP LODS – Load String 1111 0011 : 1010 110w
    STRING_REP_LODS,

    // REP MOVS – Move String 1111 0011 : 1010 010w
    STRING_REP_MOVS,

    // REP OUTS – Output String 1111 0011 : 0110 111w
    STRING_REP_OUTS,

    // REP STOS – Store String 1111 0011 : 1010 101w
    STRING_REP_STOS,

    // REPE CMPS – Compare String 1111 0011 : 1010 011w
    STRING_REPE_CMPS,

    // REPE SCAS – Scan String 1111 0011 : 1010 111w
    STRING_REPE_SCAS,

    // REPNE CMPS – Compare String 1111 0010 : 1010 011w
    STRING_REPNE_CMPS,

    // REPNE SCAS – Scan String 1111 0010 : 1010 111w
    STRING_REPNE_SCAS,

    /*
     * RET – Return from Procedure (to same segment).
     * no argument 1100 0011.
     * adding immediate to SP 1100 0010 : 16-bit displacement.
     */
    STRING_RET,

    /*
     * RET – Return from Procedure (to other segment).
     * intersegment           -> 1100 1011.
     * adding immediate to SP -> 1100 1010 : 16-bit displacement.
     */
    STRING_RETF,

    /*
     * ROL – Rotate Left.
     * register by 1 1101 000w : 11 000 reg.
     * memory by 1 1101 000w : mod 000 r/m.
     * register by CL 1101 001w : 11 000 reg.
     * memory by CL 1101 001w : mod 000 r/m.
     * register by immediate count 1100 000w : 11 000 reg : imm8 data.
     * memory by immediate count 1100 000w : mod 000 r/m : imm8 data.
     */
    STRING_ROL,

    /*
     * ROR – Rotate Right.
     * register by 1 1101 000w : 11 001 reg.
     * memory by 1 1101 000w : mod 001 r/m.
     * register by CL 1101 001w : 11 001 reg.
     * memory by CL 1101 001w : mod 001 r/m.
     * register by immediate count 1100 000w : 11 001 reg : imm8 data.
     * memory by immediate count 1100 000w : mod 001 r/m : imm8 data.
     */
    STRING_ROR,

    // RSM – Resume from System Management Mode 0000 1111 : 1010 1010
    STRING_RSM,

    // SAHF – Store AH into Flags 1001 1110
    STRING_SAHF,

    // SAL – Shift Arithmetic Left same instruction as SHL
    STRING_SAL,

    /*
     * SAR – Shift Arithmetic Right.
     * register by 1 1101 000w : 11 111 reg.
     * memory by 1 1101 000w : mod 111 r/m.
     * register by CL 1101 001w : 11 111 reg.
     * memory by CL 1101 001w : mod 111 r/m.
     * register by immediate count 1100 000w : 11 111 reg : imm8 data.
     * memory by immediate count 1100 000w : mod 111 r/m : imm8 data.
     */
    STRING_SAR,

    /*
     * SBB – Integer Subtraction with Borrow.
     * register1 to register2 0001 100w : 11 reg1 reg2.
     * register2 to register1 0001 101w : 11 reg1 reg2.
     * memory to register 0001 101w : mod reg r/m.
     * register to memory 0001 100w : mod reg r/m.
     * immediate to register 1000 00sw : 11 011 reg : immediate data.
     * immediate to AL, AX, or EAX 0001 110w : immediate data.
     * immediate to memory 1000 00sw : mod 011 r/m : immediate data.
     */
    STRING_SBB,

    // SCAS/SCASB/SCASW/SCASD – Scan String 1010 111w
    STRING_SCAS,
        STRING_SCASB = STRING_SCAS,
        STRING_SCASW = STRING_SCAS,
        STRING_SCASD = STRING_SCAS,
    
    /*
     * SETcc – Byte Set on Condition.
     * register 0000 1111 : 1001 tttn : 11 000 reg.
     * memory 0000 1111 : 1001 tttn : mod 000 r/m.
     */
    STRING_SETcc,

    // SGDT – Store Global Descriptor Table Register 0000 1111 : 0000 0001 : modA 000 r/m
    STRING_SGDT,

    /*
     * SHL – Shift Left.
     * register by 1 1101 000w : 11 100 reg.
     * memory by 1 1101 000w : mod 100 r/m.
     * register by CL 1101 001w : 11 100 reg.
     * memory by CL 1101 001w : mod 100 r/m.
     * register by immediate count 1100 000w : 11 100 reg : imm8 data.
     * memory by immediate count 1100 000w : mod 100 r/m : imm8 data.
     */
    STRING_SHL,

    /*
     * SHLD – Double Precision Shift Left.
     * register by immediate count 0000 1111 : 1010 0100 : 11 reg2 reg1 : imm8.
     * memory by immediate count 0000 1111 : 1010 0100 : mod reg r/m : imm8.
     * register by CL 0000 1111 : 1010 0101 : 11 reg2 reg1.
     * memory by CL 0000 1111 : 1010 0101 : mod reg r/m.
     */
    STRING_SHLD,

    /*
     * SHR – Shift Right.
     * register by 1 1101 000w : 11 101 reg.
     * memory by 1 1101 000w : mod 101 r/m.
     * register by CL 1101 001w : 11 101 reg.
     * memory by CL 1101 001w : mod 101 r/m.
     * register by immediate count 1100 000w : 11 101 reg : imm8 data.
     * memory by immediate count 1100 000w : mod 101 r/m : imm8 data.
     */
    STRING_SHR,

    /*
     * SHRD – Double Precision Shift Right.
     * register by immediate count 0000 1111 : 1010 1100 : 11 reg2 reg1 : imm8.
     * memory by immediate count 0000 1111 : 1010 1100 : mod reg r/m : imm8.
     * register by CL 0000 1111 : 1010 1101 : 11 reg2 reg1.
     * memory by CL 0000 1111 : 1010 1101 : mod reg r/m.
     */
    STRING_SHRD,

    // SIDT – Store Interrupt Descriptor Table Register 0000 1111 : 0000 0001 : modA 001 r/m
    STRING_SIDT,

    /*
     * SLDT – Store Local Descriptor Table Register.
     * to register 0000 1111 : 0000 0000 : 11 000 reg.
     * to memory 0000 1111 : 0000 0000 : mod 000 r/m.
     */
    STRING_SLDT,

    /*
     * SMSW – Store Machine Status Word.
     * to register 0000 1111 : 0000 0001 : 11 100 reg.
     * to memory 0000 1111 : 0000 0001 : mod 100 r/m.
    */
    STRING_SMSW,

    // STC – Set Carry Flag 1111 1001
    STRING_STC,

    // STD – Set Direction Flag 1111 1101
    STRING_STD,

    // STI – Set Interrupt Flag 1111 1011
    STRING_STI,

    // STOS/STOSB/STOSW/STOSD – Store String Data 1010 101w
    STRING_STOS,
        STRING_STOSB = STRING_STOS,
        STRING_STOSW = STRING_STOS,
        STRING_STOSD = STRING_STOS,

    /*
     * STR – Store Task Register.
     * to register 0000 1111 : 0000 0000 : 11 001 reg.
     * to memory 0000 1111 : 0000 0000 : mod 001 r/m.
     */
    STRING_STR,

    /*
     * SUB – Integer Subtraction.
     * register1 to register2 0010 100w : 11 reg1 reg2.
     * register2 to register1 0010 101w : 11 reg1 reg2.
     * memory to register 0010 101w : mod reg r/m.
     * register to memory 0010 100w : mod reg r/m.
     * immediate to register 1000 00sw : 11 101 reg : immediate data.
     * immediate to AL, AX, or EAX 0010 110w : immediate data.
     * immediate to memory 1000 00sw : mod 101 r/m : immediate data.
     */
    STRING_SUB,

    /*
     * TEST – Logical Compare.
     * register1 and register2 1000 010w : 11 reg1 reg2.
     * memory and register 1000 010w : mod reg r/m.
     * immediate and register 1111 011w : 11 000 reg : immediate data.
     * immediate and AL, AX, or EAX 1010 100w : immediate data.
     * immediate and memory 1111 011w : mod 000 r/m : immediate data.
     */
    STRING_TEST,
    
    /*
     * UD0 – Undefined instruction 0000 1111 : 1111 1111. -> 0f ff.
     * UD1 – Undefined instruction 0000 1111 : 1011 1001. -> 0f b9.
     * UD2 – Undefined instruction 0000 1111 : 0000 1011. -> 0f 0b.
     */
    STRING_UD,

    /*
     * VERR – Verify a Segment for Reading.
     * register 0000 1111 : 0000 0000 : 11 100 reg.
     * memory 0000 1111 : 0000 0000 : mod 100 r/m.
     */
    STRING_VERR,

    /*
     * VERW – Verify a Segment for Writing.
     * register 0000 1111 : 0000 0000 : 11 101 reg.
     * memory 0000 1111 : 0000 0000 : mod 101 r/m.
    */
    STRING_VERW,

    // WAIT – Wait 1001 1011
    STRING_WAIT,

    // WBINVD – Writeback and Invalidate Data Cache 0000 1111 : 0000 1001
    STRING_WBINVD,

    // WRMSR – Write to Model-Specific Register 0000 1111 : 0011 0000
    STRING_WRMSR,

    /*
     * XADD – Exchange and Add.
     * register1, register2 0000 1111 : 1100 000w : 11 reg2 reg1.
     * memory, reg 0000 1111 : 1100 000w : mod reg r/m.
     */
    STRING_XADD,

    /*
     * XCHG – Exchange Register/Memory with Register.
     * register1 with register2 1000 011w : 11 reg1 reg2.
     * AX or EAX with reg 1001 0 reg.
     * memory with reg 1000 011w : mod reg r/m.
     * XLAT/XLATB – Table Look-up Translation 1101 0111.
    */
    STRING_XCHG,

    /*
     * XOR – Logical Exclusive OR.
     * register1 to register2 0011 000w : 11 reg1 reg2.
     * register2 to register1 0011 001w : 11 reg1 reg2.
     * memory to register 0011 001w : mod reg r/m.
     * register to memory 0011 000w : mod reg r/m.
     * immediate to register 1000 00sw : 11 110 reg : immediate data.
     * immediate to AL, AX, or EAX 0011 010w : immediate data.
     * immediate to memory 1000 00sw : mod 110 r/m : immediate data.
    */
    STRING_XOR
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

typedef union opcode // estructura para representar los opcodes
{
    struct opcode_bits_final {
        uint8_t      s:1;   /* 
                            * Longitud del operando(size?):
                            * Si s = 0, los operandos son registros de 8 bits y posiciones de memoria.
                            * Si s = 1, los operandos son de 16 bits o de 32 bits:
                            */
        uint8_t      d:1;   /* 
                             * direcion:
                             * Si d=0, REG es la fuente, MOD R/M <- REG.
                             * Si d=1, REG es el destino, REG <- MOD R/M.
                             */
        uint8_t opcode:6;    // opcode de 1 byte
    } opcode_bits_final;     // no siempre se da este formato en las instrucciones
    struct opcode_bits {
        uint8_t b1:1; uint8_t b2:1; uint8_t b3:1; uint8_t b4:1; 
        uint8_t b5:1; uint8_t b6:1; uint8_t b7:1; uint8_t b8:1; 
    } opcode_bits;          // acceso a los campos de bit a bit (para los casos en los que el campo "w", "d", "s", puedan estar en diferentes ubicaciones)

    struct opcode_byte {
        uint8_t byte;       // acceder al opcode en forma de byte 
    } opcode_byte;          // acceso a todos los campos en forma de byte
} opcode;                   

typedef struct Mod_rm { // estructura para representar el mod/rm (¿Register/Memory?)
    uint8_t    R_M:3; /* 
                       * campo ¿Registro/Memoria?  (reg2?).
                       * dependiendo de la instruccion este campo se usa como indicar de un segundo registro fuente o destino.
                       * En caso contrario se usa para el acceso a memoria o indicar el uso de un campo SIB.
                       * 
                       * Formas con el Byte ModR/M solo para 32bits:
                       * |===|===|=================================.
                       * |MOD|R/M|     Addressing Mode             . 
                       * |===|===|=================================.
                       * | 00|000|[ eax ]                          .            
                       * | 01|000|[ eax + disp8 ]               (1).                   
                       * | 10|000|[ eax + disp32 ]                 .  
                       * | 11|000|register  ( al / ax / eax )   (2).
                       * | 00|001|[ ecx ]                          .            
                       * | 01|001|[ ecx + disp8 ]                  . 
                       * | 10|001|[ ecx + disp32 ]                 .                     
                       * | 11|001|register  ( cl / cx / ecx )      .             
                       * | 00|010|[ edx ]                          .            
                       * | 01|010|[ edx + disp8 ]                  .                                                          
                       * | 10|010|[ edx + disp32 ]                 .                     
                       * | 11|010|register  ( dl / dx / edx )      .                                
                       * | 00|011|[ ebx ]                          .            
                       * | 01|011|[ ebx + disp8 ]                  .                    
                       * | 10|011|[ ebx + disp32 ]                 .                     
                       * | 11|011|register  ( bl / bx / ebx )      .             
                       * | 00|100|SIB  Mode                     (3).                   
                       * | 01|100|SIB  +  disp8  Mode              .     
                       * | 10|100|SIB  +  disp32  Mode             .                         
                       * | 11|100|register  ( ah / sp / esp )      .             
                       * | 00|101|32-bit Displacement-Only Mode (4).                   
                       * | 01|101|[ ebp + disp8 ]                  .                    
                       * | 10|101|[ ebp + disp32 ]                 .                     
                       * | 11|101|register  ( ch / bp / ebp )      .             
                       * | 00|110|[ esi ]                          .            
                       * | 01|110|[ esi + disp8 ]                  .                    
                       * | 10|110|[ esi + disp32 ]                 .                     
                       * | 11|110|register  ( dh / si / esi )      .                                
                       * | 00|111|[ edi ]                          .            
                       * | 01|111|[ edi + disp8 ]                  .                    
                       * | 10|111|[ edi + disp32 ]                 .                     
                       * | 11|111|register  ( bh / di / edi )      .     
                       * |===|===|=================================. 
                       */
    uint8_t    reg:3; /* 
                       * campo registro (reg1?).
                       * - 000 -> al(si w = 0) / ax(si w = 1) / eax (32bits) / rax.
                       * - 001 -> cl(si w = 0) / cx(si w = 1) / ecx (32bits) / rcx.
                       * - 010 -> dl(si w = 0) / dx(si w = 1) / edx (32bits) / rdx.
                       * - 011 -> bl(si w = 0) / bx(si w = 1) / ebx (32bits) / rbx.
                       * - 100 -> ah(si w = 0) / sp(si w = 1) / esp (32bits) / rsp.
                       * - 101 -> ch(si w = 0) / bp(si w = 1) / ebp (32bits) / rbp.
                       * - 110 -> dh(si w = 0) / si(si w = 1) / esi (32bits) / rsi.
                       * - 111 -> bh(si w = 0) / di(si w = 1) / edi (32bits) / rdi.
                       */
    uint8_t    mod:2; /*
                       * campo MOD:
                       *  - 00 Modo de direccionamiento indirecto de registro o SIB sin desplazamiento (cuando R/M = 100) o 
                       *        modo de direccionamiento de sólo desplazamiento (cuando R/M = 101).
                       *  - 01 El desplazamiento con signo de un byte sigue a los bytes del modo de direccionamiento.
                       *  - 10 El desplazamiento con signo de cuatro/dos bytes sigue a los bytes del modo de direccionamiento.
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
 * 
 * En cada modo de direccionamiento indexado escalado, el campo MOD del byte MOD-REG-R/M especifica el tamaño del desplazamiento. Puede ser cero, uno o cuatro bytes:
 *     MOD R/M Modo de direccionamiento.
 *     --- --- ---------------------------.
 *      00 100 SIB.
 *      01 100 SIB + disp8.
 *      10 100 SIB + disp32.
 * 
 *  [ reg32 + eax*n ] MOD = 00.
 *  [ reg32 + ebx*n ].
 *  [ reg32 + ecx*n ].
 *  [ reg32 + edx*n ].
 *  [ reg32 + ebp*n ].
 *  [ reg32 + esi*n ].
 *  [ reg32 + edi*n ].
 *  
 *  [ disp + reg8 + eax*n ] MOD = 01.
 *  [ disp + reg8 + ebx*n ].
 *  [ disp + reg8 + ecx*n ].
 *  [ disp + reg8 + edx*n ].
 *  [ disp + reg8 + ebp*n ].
 *  [ disp + reg8 + esi*n ].
 *  [ disp + reg8 + edi*n ].
 *  
 *  [ disp + reg32 + eax*n ] MOD = 10.
 *  [ disp + reg32 + ebx*n ].
 *  [ disp + reg32 + ecx*n ].
 *  [ disp + reg32 + edx*n ].
 *  [ disp + reg32 + ebp*n ].
 *  [ disp + reg32 + esi*n ].
 *  [ disp + reg32 + edi*n ].
 *  
 *  [ disp + eax*n ] MOD = 00, and.
 *  [ disp + ebx*n ] BASE field = 101.
 *  [ disp + ecx*n ].
 *  [ disp + edx*n ].
 *  [ disp + ebp*n ].
 *  [ disp + esi*n ].
 *  [ disp + edi*n ].
 * 
 */
typedef struct SIB {
    uint8_t    base:3; /*
                        * BASE (3bits 2 a 0):  
                        *   bit(000) -> eax. 
                        *   bit(001) -> ecx.
                        *   bit(010) -> edx.
                        *   bit(011) -> ebx.
                        *   bit(100) -> esp.
                        *   bit(101) -> ebp -> si MOD = 01 o 10. sino, solo si MOD es 00.
                        *   bit(110) -> esi.
                        *   bit(111) -> edi.
                        */
    uint8_t   index:3; /*
                        * INDEX (3bits 5 a 3):
                        *   bit(000) ->  eax.
                        *   bit(001) ->  ecx.
                        *   bit(010) ->  edx.
                        *   bit(011) ->  ebx.
                        *   bit(100) -> ilegal.
                        *   bit(101) ->  ebp.
                        *   bit(110) ->  esi.
                        *   bit(111) ->  edi.
                        */
    uint8_t   scale:2; /*
                        * SCALE (2bits 7 a 6):
                        *   bit(00)  -> index * 1.
                        *   bit(01)  -> index * 2.
                        *   bit(10)  -> index * 4.
                        *   bit(11)  -> index * 8.
                        */
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
    uint8_t mask_tttn:6;  // 6bits para mascara tttn. si position_tttn indica que no hay bits tttn, pero este campo no esta a 0x0
                          // entonces dicho campo podra usarse para identificar otro tipo de instrucciones.
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


static inline uint8_t get_bit_w(Instruction_info *instrucion);
static inline uint8_t get_bit_d(Instruction_info *instrucion);
static inline uint64_t count_get_mask(uint64_t x);
static inline uint64_t popcnt_software(uint64_t x);
static char *get_string_instruction_by_id(string_instrution_id id);

#define get_string_register(size_word, bit_w, id) get_string_mod_3(size_word, bit_w, id)
static char *get_string_mod_0(encoder_x86 size_word, Instruction_info *my_instruccion_);
static char *get_string_mod_1(encoder_x86 size_word, Instruction_info *my_instruccion_);
static char *get_string_mod_2(encoder_x86 size_word, Instruction_info *my_instruccion_);
static char *get_string_mod_3(encoder_x86 size_word, uint8_t bit_w, register_id id);

// (Instrucciones en page: 2845/2875) Intel® 64 and IA-32 Architectures Software Developer’s Manual, Combined Volumes: 1, 2A, 2B, 2C, 2D, 3A, 3B,

__attribute__((__section__(".instruccion"))) static Instruction_info my_instruccion[] = {
    {
        .string               = STRING_AAS,
        .immediate_instrution = 0b1,         // es una instruccion inmediata
        .opcode_size          = 0b00,        // un byte de opcode
        .posicion_w           = 0b0000,      // no hay bit "w"
        .posicion_s           = 0b0000,      // no hay bit "s"
        .position_rm          = 0b11,        // no hay r/m
        .position_reg         = 0b11,        // no hay reg
        .position_mod         = 0b11,        // no hay mod
        .position_tttn        = NOT_JMP_ACTIVATE_EXCEPTIONS,        // no hay tttn
        .mask_mod             = 0b00000000,
        .mask_reg             = 0b00000000,
        .mask_rm              = 0b00000000,
        .mask_tttn            = NOT_TTTN,
        .number_reg           = 0b00,         // 0b00 - no usa campo reg
        .immediate_data       = NO_IMMEDIATE_DATA,          // no tiene datos inmediatos
        .instruction = { // AAS – ASCII Adjust AL after Subtraction
            .prefix = { 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
            .opcode = {
                (opcode){ .opcode_byte.byte = NOT_OPCODE },
                (opcode){ .opcode_byte.byte = NOT_OPCODE }, // opcode secundario
                (opcode){ .opcode_byte.byte = 0b00111111 }, // opcode primario
            },
            .Mod_rm = (Mod_rm){   .mod = NOT_VALUE,   .reg = NOT_VALUE,  .R_M = NOT_VALUE },
            .SIB    =    (SIB){ .scale = NOT_VALUE, .index = NOT_VALUE, .base = NOT_VALUE },
            .displacement = { NOT_VALUE, NOT_VALUE, NOT_VALUE, NOT_VALUE},
            .immediate    = { NOT_VALUE, NOT_VALUE, NOT_VALUE, NOT_VALUE}
        }
    },
    {
        .string               = STRING_AAA,
        .immediate_instrution = 0b1,         // es una instruccion inmediata
        .opcode_size          = 0b00,        // un byte de opcode
        .posicion_w           = 0b0000,      // no hay bit "w"
        .posicion_s           = 0b0000,      // no hay bit "s"
        .position_rm          = 0b11,        // no hay r/m
        .position_reg         = 0b11,        // no hay reg
        .position_mod         = 0b11,        // no hay mod
        .position_tttn        = NOT_JMP_ACTIVATE_EXCEPTIONS,        // no hay tttn
        .mask_mod             = 0b00000000,
        .mask_reg             = 0b00000000,
        .mask_rm              = 0b00000000,
        .mask_tttn            = NOT_TTTN,
        .number_reg           = 0b00,         // 0b00 - no usa campo reg
        .immediate_data       = NO_IMMEDIATE_DATA,          // no tiene datos inmediatos
        .instruction = { // AAA – ASCII Adjust after Addition
            .prefix = { 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
            .opcode = {
                (opcode){ .opcode_byte.byte = NOT_OPCODE }, 
                (opcode){ .opcode_byte.byte = NOT_OPCODE },
                (opcode){ .opcode_byte.byte = 0b00110111 }, // opcode primario
            },
            .Mod_rm = (Mod_rm){   .mod = NOT_VALUE,   .reg = NOT_VALUE,  .R_M = NOT_VALUE },
            .SIB    =    (SIB){ .scale = NOT_VALUE, .index = NOT_VALUE, .base = NOT_VALUE },
            .displacement = { NOT_VALUE, NOT_VALUE, NOT_VALUE, NOT_VALUE},
            .immediate    = { NOT_VALUE, NOT_VALUE, NOT_VALUE, NOT_VALUE}
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
        .position_tttn        = NOT_JMP_ACTIVATE_EXCEPTIONS,        // no hay tttn
        .mask_mod             = 0b00000000,
        .mask_reg             = 0b00000000,
        .mask_rm              = 0b00000000,
        .mask_tttn            = NOT_TTTN,
        .number_reg           = 0b00,         // 0b00 - no usa campo reg
        .immediate_data       = NO_IMMEDIATE_DATA,          // no tiene datos inmediatos
        .instruction = { // AAS – ASCII Adjust AL after Subtraction
            .prefix = { 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
            .opcode = {
                (opcode){ .opcode_byte.byte = NOT_OPCODE },
                (opcode){ .opcode_byte.byte = NOT_OPCODE }, // opcode secundario
                (opcode){ .opcode_byte.byte = 0b00111111 }, // opcode primario
            },
            .Mod_rm = (Mod_rm){   .mod = NOT_VALUE,   .reg = NOT_VALUE,  .R_M = NOT_VALUE },
            .SIB    =    (SIB){ .scale = NOT_VALUE, .index = NOT_VALUE, .base = NOT_VALUE },
            .displacement = { NOT_VALUE, NOT_VALUE, NOT_VALUE, NOT_VALUE},
            .immediate    = { NOT_VALUE, NOT_VALUE, NOT_VALUE, NOT_VALUE}
        }
    },
    {
        .string               = STRING_AAD,
        .immediate_instrution = 0b1,         // es una instruccion inmediata
        .opcode_size          = SIZE_OF_OPCODE_2_BYTE,        // dos byte's de opcode
        .posicion_w           = 0b0000,      // no hay bit "w"
        .posicion_s           = 0b0000,      // no hay bit "s"
        .position_rm          = 0b11,        // no hay r/m
        .position_reg         = 0b11,        // no hay reg
        .position_mod         = 0b11,        // no hay mod
        .position_tttn        = NOT_JMP_ACTIVATE_EXCEPTIONS,        // no hay tttn
        .mask_mod             = 0b00000000,
        .mask_reg             = 0b00000000,
        .mask_rm              = 0b00000000,
        .mask_tttn            = NOT_TTTN,
        .number_reg           = 0b00,         // 0b00 - no usa campo reg
        .immediate_data       = NO_IMMEDIATE_DATA,          // no tiene datos inmediatos
        .instruction = { // AAD – ASCII Adjust AX before Division
            .prefix = { 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
            .opcode = {
                (opcode){ .opcode_byte.byte = NOT_OPCODE },
                (opcode){ .opcode_byte.byte = 0b00001010 }, // opcode secundario
                (opcode){ .opcode_byte.byte = 0b11010101 }, // opcode primario
            },
            .Mod_rm = (Mod_rm){   .mod = NOT_VALUE,   .reg = NOT_VALUE,  .R_M = NOT_VALUE },
            .SIB    =    (SIB){ .scale = NOT_VALUE, .index = NOT_VALUE, .base = NOT_VALUE },
            .displacement = { NOT_VALUE, NOT_VALUE, NOT_VALUE, NOT_VALUE},
            .immediate    = { NOT_VALUE, NOT_VALUE, NOT_VALUE, NOT_VALUE}
        }
    },
    {
        .string               = STRING_AAM,
        .immediate_instrution = 0b1,         // es una instruccion inmediata
        .opcode_size          = SIZE_OF_OPCODE_2_BYTE,        // dos byte's de opcode
        .posicion_w           = 0b0000,      // no hay bit "w"
        .posicion_s           = 0b0000,      // no hay bit "s"
        .position_rm          = 0b11,        // no hay r/m
        .position_reg         = 0b11,        // no hay reg
        .position_mod         = 0b11,        // no hay mod
        .position_tttn        = NOT_JMP_ACTIVATE_EXCEPTIONS,        // no hay tttn
        .mask_mod             = 0b00000000,
        .mask_reg             = 0b00000000,
        .mask_rm              = 0b00000000,
        .mask_tttn            = NOT_TTTN,
        .number_reg           = 0b00,         // 0b00 - no usa campo reg
        .immediate_data       = NO_IMMEDIATE_DATA,          // no tiene datos inmediatos
        .instruction = { // AAM – ASCII Adjust AX after Multiply
            .prefix = { 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
            .opcode = {
                (opcode){ .opcode_byte.byte = NOT_OPCODE },
                (opcode){ .opcode_byte.byte = 0b00001010 }, // opcode secundario
                (opcode){ .opcode_byte.byte = 0b11010100 }, // opcode primario
            },
            .Mod_rm = (Mod_rm){   .mod = NOT_VALUE,   .reg = NOT_VALUE,  .R_M = NOT_VALUE },
            .SIB    =    (SIB){ .scale = NOT_VALUE, .index = NOT_VALUE, .base = NOT_VALUE },
            .displacement = { NOT_VALUE, NOT_VALUE, NOT_VALUE, NOT_VALUE},
            .immediate    = { NOT_VALUE, NOT_VALUE, NOT_VALUE, NOT_VALUE}
        }
    },
    {
        .string               = STRING_ADC,
        .immediate_instrution = NO_IMMEDIATE_INSTRUCTION,          // no es una instruccion inmediata
        .opcode_size          = SIZE_OF_OPCODE_2_BYTE,         // dos byte's de opcode
        .posicion_w           = w_in_bit_1,       // hay bit "w" en el bit 1
        .posicion_d           = 0b0010,       // hay bit "d" el bit 2 del opcode primario
        .posicion_s           = 0b0000,       // no hay bit "s"
        .position_rm          = rm_in_byte_2,         // hay r/m en el segundo byte 
        .position_reg         = reg_in_byte_2,         // hay reg en el segundo byte
        .position_mod         = mod_in_byte_2,         // hay campo mod en el segundo byte
        .position_tttn        = NOT_JMP_ACTIVATE_EXCEPTIONS,         // no hay tttn
        .mask_mod             = 0b11000000,   // los bits mod's estan en los primeros 2 bits del segundo byte de opcode
        .mask_reg             = 0b00111000,   // con esta mascara se obtiene los bits 5, 4 y 3 de 7,6,5,4,3,2,1,0
        .mask_rm              = 0b00000111,   // con esta mascara se obtiene los bits 2, 1 y 0 de 7,6,5,4,3,2,1,0
        .mask_tttn            = 0b000100,     // al ser el position_tttn 11, quiere decir que no es una instruccion de salto
        // por tanto podemos usar mask tttn para identificar instrucciones diferentes a los saltos,
        // en este caso se usara para identicar esta variante:
        // immediate to AL, AX, or EAX 0001 010w : immediate data
        // siendo el bit tttn en este caso el 00 0100 de la instruccion, asi podemos identificar esta variante
        
        .number_reg           = INSTRUCTION_OF_ONE_REGISTER,         // 0b01 - usa un campo reg 
        .immediate_data       = NO_IMMEDIATE_DATA,          // no tiene datos inmediatos
        .instruction = { // ADC – ADD with Carry
            .prefix = { 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
            .opcode = {
                (opcode){ .opcode_byte.byte = NOT_OPCODE },
                (opcode){ .opcode_byte.byte = NOT_OPCODE }, // opcode secundario
                (opcode){ .opcode_byte.byte = 0b00010000 }, // opcode primario
            },
            .Mod_rm = (Mod_rm){   .mod = NOT_VALUE,   .reg = NOT_VALUE,  .R_M = NOT_VALUE },
            .SIB    =    (SIB){ .scale = NOT_VALUE, .index = NOT_VALUE, .base = NOT_VALUE },
            .displacement = { NOT_VALUE, NOT_VALUE, NOT_VALUE, NOT_VALUE},
            .immediate    = { NOT_VALUE, NOT_VALUE, NOT_VALUE, NOT_VALUE}
        }
    },
    {
        // immediate to memory   1000 00sw : mod 010 r/m : immediate data
        // immediate to register 1000 00sw : 11  010 reg : immediate data
        .string               = STRING_ADC,
        .immediate_instrution = NO_IMMEDIATE_INSTRUCTION,          // es una instruccion inmediata
        .opcode_size          = SIZE_OF_OPCODE_2_BYTE,         // dos byte's de opcode
        .posicion_w           = w_in_bit_1,       // hay bit "w" en el bit 1
        .posicion_d           = d_not_in_bit,       // no hay bit 
        .posicion_s           = s_in_bit_2,       // hay bit "s" el bit 2 del opcode primario
        .position_rm          = rm_in_byte_2,         // hay r/m en el segundo byte 
        .position_reg         = reg_in_byte_2,         // hay reg en el segundo byte
        .position_mod         = mod_in_byte_2,         // hay campo mod en el segundo byte
        .position_tttn        = NOT_JMP_ACTIVATE_EXCEPTIONS,         // no hay tttn
        .mask_mod             = 0b11000000,   // los bits mod's estan en los primeros 2 bits del segundo byte de opcode
        .mask_reg             = 0b00111000,   // con esta mascara se obtiene los bits 5, 4 y 3 de 7,6,5,4,3,2,1,0
        .mask_rm              = 0b00000111,   // con esta mascara se obtiene los bits 2, 1 y 0 de 7,6,5,4,3,2,1,0
        .mask_tttn            = NOT_TTTN,
        .number_reg           = INSTRUCTION_OF_ONE_REGISTER,         // 0b10 - usa un campo reg 
        .immediate_data       = HAS_IMMEDIATE_DATA,          // tiene datos inmediatos
        .instruction = { // ADC – ADD with Carry
            .prefix = { 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
            .opcode = {
                (opcode){ .opcode_byte.byte = NOT_OPCODE },
                (opcode){ .opcode_byte.byte = 0b00010000 }, // opcode secundario
                (opcode){ .opcode_byte.byte = 0b10000000 }, // opcode primario
            },
            .Mod_rm = (Mod_rm){   .mod = NOT_VALUE,   .reg = NOT_VALUE,  .R_M = NOT_VALUE },
            .SIB    =    (SIB){ .scale = NOT_VALUE, .index = NOT_VALUE, .base = NOT_VALUE },
            .displacement = { NOT_VALUE, NOT_VALUE, NOT_VALUE, NOT_VALUE},
            .immediate    = { NOT_VALUE, NOT_VALUE, NOT_VALUE, NOT_VALUE}
        }
    },
    {
        // AND
        // immediate to register 1000 00sw : 11 100 reg : immediate data
        // immediate to memory 1000 00sw : mod 100 r/m : immediate data
        .string               = STRING_AND,
        .immediate_instrution = NO_IMMEDIATE_INSTRUCTION,          // es una instruccion inmediata
        .opcode_size          = SIZE_OF_OPCODE_2_BYTE,         // dos byte's de opcode
        .posicion_w           = w_in_bit_1,       // hay bit "w" en el bit 1
        .posicion_d           = d_not_in_bit,       // no hay bit "d" 
        .posicion_s           = s_in_bit_2,       // hay bit "s" en el segundo bit
        .position_rm          = rm_in_byte_2,         // hay r/m en el segundo byte 
        .position_reg         = reg_in_byte_2,         // hay reg en el segundo byte
        .position_mod         = mod_in_byte_2,         // hay campo mod en el segundo byte
        .position_tttn        = NOT_JMP_ACTIVATE_EXCEPTIONS,         // no hay tttn
        .mask_mod             = 0b11000000,   // los bits mod's estan en los primeros 2 bits del segundo byte de opcode
        .mask_reg             = 0b00111000,   // con esta mascara se obtiene los bits 5, 4 y 3 de 7,6,5,4,3,2,1,0
        .mask_rm              = 0b00000111,   // con esta mascara se obtiene los bits 2, 1 y 0 de 7,6,5,4,3,2,1,0
        .mask_tttn            = NOT_TTTN,
        .number_reg           = INSTRUCTION_OF_ONE_REGISTER,         // 0b10 - usa un campo reg 
        .immediate_data       = HAS_IMMEDIATE_DATA,          // tiene datos inmediatos
        .instruction = { // 
            .prefix = { 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
            .opcode = {
                (opcode){ .opcode_byte.byte = NOT_OPCODE },
                (opcode){ .opcode_byte.byte = 0b00100000 }, // opcode secundario
                (opcode){ .opcode_byte.byte = 0b10000000 }, // opcode primario
            },
            .Mod_rm = (Mod_rm){   .mod = NOT_VALUE,   .reg = NOT_VALUE,  .R_M = NOT_VALUE },
            .SIB    =    (SIB){ .scale = NOT_VALUE, .index = NOT_VALUE, .base = NOT_VALUE },
            .displacement = { NOT_VALUE, NOT_VALUE, NOT_VALUE, NOT_VALUE},
            .immediate    = { NOT_VALUE, NOT_VALUE, NOT_VALUE, NOT_VALUE}
        }
    },
    {
        // add
        // register1 to register2 0000 000w : 11 reg1 reg2
        // register2 to register1 0000 001w : 11 reg1 reg2
        // memory to register 0000 001w : mod reg r/m
        // register to memory 0000 000w : mod reg r/m
        .string               = STRING_ADD,
        .immediate_instrution = NO_IMMEDIATE_INSTRUCTION,          // es una instruccion inmediata
        .opcode_size          = SIZE_OF_OPCODE_1_BYTE,         // dos byte's de opcode
        .posicion_w           = w_in_bit_1,       // hay bit "w" en el bit 1
        .posicion_d           = d_in_bit_2,       // hay bit "d" el bit 2 del opcode primario
        .posicion_s           = s_in_bit_undefined,       // hay bit "s" en el segundo bit
        .position_rm          = rm_in_byte_2,         // hay r/m en el segundo byte 
        .position_reg         = reg_in_byte_2,         // hay reg en el segundo byte
        .position_mod         = mod_in_byte_2,         // hay campo mod en el segundo byte
        .position_tttn        = NOT_JMP_ACTIVATE_EXCEPTIONS,         // no hay tttn
        .mask_mod             = 0b11000000,   // los bits mod's estan en los primeros 2 bits del segundo byte de opcode
        .mask_reg             = 0b00111000,   // con esta mascara se obtiene los bits 5, 4 y 3 de 7,6,5,4,3,2,1,0
        .mask_rm              = 0b00000111,   // con esta mascara se obtiene los bits 2, 1 y 0 de 7,6,5,4,3,2,1,0
        .mask_tttn            = EXCEPTION_AL_AX_EAX,     // al ser el position_tttn 11, quiere decir que no es una instruccion de salto
        // por tanto podemos usar mask tttn para identificar instrucciones diferentes a los saltos,
        // en este caso se usara para identicar esta variante:
        // immediate to AL, AX, or EAX 0000 010w : immediate data
        // siendo el bit tttn en este caso el 00 0100 de la instruccion, asi podemos identificar esta variante
        
        .number_reg           = INSTRUCTION_OF_ONE_REGISTER,         // 0b10 - usa un campo reg 
        .immediate_data       = NO_IMMEDIATE_DATA,          // tiene datos inmediatos
        .instruction = { // ADC – ADD with Carry
            .prefix = { NOT_PREFIX, NOT_PREFIX, NOT_PREFIX, NOT_PREFIX },
            .opcode = {
                (opcode){ .opcode_byte.byte = EXCEPTION_ADD_OPCODE },
                (opcode){ .opcode_byte.byte = EXCEPTION_ADD_OPCODE }, // opcode secundario
                // si se indica que se usa un byte de opcode(opcode_size = 0b00), solo se hara uso del valor opcode[2] como unico valor real,
                // los siguientes campos opcode[1] y opcode[0] se podran usar para codifcar informacion addicional de la instruccion
                (opcode){ .opcode_byte.byte = 0b00000011 }, // opcode primario
            },
            .Mod_rm = (Mod_rm){   .mod = NOT_VALUE,   .reg = NOT_VALUE,  .R_M = NOT_VALUE },
            .SIB    =    (SIB){ .scale = NOT_VALUE, .index = NOT_VALUE, .base = NOT_VALUE },
            .displacement = { NOT_VALUE, NOT_VALUE, NOT_VALUE, NOT_VALUE},
            .immediate    = { NOT_VALUE, NOT_VALUE, NOT_VALUE, NOT_VALUE}
        }
    },
    {
        // add
        // immediate to register 1000 00sw : 11 000 reg : immediate data
        // immediate to memory 1000 00sw : mod 000 r/m : immediate data
        .string               = STRING_ADD,
        .immediate_instrution = NO_IMMEDIATE_INSTRUCTION,          // es una instruccion inmediata
        .opcode_size          = SIZE_OF_OPCODE_2_BYTE,         // un/dos byte's de opcode
        .posicion_w           = w_in_bit_1,       // hay bit "w" en el bit 1
        .posicion_d           = d_not_in_bit,       // no hay bit "d" 
        .posicion_s           = s_in_bit_2,       // hay bit "s" en el segundo bit
        .position_rm          = rm_in_byte_2,         // hay r/m en el segundo byte 
        .position_reg         = reg_in_byte_2,         // hay reg en el segundo byte
        .position_mod         = mod_in_byte_2,         // hay campo mod en el segundo byte
        .position_tttn        = NOT_JMP_ACTIVATE_EXCEPTIONS,         // no hay tttn
        .mask_mod             = 0b11000000,   // los bits mod's estan en los primeros 2 bits del segundo byte de opcode
        .mask_reg             = 0b00111000,   // con esta mascara se obtiene los bits 5, 4 y 3 de 7,6,5,4,3,2,1,0
        .mask_rm              = 0b00000111,   // con esta mascara se obtiene los bits 2, 1 y 0 de 7,6,5,4,3,2,1,0
        .mask_tttn            = NOT_TTTN,
        .number_reg           = INSTRUCTION_OF_ONE_REGISTER,         // 0b10 - usa un campo reg 
        .immediate_data       = HAS_IMMEDIATE_DATA,          // tiene datos inmediatos
        .instruction = { // ADC – ADD with Carry
            .prefix = { 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
            .opcode = {
                (opcode){ .opcode_byte.byte = NOT_OPCODE },
                (opcode){ .opcode_byte.byte = NOT_OPCODE }, // opcode secundario
                // si se indica que se usa un byte de opcode(opcode_size = 0b00), solo se hara uso del valor opcode[2] como unico valor real,
                // los siguientes campos opcode[1] y opcode[0] se podran usar para codifcar informacion addicional de la instruccion
                (opcode){ .opcode_byte.byte = 0b10000000 }, // opcode primario
            },
            .Mod_rm = (Mod_rm){   .mod = NOT_VALUE,   .reg = NOT_VALUE,  .R_M = NOT_VALUE },
            .SIB    =    (SIB){ .scale = NOT_VALUE, .index = NOT_VALUE, .base = NOT_VALUE },
            .displacement = { NOT_VALUE, NOT_VALUE, NOT_VALUE, NOT_VALUE},
            .immediate    = { NOT_VALUE, NOT_VALUE, NOT_VALUE, NOT_VALUE}
        }
    },
    {
        /*
         * AND – Logical AND
         * register1 to register2 0010 000w : 11 reg1 reg2
         * register2 to register1 0010 001w : 11 reg1 reg2
         * memory to register 0010 001w : mod reg r/m
         * register to memory 0010 000w : mod reg r/m
         */
        .string               = STRING_AND,
        .immediate_instrution = NO_IMMEDIATE_INSTRUCTION,          // es una instruccion inmediata
        .opcode_size          = SIZE_OF_OPCODE_2_BYTE,         // dos byte's de opcode
        .posicion_w           = w_in_bit_1,       // hay bit "w" en el bit 1
        .posicion_d           = 0b0010,       // hay bit  en el segundo bit
        .posicion_s           = 0b0000,       // no hay bit "s"
        .position_rm          = rm_in_byte_2,         // hay r/m en el segundo byte 
        .position_reg         = reg_in_byte_2,         // hay reg en el segundo byte
        .position_mod         = mod_in_byte_2,         // hay campo mod en el segundo byte
        .position_tttn        = NOT_JMP_ACTIVATE_EXCEPTIONS,         // no hay tttn
        .mask_mod             = 0b11000000,   // los bits mod's estan en los primeros 2 bits del segundo byte de opcode
        .mask_reg             = 0b00111000,   // con esta mascara se obtiene los bits 5, 4 y 3 de 7,6,5,4,3,2,1,0
        .mask_rm              = 0b00000111,   // con esta mascara se obtiene los bits 2, 1 y 0 de 7,6,5,4,3,2,1,0
        .mask_tttn            = 0b000100,     // al ser el position_tttn 11, quiere decir que no es una instruccion de salto
        .number_reg           = INSTRUCTION_OF_ONE_REGISTER,         // 0b10 - usa un campo reg 
        .immediate_data       = NO_IMMEDIATE_DATA,          // tiene datos inmediatos
        .instruction = { // ADC – ADD with Carry
            .prefix = { 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
            .opcode = {
                (opcode){ .opcode_byte.byte = NOT_OPCODE },
                (opcode){ .opcode_byte.byte = NOT_OPCODE }, // opcode secundario
                (opcode){ .opcode_byte.byte = 0b00100000 }, // opcode primario
            },
            .Mod_rm = (Mod_rm){   .mod = NOT_VALUE,   .reg = NOT_VALUE,  .R_M = NOT_VALUE },
            .SIB    =    (SIB){ .scale = NOT_VALUE, .index = NOT_VALUE, .base = NOT_VALUE },
            .displacement = { NOT_VALUE, NOT_VALUE, NOT_VALUE, NOT_VALUE},
            .immediate    = { NOT_VALUE, NOT_VALUE, NOT_VALUE, NOT_VALUE}
        }
    },    
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