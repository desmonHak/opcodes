#ifndef __PRINT_STRUCTS_FORMAT_C_
#define __PRINT_STRUCTS_FORMAT_C_

#include "print_structs_format.h"

void print_table_hex(char *string_init, char *string_text_for_printing, size_t size_string_text_for_printing, encoder_x86 encoder_val) {
    size_t size = (snprintf(NULL, 0, "%04x", size_string_text_for_printing) + 1) * sizeof(char);
    char *buffer_Position_memory = (char *)malloc(size), *buffer_spaces = NULL;
    sprintf(buffer_Position_memory, "%04x", size_string_text_for_printing);

    uint32_t level_space = strlen(buffer_Position_memory) ;
    buffer_spaces = (char *)malloc(level_space);
    for (uint32_t i = 0; i < level_space; i++) buffer_spaces[i] = ' ';
    buffer_spaces[level_space] = '\0';
    printf_color("\n%s%s ", string_init, buffer_spaces);
    for (uint16_t r = 0x23; r < 0x33; r++) { 
        printf_color("|"FOREGROUND_COLOR_CUSTOM("%d")"%02x#{FG:reset}", r>> 2, ((uint8_t)(r-0x33)) - 0xf0 ); 
    }

    unsigned int random_color = jenkins_hash(string_text_for_printing[0], level_space, 0, 1, 2, 3, 4);
    printf_color("|\n%s"FOREGROUND_COLOR_CUSTOM("%d")"0000 #{FG:reset}", string_init, (uint8_t)(random_color >> 2));
    for (uint32_t i = 0; i < size_string_text_for_printing; i++)
    {
    unsigned int Avalue1, Avalue2, Avalue3, seed, values[] = {
        (unsigned int)size_string_text_for_printing, 
        size, 
        level_space, 
        0xa0, 0xe1, 
        string_text_for_printing[i]
    };
    regenerate_keys:
        //shuffle_array(values, sizeof(values)/sizeof(int));
        seed = (unsigned int)string_text_for_printing[i];
        Avalue1 = jenkins_hash(seed,    values[0], values[1], values[2], values[3], values[4], values[5]);
        Avalue2 = jenkins_hash(Avalue1, values[0], values[1], values[2], values[3], values[4], values[5]);
        Avalue3 = jenkins_hash(Avalue2, values[0], values[1], values[2], values[3], values[4], values[5]);
        if ((Avalue1 & Avalue2 & Avalue3) == Avalue1) { 
            values[0] ^= 0b00101010; values[1] |= values[0];
            values[2] ^= values[1];  values[3] &= values[2];
            values[4] |= values[3];  values[5] ^= values[4];
            goto regenerate_keys;
        }
        printf_color("|"FOREGROUND_COLOR_CUSTOM("%d")"%.2X#{BG:reset}", 
        ((((uint8_t)string_text_for_printing[i] >> 2)) & 0b1110111) | 0b00000001,
        (uint8_t)string_text_for_printing[i]);

        //printf_color(" #{FG:red}#{BG:green}|#{BG:reset}");
        if ((i+1) % (BLOCK_SLICES / 8) == 0){
            unsigned int random_color = jenkins_hash(string_text_for_printing[0], Avalue1, Avalue2, Avalue3, 2, 3, 4);
            printf_color("|\n%s"FOREGROUND_COLOR_CUSTOM("%d")"%04x #{FG:reset}", string_init, (uint8_t)(random_color >> 2), i+1);
        }

    }
    free(buffer_Position_memory);
    free(buffer_spaces);
    printf("|\n");
}

