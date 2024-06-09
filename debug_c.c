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

#ifndef __DEBUG_C__
#define __DEBUG_C__
#include "debug_c.h"

#ifdef _ExceptionHandler_WIN_
static const char* ExceptionCodeDescription( const unsigned int code )
{
    #define STRING_EXCEPTION_WINDOWS(error_define) (const char*)#error_define
    switch( code ) {
        case EXCEPTION_ACCESS_VIOLATION:         return STRING_EXCEPTION_WINDOWS(EXCEPTION_ACCESS_VIOLATION)         ;
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    return STRING_EXCEPTION_WINDOWS(EXCEPTION_ARRAY_BOUNDS_EXCEEDED)    ;
        case EXCEPTION_BREAKPOINT:               return STRING_EXCEPTION_WINDOWS(EXCEPTION_BREAKPOINT)               ;
        case EXCEPTION_DATATYPE_MISALIGNMENT:    return STRING_EXCEPTION_WINDOWS(EXCEPTION_DATATYPE_MISALIGNMENT)    ;
        case EXCEPTION_FLT_DENORMAL_OPERAND:     return STRING_EXCEPTION_WINDOWS(EXCEPTION_FLT_DENORMAL_OPERAND)     ;
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:       return STRING_EXCEPTION_WINDOWS(EXCEPTION_FLT_DIVIDE_BY_ZERO)       ;
        case EXCEPTION_FLT_INEXACT_RESULT:       return STRING_EXCEPTION_WINDOWS(EXCEPTION_FLT_INEXACT_RESULT)       ;
        case EXCEPTION_FLT_INVALID_OPERATION:    return STRING_EXCEPTION_WINDOWS(EXCEPTION_FLT_INVALID_OPERATION)    ;
        case EXCEPTION_FLT_OVERFLOW:             return STRING_EXCEPTION_WINDOWS(EXCEPTION_FLT_OVERFLOW)             ;
        case EXCEPTION_FLT_STACK_CHECK:          return STRING_EXCEPTION_WINDOWS(EXCEPTION_FLT_STACK_CHECK)          ;
        case EXCEPTION_FLT_UNDERFLOW:            return STRING_EXCEPTION_WINDOWS(EXCEPTION_FLT_UNDERFLOW)            ;
        case EXCEPTION_ILLEGAL_INSTRUCTION:      return STRING_EXCEPTION_WINDOWS(EXCEPTION_ILLEGAL_INSTRUCTION)      ;
        case EXCEPTION_IN_PAGE_ERROR:            return STRING_EXCEPTION_WINDOWS(EXCEPTION_IN_PAGE_ERROR)            ;
        case EXCEPTION_INT_DIVIDE_BY_ZERO:       return STRING_EXCEPTION_WINDOWS(EXCEPTION_INT_DIVIDE_BY_ZERO)       ;
        case EXCEPTION_INT_OVERFLOW:             return STRING_EXCEPTION_WINDOWS(EXCEPTION_INT_OVERFLOW)             ;
        case EXCEPTION_INVALID_DISPOSITION:      return STRING_EXCEPTION_WINDOWS(EXCEPTION_INVALID_DISPOSITION)      ;
        case EXCEPTION_NONCONTINUABLE_EXCEPTION: return STRING_EXCEPTION_WINDOWS(EXCEPTION_NONCONTINUABLE_EXCEPTION) ;
        case EXCEPTION_PRIV_INSTRUCTION:         return STRING_EXCEPTION_WINDOWS(EXCEPTION_PRIV_INSTRUCTION)         ;
        case EXCEPTION_SINGLE_STEP:              return STRING_EXCEPTION_WINDOWS(EXCEPTION_SINGLE_STEP)              ;
        case EXCEPTION_STACK_OVERFLOW:           return STRING_EXCEPTION_WINDOWS(EXCEPTION_STACK_OVERFLOW)           ;
        default: return "UNKNOWN EXCEPTION" ;
    }
}

