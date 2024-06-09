/*
 *	Licencia Apache, Versión 2.0 con Modificación
 *	
 *	Copyright 2023 Desmon (David)
 *	
 *	Se concede permiso, de forma gratuita, a cualquier persona que obtenga una copia de 
 *	este software y archivos de documentación asociados (el "Software"), para tratar el 
 *	Software sin restricciones, incluidos, entre otros, los derechos de uso, copia, 
 *	modificación, fusión, publicación, distribución, sublicencia y/o venta de copias del 
 *	Software, y para permitir a las personas a quienes se les proporcione el Software 
 *	hacer lo mismo, sujeto a las siguientes condiciones:
 *	
 *	El anterior aviso de copyright y este aviso de permiso se incluirán en todas las 
 *	copias o partes sustanciales del Software.
 *	
 *	EL SOFTWARE SE PROPORCIONA "TAL CUAL", SIN GARANTÍA DE NINGÚN TIPO, EXPRESA O 
 *	IMPLÍCITA, INCLUYENDO PERO NO LIMITADO A LAS GARANTÍAS DE COMERCIABILIDAD, IDONEIDAD 
 *	PARA UN PROPÓSITO PARTICULAR Y NO INFRACCIÓN. EN NINGÚN CASO LOS TITULARES DEL 
 *	COPYRIGHT O LOS TITULARES DE LOS DERECHOS DE AUTOR SERÁN RESPONSABLES DE NINGÚN 
 *	RECLAMO, DAÑO U OTRA RESPONSABILIDAD, YA SEA EN UNA ACCIÓN DE CONTRATO, AGRAVIO O DE 
 *	OTRA MANERA, QUE SURJA DE, FUERA DE O EN CONEXIÓN CON EL SOFTWARE O EL USO U OTRO TIPO
 *	DE ACCIONES EN EL SOFTWARE.
 *	
 *	Además, cualquier modificación realizada por terceros se considerará propiedad del 
 *	titular original de los derechos de autor. Los titulares de derechos de autor 
 *	originales no se responsabilizan de las modificaciones realizadas por terceros.
 *	
 *	Queda explícitamente establecido que no es obligatorio especificar ni notificar los 
 *	cambios realizados entre versiones, ni revelar porciones específicas de código 
 *	modificado.
 */

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>
#include <stdarg.h>
#include "colors.h"
#include "error_c.h"

//#include "WL_lib.h"
//#define NAME_DEFAULT_LOG_DEBUG "debug_log.txt"
//MyFile Log_debug_file;

#ifdef DEBUG_ENABLE
#define DEBUG_PRINT(level, fmt, ...) debug_print(level, "#{FG:lgreen}[#{FG:lpurple}%s#{FG:reset}:#{FG:cyan}%s#{FG:reset}:#{FG:red}%03d#{FG:green}]#{FG:reset} " fmt , get_level_debug(level), __FILE__, __LINE__, ##__VA_ARGS__)

#else
#define DEBUG_PRINT(level, fmt, ...)
#endif


#define INIT_TYPE_FUNC_DBG(type_return, name_func) "#{FG:lred} "#type_return" #{FG:cyan}"#name_func"#{FG:white}("
#define TYPE_DATA_DBG(type_data, name_arg) " #{FG:lyellow}"#type_data" #{FG:white} "name_arg", "
#define END_TYPE_FUNC_DBG "#{FG:white}); #{FG:reset}\n"

typedef enum
{
    DEBUG_LEVEL_INFO,
    DEBUG_LEVEL_WARNING,
    DEBUG_LEVEL_ERROR
} DebugLevel;
static DebugLevel currentLevel = DEBUG_LEVEL_INFO; // nivel de debbug del programa por default

const char* ErrorLevelStrings[] = {
    "INFO",
    "WARNING",
    "ERROR",
};

typedef struct {
    char *message;
    const char *file;
    unsigned int line;
    DebugLevel level_c;
} Debug;

#define DEBUG_C(level, msg)                                                                 \
    do                                                                                    \
    {                                                                                     \
        Debug Deb = {.message = msg, .file = __FILE__, .line = __LINE__, .level_c = level}; \
        debug_print(Deb.level_c, Deb.message);     \
 } while (0)

#ifdef DEBUG_ENABLE
#define debug_malloc(type, name_var, size) \
        name_var = (type*)malloc(size); \
        DEBUG_PRINT(DEBUG_LEVEL_INFO, "#{FG:red}[#{FG:yellow}MALLOC#{FG:red}]#{FG:lred}  #{FG:white}(#{FG:lred}%s#{FG:white}*)#{FG:cyan}malloc#{FG:white}(%d) #{FG:lgreen}Variable#{FG:white}: %s, #{FG:lgreen}Tipo#{FG:white}: %s, #{FG:lgreen}Puntero#{FG:white}: %p\n", #type, size, #name_var, #type, name_var);
#else
#define debug_malloc(type, name_var, size) name_var = (type*)malloc(size)
#endif
#ifdef DEBUG_ENABLE
#define debug_calloc(type, name_var, cantidad, size) \
        name_var = (type*)calloc(cantidad, size); \
        DEBUG_PRINT(DEBUG_LEVEL_INFO, "#{FG:red}[#{FG:yellow}CALLOC#{FG:red}] #{FG:white}(#{FG:lred}%s#{FG:white}*)#{FG:cyan}calloc#{FG:white}(%d, %d) #{FG:lgreen}Variable#{FG:white}: %s, #{FG:lgreen}Tipo#{FG:white}: %s, #{FG:lgreen}Puntero#{FG:white}: %p\n", #type, cantidad, size, #name_var, #type, name_var); \
        if (NULL == name_var) puts("calloc error");
#else
#define debug_calloc(type, name_var, cantidad, size) name_var = (type*)calloc(cantidad, size); if (NULL == name_var) puts("calloc error");
#endif

#ifdef DEBUG_ENABLE
#define debug_realloc(type, name_var, size) \
        name_var = (type*)realloc(name_var, size); \
        DEBUG_PRINT(DEBUG_LEVEL_INFO, "#{FG:red}[#{FG:yellow}REALLOC#{FG:red}] #{FG:white}(#{FG:lred}%s#{FG:white}*)#{FG:cyan}realloc#{FG:white}(%p, %d) #{FG:lgreen}Variable#{FG:white}: %s, #{FG:lgreen}Tipo#{FG:white}: %s, #{FG:lgreen}Puntero#{FG:white}: %p\n", #type, name_var, size, #name_var, #type, name_var); \
        if (NULL == name_var) puts("realloc error");
#else
#define debug_realloc(type, name_var, size) name_var = (type*)realloc(name_var, size); if (NULL == name_var) puts("realloc error");
#endif

const char* get_level_debug(DebugLevel level);
//void debug_set_log_file(const char *filename);
void debug_set_level(DebugLevel level);
void debug_print(DebugLevel level, const char *fmt, ...);
void __attribute__((constructor)) __constructor_debug_c__();
void __attribute__((destructor)) __destructor_debug_c__();


#ifdef _WIN32
#ifndef _ExceptionHandler_WIN_
#define _ExceptionHandler_WIN_
LONG WINAPI ExceptionHandler(EXCEPTION_POINTERS *ExceptionInfo);
#endif
#endif
//static FILE *logFile = NULL;


#include "debug_c.c"
#endif 