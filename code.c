#include "print_structs_format.h"

#include <stdio.h>

int main(){
    Instruction my_instruccion = (Instruction){
        .prefix = { 0b00000000, 0b00000000, 0b00000000, 0b00000000},
        .opcode = {
            (opcode){
                .opcode = 0b000000,
                .d      = 0b0,
                .s      = 0b1
            },
            (opcode){
                .opcode = 0b000000,
                .d      = 0b1,
                .s      = 0b0
            },
            (opcode){
                .opcode = 0b000000,
                .d      = 0b1,
                .s      = 0b1
            }
        },
        .Mod_rm = (Mod_rm){
            .mod = 0b10,
            .reg = 0b101,
            .R_M = 0b010
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




    return 0;
}