LONG WINAPI ExceptionHandler(EXCEPTION_POINTERS *ExceptionInfo) {
    EXCEPTION_RECORD *registro_excepcion = ExceptionInfo->ExceptionRecord;
    CONTEXT         *registro_contexto  = ExceptionInfo->ContextRecord;

    printf("Se ha producido una excepcion (codigo 0x%lx) en la direccion 0x%p\n\n", 
        registro_excepcion->ExceptionCode, registro_excepcion->ExceptionAddress);

    printf( 
            "\tDWORD  ExceptionCode     = 0x%p = %s;\n"
            "\tDWORD  ExceptionFlags    = 0x%p;\n"
            "\tSIZE_T ExceptionRecord   = 0x%p;\n"
            "\tSIZE_T ExceptionAddress  = 0x%p;\n"
            "\tDWORD  NumberParameters  = %u;\n"
            "\tSIZE_T ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS] = {", 
            registro_excepcion->ExceptionCode, ExceptionCodeDescription(registro_excepcion->ExceptionCode),
            registro_excepcion->ExceptionFlags, 
            registro_excepcion->ExceptionRecord, 
            registro_excepcion->ExceptionAddress,
            registro_excepcion->NumberParameters
        );
        uint8_t tab = EXCEPTION_MAXIMUM_PARAMETERS / 4;
        for (size_t i = 0; i < EXCEPTION_MAXIMUM_PARAMETERS; i++){
            if ((i % tab) == 0) printf("\n\t\t");
            printf(" %p,", registro_excepcion->ExceptionInformation[i]);
        }

        printf("\n\t}\n\n");
        #ifdef __x86_64__
        printf(
            "\tDWORD64 P1Home       = %p;\n"
            "\tDWORD64 P2Home       = %p;\n"
            "\tDWORD64 P3Home       = %p;\n"
            "\tDWORD64 P4Home       = %p;\n"
            "\tDWORD64 P5Home       = %p;\n"
            "\tDWORD64 P6Home       = %p;\n"
            "\tDWORD   ContextFlags = %08x;\n"
            "\tDWORD   MxCsr        = %08x;\n"
            "\tWORD    SegCs        = %04x;\n"
            "\tWORD    SegDs        = %04x;\n"
            "\tWORD    SegEs        = %04x;\n"
            "\tWORD    SegFs        = %04x;\n"
            "\tWORD    SegGs        = %04x;\n"
            "\tWORD    SegSs        = %04x;\n"
            "\tDWORD   EFlags       = %08x;\n"
            "\tDWORD64 Dr0          = %p;\n"
            "\tDWORD64 Dr1          = %p;\n"
            "\tDWORD64 Dr2          = %p;\n"
            "\tDWORD64 Dr3          = %p;\n"
            "\tDWORD64 Dr6          = %p;\n"
            "\tDWORD64 Dr7          = %p;\n"
            "\tDWORD64 Rax          = %p;\n"
            "\tDWORD64 Rcx          = %p;\n"
            "\tDWORD64 Rdx          = %p;\n"
            "\tDWORD64 Rbx          = %p;\n"
            "\tDWORD64 Rsp          = %p;\n"
            "\tDWORD64 Rbp          = %p;\n"
            "\tDWORD64 Rsi          = %p;\n"
            "\tDWORD64 Rdi          = %p;\n"
            "\tDWORD64 R8           = %p;\n"
            "\tDWORD64 R9           = %p;\n"
            "\tDWORD64 R10          = %p;\n"
            "\tDWORD64 R11          = %p;\n"
            "\tDWORD64 R12          = %p;\n"
            "\tDWORD64 R13          = %p;\n"
            "\tDWORD64 R14          = %p;\n"
            "\tDWORD64 R15          = %p;\n"
            "\tDWORD64 Rip          = %p;\n"
            "\tM128A   Header[2]    = { %p%p, %p%p };\n"
            "\tM128A   Legacy[8]    = { \n\t\t%p%p, %p%p, \n\t\t%p%p, %p%p, \n\t\t%p%p, %p%p, \n\t\t%p%p, %p%p };\n"
            "\tM128A   Xmm0         = %p%p;\n"
            "\tM128A   Xmm1         = %p%p;\n"
            "\tM128A   Xmm2         = %p%p;\n"
            "\tM128A   Xmm3         = %p%p;\n"
            "\tM128A   Xmm4         = %p%p;\n"
            "\tM128A   Xmm5         = %p%p;\n"
            "\tM128A   Xmm6         = %p%p;\n"
            "\tM128A   Xmm7         = %p%p;\n"
            "\tM128A   Xmm8         = %p%p;\n"
            "\tM128A   Xmm9         = %p%p;\n"
            "\tM128A   Xmm10        = %p%p;\n"
            "\tM128A   Xmm11        = %p%p;\n"
            "\tM128A   Xmm12        = %p%p;\n"
            "\tM128A   Xmm13        = %p%p;\n"
            "\tM128A   Xmm14        = %p%p;\n"
            "\tM128A   Xmm15        = %p%p;\n"
            "\tM128A   VectorRegister[26] = ",
                registro_contexto->P1Home, registro_contexto->P2Home,
                registro_contexto->P3Home, registro_contexto->P4Home,
                registro_contexto->P5Home, registro_contexto->P6Home,
                registro_contexto->ContextFlags,
                registro_contexto->MxCsr,
                registro_contexto->SegCs,  registro_contexto->SegDs,
                registro_contexto->SegEs,  registro_contexto->SegFs,
                registro_contexto->SegGs,  registro_contexto->P1Home,
                registro_contexto->SegCs,  registro_contexto->SegSs,
                registro_contexto->EFlags, 
                registro_contexto->Dr0,    registro_contexto->Dr1,
                registro_contexto->Dr2,    registro_contexto->Dr3,
                registro_contexto->Dr6,    registro_contexto->Dr7,
                registro_contexto->Rax,    registro_contexto->Rcx,
                registro_contexto->Rdx,    registro_contexto->Rbx,
                registro_contexto->Rsp,    registro_contexto->Rbp,
                registro_contexto->Rsi,    registro_contexto->Rcx,
                registro_contexto->Rax,    registro_contexto->Rdi,
                registro_contexto->R8,     registro_contexto->R9,
                registro_contexto->R10,    registro_contexto->R11,
                registro_contexto->R12,    registro_contexto->R13,
                registro_contexto->R14,    registro_contexto->R15,
                registro_contexto->Rip,    
                registro_contexto->R8,     registro_contexto->R9,
                registro_contexto->Xmm0,   registro_contexto->Xmm1,
                registro_contexto->Xmm2,   registro_contexto->Xmm3,
                registro_contexto->Xmm4,   registro_contexto->Xmm5,
                registro_contexto->Xmm6,   registro_contexto->Xmm7,
                registro_contexto->Xmm8,   registro_contexto->Xmm9,
                registro_contexto->Xmm10,  registro_contexto->Xmm11,
                registro_contexto->Xmm12,  registro_contexto->Xmm13,
                registro_contexto->Xmm14,  registro_contexto->Xmm15
                
                

        );
        tab = 26 / 5;
        for (size_t i = 0; i < 26; i++){
            if ((i % tab) == 0) printf("\n\t\t");
            M128A val = registro_contexto->VectorRegister[i];
            printf(" %p%p,", (void*)val.High, (void*)val.Low);
        }
        printf("\n\t}\n");
        printf(
            "\tDWORD64 VectorControl        = %p;\n"
            "\tDWORD64 DebugControl         = %p;\n"
            "\tDWORD64 LastBranchToRip      = %p;\n"
            "\tDWORD64 LastBranchFromRip    = %p;\n"
            "\tDWORD64 LastExceptionToRip   = %p;\n"
            "\tDWORD64 LastExceptionFromRip = %p;\n",
            registro_contexto->VectorControl,
            registro_contexto->DebugControl,
            registro_contexto->LastBranchToRip,
            registro_contexto->LastBranchToRip,
            registro_contexto->LastExceptionToRip,
            registro_contexto->LastExceptionFromRip
        );
        #else
        printf(
            "\tDWORD 		ContextFlags = %p;\n"
            "\tDWORD 		Dr0          = %p;\n"
            "\tDWORD 		Dr1          = %p;\n"
            "\tDWORD 		Dr2          = %p;\n"
            "\tDWORD 		Dr3          = %p;\n"
            "\tDWORD 		Dr6          = %p;\n"
            "\tDWORD 		Dr7          = %p;\n"
            "\tDWORD 		SegGs        = %p;\n"
            "\tDWORD 		SegFs        = %p;\n"
            "\tDWORD 		SegEs        = %p;\n"
            "\tDWORD 		SegDs        = %p;\n"
            "\tDWORD 		Edi          = %p;\n"
            "\tDWORD 		Esi          = %p;\n"
            "\tDWORD 		Ebx          = %p;\n"
            "\tDWORD 		Edx          = %p;\n"
            "\tDWORD 		Ecx          = %p;\n"
            "\tDWORD 		Eax          = %p;\n"
            "\tDWORD 		Ebp          = %p;\n"
            "\tDWORD 		Eip          = %p;\n"
            "\tDWORD 		SegCs        = %p;\n"
            "\tDWORD 		EFlags       = %p;\n"
            "\tDWORD 		Esp          = %p;\n"
            "\tDWORD 		SegSs        = %p;\n"
            "\tBYTE			ExtendedRegisters[MAXIMUM_SUPPORTED_EXTENSION] = ",
            registro_contexto->ContextFlags,
            registro_contexto->Dr0,         registro_contexto->Dr1,
            registro_contexto->Dr2,         registro_contexto->Dr3,
            registro_contexto->Dr6,         registro_contexto->Dr7,
            registro_contexto->SegGs,       registro_contexto->SegFs,
            registro_contexto->SegEs,       registro_contexto->SegDs,
            registro_contexto->Edi,         registro_contexto->Esi,
            registro_contexto->Ebx,         registro_contexto->Edx,
            registro_contexto->Ecx,         registro_contexto->Eax,
            registro_contexto->Ebp,         registro_contexto->Eip,
            registro_contexto->SegCs,       registro_contexto->EFlags,
            registro_contexto->Esp,         registro_contexto->SegSs

        );
        
        tab = MAXIMUM_SUPPORTED_EXTENSION / 16;
        for (size_t i = 0; i < MAXIMUM_SUPPORTED_EXTENSION; i++){
            if ((i % tab) == 0) printf("\n\t\t");
            printf(" %02x", registro_contexto->ExtendedRegisters[i]);
        }
        printf("\n\t}\n");
    #endif

    return EXCEPTION_EXECUTE_HANDLER; // Manejar la excepcion
}
#endif

