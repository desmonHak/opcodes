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

#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdio.h>
#include <stdlib.h>

#define ERROR_LOG_FILE "error_log.txt"

typedef enum {
    ERROR_LEVEL_INFO,
    ERROR_LEVEL_WARNING,
    ERROR_LEVEL_ERROR
} ErrorLevel;

typedef struct {
    char *message;
    const char *file;
    unsigned int line;
    ErrorLevel level_c;
} Error;

void error_print(const Error *err);
void error_log(const Error *err);

#define ERROR_C(level, msg)                                                                 \
    do                                                                                    \
    {                                                                                     \
        Error err = {.message = msg, .file = __FILE__, .line = __LINE__, .level_c = level}; \
        error_print(&err);                                                                \
        error_log(&err);                                                                  \
        if (level == ERROR_LEVEL_ERROR)                                                   \
        {                                                                                 \
            exit(EXIT_FAILURE);                                                           \
        }                                                                                 \
    } while (0)


#include "error_c.c"
#endif 