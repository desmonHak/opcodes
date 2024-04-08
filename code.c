#include "print_structs_format.h"

#include <stdio.h>

int main(){
    Instruction my_instruccion = (Instruction){
        .prefix = { 0b00000000, 0b00000000, 0b00000000, 0b00000000},
        .opcode = {
            (opcode){
                .opcode = 0b000000,
                .d      = 0b0,
                .s      = 0b0
            },
            (opcode){
                .opcode = 0b000000,
                .d      = 0b0,
                .s      = 0b0
            },
            (opcode){
                .opcode = 0b000000, // add cl, al
                .d      = 0b0,
                .s      = 0b0
            }
        },
        .Mod_rm = (Mod_rm){
            .mod = 0b11,
            .reg = 0b000,
            .R_M = 0b001
        },
        .SIB = (SIB) {
            .scale = 0b10,
            .index = 0b101,
            .base  = 0b010
        },
        .displacement = { 0b00000010, 0b00001000, 0b00100000, 0b10000000},
        .immediate    = { 0b00000010, 0b00001000, 0b00100000, 0b10000000}
    };

    print_instruccion_binary(&my_instruccion);
    print_instruccion(&my_instruccion);

    my_instruccion = (Instruction){
        .prefix = { 0b00000000, 0b00000000, 0b00000000, Prefix_addr_size},
        .opcode = {
            (opcode){
                .opcode = 0b000000,
                .d      = 0b0,
                .s      = 0b0
            },
            (opcode){
                .opcode = 0b000000,
                .d      = 0b0,
                .s      = 0b0
            },
            (opcode){
                .opcode = 0b111111, // jmp [eax*2 + ebx]
                .d      = 0b1,
                .s      = 0b1
            }
        },
        .Mod_rm = (Mod_rm){
            .mod = 0b00,
            .reg = 0b100,
            .R_M = 0b100
        },
        .SIB = (SIB) {
            .scale = 0b01,
            .index = 0b000,
            .base  = 0b011
        },
        .displacement = { 0b00000000, 0b00000000, 0b00000000, 0b00000000},
        .immediate    = { 0b00000000, 0b00000000, 0b00000000, 0b00000000}
    };
    print_instruccion_binary(&my_instruccion);
    print_instruccion(&my_instruccion);

    return 0;
}