void __attribute__((constructor)) __constructor_debug_c__(){
    //debug_set_log_file("debug_log.txt");
    //open_file(&Log_debug_file, NAME_DEFAULT_LOG_DEBUG, READ_WRITE );
    #ifdef _ExceptionHandler_WIN_
    SetUnhandledExceptionFilter(ExceptionHandler);
    #endif
    DEBUG_PRINT(DEBUG_LEVEL_INFO, "#{FG:white}[#{FG:red}DEBUG INIT#{FG:white}]\n");
    
    /*if (Log_debug_file.archivo == OPEN_ERROR) {
        printf("Error al abrir el archivo " NAME_DEFAULT_LOG_DEBUG "\n");
        return 1;
    }*/

}
void __attribute__((destructor)) __destructor_debug_c__(){
    /*if (logFile != NULL)
    {
        fclose(logFile);
        logFile = NULL;
    }*/
    /*close_file(&Log_debug_file);*/
}

const char* get_level_debug(DebugLevel level) {
    switch(level){

    case DEBUG_LEVEL_INFO:
        return ErrorLevelStrings[0];
        break;
    case DEBUG_LEVEL_WARNING:
        return ErrorLevelStrings[1];
        break;
    case DEBUG_LEVEL_ERROR:
        return ErrorLevelStrings[2];
        break;
    default:
        return "UNKNOW";
        break;
    };
}  