void print_binary(unsigned int num, uint16_t num_bits, uint16_t init_count) {
    // num_bits = 2
    // init_count = 0
    uint16_t global_counter = 0;
    uint16_t counter_bits = num_bits -1;
    for (uint16_t i = 1; (counter_bits  > 0) || (i <= 8); i++ ) {
        //printf("(%d)", counter_bits);
        if (i > init_count && counter_bits+1 != 0) 
        {
            printf_color("#{FG:lyellow}%d#{FG:reset}", (num >> counter_bits) & 1);
            if (counter_bits > 0) counter_bits--;
        }
        else putchar('-');
        if ((i % 4) == 0 && (i != 0)) putchar(' ');
    }
    putchar('\n');
}

/*void print_binary(unsigned int num, int num_bits) {
    for (int i = num_bits - 1; i >= 0; i--) {
        printf_color("#{FG:lyellow}%d#{FG:reset}", (num >> i) & 1);
        if (i % 4 == 0 && i != 0) printf(" "); 
    } printf("\n");
}*/

void print_opcode(Instruction_info *my_instruccion_, opcode opcode, uint8_t number_opcode) {
    printf_color("opcode#{FG:lpurple}{#{FG:lred}%d#{FG:lpurple}}#{FG:reset} (#{FG:lblue}8bits#{FG:reset}): #{FG:lgreen}%02X#{FG:reset} = #{FG:lyellow}", number_opcode, *((uint8_t*)(&opcode)));
    print_binary( *((uint8_t*)(&opcode)), 8, 0);
    printf_color("#{FG:reset}opcode[#{FG:lred}%d#{FG:reset}] (#{FG:lblue}6bits#{FG:reset}): #{FG:lgreen}%02X#{FG:reset} = #{FG:lyellow}", number_opcode, opcode.opcode_bits_final.opcode);
    print_binary(opcode.opcode_bits_final.opcode<< 2, 8, 0);
    //printf_color("#{FG:reset}opcode[#{FG:lred}%d#{FG:reset}].d:  #{FG:lgreen}%02X#{FG:reset} =        #{FG:lyellow}%d#{FG:reset}\n", number_opcode, opcode.opcode_bits_final.d, opcode.opcode_bits_final.d);
    //printf_color("opcode[#{FG:lred}%d#{FG:reset}].s:  #{FG:lgreen}%02X#{FG:reset} =         #{FG:lyellow}%d#{FG:reset}\n\n", number_opcode, opcode.opcode_bits_final.s, opcode.opcode_bits_final.s);
    if (number_opcode == 2) {
        if (my_instruccion_->posicion_d != 0) {
            printf_color("#{FG:reset}opcode[#{FG:lred}%d#{FG:reset}].d(#{FG:lblue}1bit#{FG:reset}): #{FG:lgreen}%02X#{FG:reset} = #{FG:reset}", number_opcode, opcode.opcode_bits_final.d);
            print_binary(opcode.opcode_bits_final.d, 1, 6);
        } else printf_color("#{FG:reset}opcode[#{FG:lred}%d#{FG:reset}].d(#{FG:lblue}1bit#{FG:reset}): #{FG:lred}%02X#{FG:reset} = #{FG:lred}not exists field D#{FG:reset}\n", number_opcode, opcode.opcode_bits_final.d);
        if (my_instruccion_->posicion_s != 0) {
            printf_color("opcode[#{FG:lred}%d#{FG:reset}].s(#{FG:lblue}1bit#{FG:reset}): #{FG:lgreen}%02X#{FG:reset} = #{FG:reset}", number_opcode, opcode.opcode_bits_final.s);
            print_binary(opcode.opcode_bits_final.d, 1, 7);
        } else printf_color("#{FG:reset}opcode[#{FG:lred}%d#{FG:reset}].s(#{FG:lblue}1bit#{FG:reset}): #{FG:lred}%02X#{FG:reset} = #{FG:lred}not exists field S#{FG:reset}\n", number_opcode, opcode.opcode_bits_final.s);
        if (my_instruccion_->posicion_w != 0) {
            printf_color("opcode[#{FG:lred}%d#{FG:reset}].w(#{FG:lblue}1bit#{FG:reset}): #{FG:lgreen}%02X#{FG:reset}   -> #{FG:lred}(#{FG:reset}mask#{FG:lred})#{FG:reset} #{FG:reset}", number_opcode, get_bit_w(my_instruccion_));
            print_binary(my_instruccion_->posicion_w, 4, 4);
        } else printf_color("#{FG:reset}opcode[#{FG:lred}%d#{FG:reset}].w(#{FG:lblue}1bit#{FG:reset}): #{FG:lred}%02X#{FG:reset} = #{FG:lred}not exists field W#{FG:reset}\n", number_opcode, get_bit_w(my_instruccion_));
    }
    putchar('\n');
}

