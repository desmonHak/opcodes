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

#ifdef _WIN32
#ifndef _ExceptionHandler_WIN_
#define _ExceptionHandler_WIN_
LONG WINAPI ExceptionHandler(EXCEPTION_POINTERS *ExceptionInfo) {
    printf("Se ha producido una excepcion (codigo %lx) en la direccion %p\n",
        ExceptionInfo->ExceptionRecord->ExceptionCode,
        ExceptionInfo->ExceptionRecord->ExceptionAddress);
    return EXCEPTION_EXECUTE_HANDLER; // Manejar la excepcion
}
#endif
#endif

void __attribute__((constructor)) __constructor_debug_c__(){
    //debug_set_log_file("debug_log.txt");
    //open_file(&Log_debug_file, NAME_DEFAULT_LOG_DEBUG, READ_WRITE );
    #ifdef _WIN32
    #ifndef _ExceptionHandler_WIN_
    SetUnhandledExceptionFilter(ExceptionHandler);
    #endif
    #endif
    DEBUG_PRINT(DEBUG_LEVEL_INFO, "#{FG:white}[#{FG:red}DEBUG INIT#{FG:white}]");
    
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
    putchar( '\n');

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