/*void debug_set_log_file(const char *filename)
{
    if (logFile != NULL)
    {
        fclose(logFile);
        logFile = NULL;
    }

    logFile = fopen(filename, "w");
}*/

void debug_set_level(DebugLevel level)
{
    currentLevel = level;
}

void debug_print(DebugLevel level, const char *fmt, ...)
{
    if (fmt == (const char *)NULL) return;
    if (level < currentLevel)
    {
        return;
    }

    va_list args;
    va_start(args, fmt);

    va_list args_copy;
    va_copy(args_copy, args);
    size_t size = (vsnprintf(NULL, 0, fmt, args_copy) + 1) * sizeof(char);
    va_end(args_copy);
    va_copy(args_copy, args);

    vprintf_color( fmt, args_copy);
    va_end(args_copy);

    /*if (logFile != NULL)
    {
        vfprintf(logFile, fmt, args);
        fprintf(logFile, "\n");
    }*/

    /*unsigned char *formatted_buffer = (unsigned char *)malloc(size);
    vsprintf(formatted_buffer, fmt, args);

    write_file(&Log_debug_file, formatted_buffer);
    if(Log_debug_file.size == WRITE_ERROR){
        printf("Error al escribir el archivo\n");
    }*/

    va_end(args);

    if (level == DEBUG_LEVEL_ERROR)
    {
        va_start(args, fmt);
        char *message = malloc(size * sizeof(char));
        vsnprintf(message, size, fmt, args);
        va_end(args);
        
        ERROR_C(ERROR_LEVEL_ERROR, message);
    }
}
#endif