void print_instruccion_binary(Instruction *my_instruccion) {
    printf("Instruccion = \n");
    for (size_t i = 0; i < sizeof(Instruction); i++) {
        printf("\t\t\t\t");
        print_binary(((uint8_t*)(my_instruccion))[i], 8, 0);
        printf(" ");
    }printf("\n");
}

void print_instruccion(Instruction_info *my_instruccion_, encoder_x86 encoder_val) {

    unsigned int Avalue1, Avalue2, Avalue3, seed, values[] = {
        my_instruccion_->instruction.opcode[0].opcode_byte.byte + 0x0f01, 
        my_instruccion_->instruction.opcode[1].opcode_byte.byte + 0xf0010a0, 
        my_instruccion_->instruction.opcode[2].opcode_byte.byte, 
        0xa0, 0xf1, 
        my_instruccion_->instruction.opcode[2].opcode_byte.byte + 1 +
        my_instruccion_->instruction.opcode[1].opcode_byte.byte +
        my_instruccion_->instruction.opcode[0].opcode_byte.byte + 20
    };
    regenerate_keys:
        //shuffle_array(values, sizeof(values)/sizeof(int));
        seed = my_instruccion_->instruction.opcode[2].opcode_byte.byte * 
                            my_instruccion_->instruction.opcode[1].opcode_byte.byte * 
                            my_instruccion_->instruction.opcode[0].opcode_byte.byte ;
        Avalue1 = jenkins_hash(seed,    values[0], values[1], values[2], values[3], values[4], values[5]);
        Avalue2 = jenkins_hash(Avalue1, values[0], values[1], values[2], values[3], values[4], values[5]);
        Avalue3 = jenkins_hash(Avalue2, values[0], values[1], values[2], values[3], values[4], values[5]);
        if ((Avalue1 & Avalue2 & Avalue3) == Avalue1) { 
                    values[0] ^= 0b10101010; values[1] ^= values[0];
                    values[2] ^= values[1];  values[3] &= values[2];
                    values[4] ^= values[3];  values[5] ^= values[4];
                    goto regenerate_keys;
        }
    printf_color("string instruccion: #{BG:%d;%d;%d} %s#{FG:reset} (color: %u %u %u)\n", (unsigned char)Avalue1, (unsigned char)Avalue2, (unsigned char)Avalue3,  get_string_instrution(my_instruccion_->string), (unsigned char)Avalue1, (unsigned char)Avalue2, (unsigned char)Avalue3);
    Instruction *my_instruccion = &(my_instruccion_->instruction);
    printf_color("prefix:       #{FG:lgreen}%02X %02X %02X %02X#{FG:reset} = \n", my_instruccion->prefix[0], my_instruccion->prefix[1], my_instruccion->prefix[2], my_instruccion->prefix[3]);
    for (int i = 0; i < sizeof(uint8_t) *4; i++) {
        printf("\t\t\t\t");
        print_binary(my_instruccion->prefix[i], 8, 0);

    } printf("\n");

    printf_color("SizeOpcode(#{FG:lblue}2bits#{FG:reset}): #{FG:lgreen}%02X#{FG:reset} = ", my_instruccion_->opcode_size+1);
    print_binary(my_instruccion_->opcode_size, 2, 6);

    // mostart solo x bytes del opcode definidos por el miembro .opcode_size
    // se recorre el array de 3 posiciones de forma inversa, [2, 1, 0], siendo 2 el opcode primario.
    for (unsigned char pos = my_instruccion_->opcode_size +1; pos != 0; pos--)
        print_opcode(my_instruccion_, my_instruccion->opcode[3-pos], pos);
        // array[limite del array - posicion actual] == recorrer array desde el final

    
    printf_color("Mod_rm:       #{FG:lgreen}%02X#{FG:reset} =       ", *((uint8_t*)&(my_instruccion->Mod_rm)));
    print_binary( *((uint8_t*)&(my_instruccion->Mod_rm)), 8, 0);
    printf_color("mod:          #{FG:lgreen}%02X#{FG:reset} =       ", my_instruccion->Mod_rm.mod); print_binary(my_instruccion->Mod_rm.mod, 2, 0);
    printf_color("reg:          #{FG:lgreen}%02X#{FG:reset} = %03s = ", my_instruccion->Mod_rm.reg, get_string_register(encoder_val, get_bit_w(my_instruccion_), my_instruccion->Mod_rm.reg)); 
    //(instrutions[i] & my_instruccion_->mask_reg) >> count_get_mask(actual_node->Instruction.mask_reg)
    print_binary(my_instruccion->Mod_rm.reg, 3, 2);
    printf_color("R_M:          #{FG:lgreen}%02X#{FG:reset} = %03s = ", my_instruccion->Mod_rm.R_M, get_string_register(encoder_val, get_bit_w(my_instruccion_), my_instruccion->Mod_rm.R_M));
    print_binary(my_instruccion->Mod_rm.R_M, 3, 5);

    /*
     * "mod" con valor 00 junto al campo "r/m" 100 indica que se trata del modo SIB
     * Mod_rm:       AA = 00xx x100 (0x04)
     *  - mod:       02 = 00
     *  - reg:       05 =   xxx     (eliminar campo REG usando AND 0b11000111)
     *  - R_M:       02 =       100
     */
    if ((*((uint8_t*)&(my_instruccion->Mod_rm)) & 0b11000111) == 0b00000100){ // modo SIB
        printf_color("\nSIB:          #{FG:lgreen}%02X#{FG:reset} = ", *((uint8_t*)&(my_instruccion->SIB)));
        print_binary( *((uint8_t*)&(my_instruccion->SIB)), 8, 0);
        printf_color("scale:        #{FG:lgreen}%02X#{FG:reset} = ", my_instruccion->SIB.scale); print_binary(my_instruccion->SIB.scale, 2, 0);
        printf_color("index:        #{FG:lgreen}%02X#{FG:reset} = ", my_instruccion->SIB.index); print_binary(my_instruccion->SIB.index, 3, 2);
        printf_color("base:         #{FG:lgreen}%02X#{FG:reset} = ",  my_instruccion->SIB.base); print_binary(my_instruccion->SIB.base, 3, 5);
    
        printf_color("\ndisplacement: #{FG:lgreen}%02X %02X %02X %02XX#{FG:reset} = \n", my_instruccion->displacement[0], my_instruccion->displacement[1], my_instruccion->displacement[2], my_instruccion->displacement[3]);
        for (int i = 0; i < sizeof(uint8_t) * 4; i++) {
            printf("\t\t\t\t");
            print_binary(my_instruccion->displacement[i], 8, 0);
        } printf("\n");

        printf_color("immediate:    #{FG:lgreen}%02X %02X %02X %02XX#{FG:reset} = \n", my_instruccion->immediate[0], my_instruccion->immediate[1], my_instruccion->immediate[2], my_instruccion->immediate[3]);
        for (int i = 0; i < sizeof(uint8_t) * 4; i++) {
            printf("\t\t\t\t");
            print_binary(my_instruccion->immediate[i], 8, 0);
        } printf("\n");
    }
}

#endif