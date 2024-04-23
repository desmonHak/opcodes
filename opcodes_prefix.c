#ifndef __OPCODES_PREFIX_C_
#define __OPCODES_PREFIX_C_

#include "opcodes_prefix.h"

static const char *get_string_instrution(string_instrution_id id){
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

#endif