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
/* 
 *
 * fuente: https://intelxed.github.io/ref-manual/index.html#INTRO
 * Las instrucciones en x86 ocupan un maximo de 1-15 bytes
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
 *     
 * fuente: https://www.scs.stanford.edu/05au-cs240c/lab/i386/s17_02.htm
 * Prefijos:
 * 
 *          Grp 1, Grp 2, (optional)
 *          Grp 3, Grp 4 (pag 2869 intel)
 * 
 *      ----------------------------------------------------------------------------------
 *      Prefijos heredados (Consulte el capítulo 2, "Formato de las instrucciones", del Manual del desarrollador de software de las arquitecturas Intel® 64 e IA-32, Volumen 2A, para obtener más información sobre los prefijos heredados.):
 *      0x66    Anulación del tamaño operativo
 *      0x67    Anulación del tamaño de la dirección
 * 
 *      ===========================================================================
 *      0xF3    Prefijo REP (utilizado sólo con instrucciones de string)
 *      0xF3    Prefijo REPE/REPZ (sólo se utiliza con instrucciones de string)
 *      ----------------------------------------------------------------------------------
 *      0xF2    Prefijo REPNE/REPNZ (utilizado sólo con instrucciones de cadena)
 *      0xF0    Prefijo LOCK
 *      ===========================================================================
 * 
 *      Los prefihos heredados se utilizan en extensiones de instrucciones. Los prefijos Legacy(heredados) deben colocarse antes de los prefijos REX.
 * 
 * B.1.2 Prefijos REX
 * Los prefijos REX son un conjunto de 16 opcodes que abarcan una fila del mapa de opcodes y ocupan las entradas 40H a 4FH. Estos
 * opcodes representan instrucciones válidas (INC o DEC) en los modos de operación IA-32 y en modo de compatibilidad. En modo
 * los mismos opcodes representan el prefijo de instrucción REX y no se tratan como instrucciones individuales.
 * Consulte el capítulo 2, "Formato de las instrucciones", del Manual del desarrollador de software de las arquitecturas Intel® 64 e IA-32,
 * Volumen 2A, para obtener más información sobre los prefijos REX.
 * 
 *      prefijos de anulacion de segmento:
 *          0x2E    Prefijo de anulación del segmento CS (segmento de codigo)
 *          0x36    Prefijo de anulación del segmento SS (segmento de pila)
 *          0x3E    Prefijo de anulación del segmento DS (segmento de datos)
 *          0x26    Prefijo de anulación del segmento ES
 *          0x64    Prefijo de anulación del segmento FS
 *          0x65    Prefijo de anulación del segmento GS
 * 
 *          Estos prefijos permiten anular el segmento de memoria predeterminado al que hace referencia la instruccion.
 *          Esto puede ser útil en diversas situaciones, como cuando se trabaja con segmentos específicos de datos, como 
 *          DS o ES, o cuando se necesita acceder a datos en segmentos de memoria especiales, como el segmento de video (VGABIOS), 
 *          el segmento de pila (SS) o el segmento de código (CS) para realizar ciertas operaciones específicas.
 *          Por ejemplo, supongamos que necesitas acceder a datos en el segmento de datos (DS) en lugar del segmento de código 
 *          (CS) predeterminado. Puedes utilizar el prefijo de segmento ds: antes de la instrucción para anular temporalmente 
 *          el segmento de datos predeterminado y acceder a los datos en el segmento de datos especificado.
 * 
 *          mov eax, dword ptr ds:[ebx]  ; copia un valor de 32 bits de la dirección de memoria apuntada por el registro ebx 
 *          en el segmento de datos (DS) en el registro eax. Específicamente, está accediendo a los datos de tamaño de palabra 
 *          doble (32 bits) en el segmento de datos y los está almacenando en el registro eax.
 * 
 * 

 * 
 * (pag 2869 intel)
 * los opcodes 1000 00 indican que es una instruccion inmediata (superior a 0x80 = 1000 00)
 * 
 * 
 * B.1.4 Campos Especiales (pagina 2870)
 * La Tabla B-1 enumera los campos de bits que aparecen en ciertas instrucciones, a veces dentro de los bytes del opcode. Todos estos campos
 * (excepto el bit d) aparecen en los formatos de instrucciones de propósito general en la Tabla B-13.
 * 
 * El campo Reg puede utilizarse como campo de extensión opcode (TTT) y como manera de codificar registros de diagnóstico (eee).
 * El campo reg tmb es especificador de registro general.    3bit
 * w Especifica si los datos son bytes o de tamaño completo, donde tamaño completo es 16 o 32 bits -> 1bit
 * s Especifica la extensión de signo de un campo inmediato -> 1bit
 * sreg2 Especificador de registro de segmento para CS, SS, DS, ES -> 2bit
 * sreg3 Especificador de registro de segmento para CS, SS, DS, ES, FS, GS -> 3bit
 * eee Especifica un registro de propósito especial (control o depuración) -> 3bit
 * tttn Para instrucciones condicionales, especifica una condición afirmada o negada -> 4bit
 * d Especifica la dirección de la operación de datos -> 1bit
 * 
 * fuente: https://www-user.tu-chemnitz.de/~heha/hsn/chm/x86.chm/x86.htm
 * Si w = 0, los operandos son registros de 8 bits y posiciones de memoria.
 * Si w = 1, los operandos son de 16 bits o de 32 bits:
 * En sistemas operativos de 32 bits, si s = 1, los operandos son por defecto de 32 bits.
 * Para especificar un operando de 16 bits (en Windows o Linux) debe 
 * insertar un byte especial de prefijo de tamaño de operando delante de la instrucción 
 * (ejemplo de esto más adelante).
 * 
 * 
 * Para instrucciones inmediatas:
 * Interacción de w y s
 * 
 * w = 0 y s = 0:
 * El operando inmediato es de 8 bits.
 * No hay extensión de signo porque no es necesario, ya que el operando destino también es de 8 bits.
 * w = 1 y s = 0:
 * 
 * El operando inmediato es del tamaño completo especificado por w (16 o 32 bits).
 * No hay extensión de signo porque el operando inmediato ya coincide con el tamaño del operando destino.
 * w = 1 y s = 1:
 * 
 * El operando inmediato es de 8 bits, pero se extiende con signo para coincidir con el tamaño del operando destino (16 o 32 bits).
 * Esta configuración permite usar un operando inmediato pequeño (de 8 bits) y extenderlo para operaciones que requieren un operando más grande.
 * 
 * Ejemplo Práctico
 * Supongamos que estamos en modo protegido y estamos usando la instrucción ADD con un operando inmediato. Aquí hay algunos 
 * ejemplos de cómo los campos w y s afectan la instrucción:
 * 
 * Instrucción: ADD EAX, imm8 (opcode con w = 0 y s = 0)
 * 
 * Operando inmediato de 8 bits.
 * Se suma directamente a EAX sin extensión de signo.
 * Ejemplo: ADD EAX, 0x7F (suma 127 a EAX).
 * Instrucción: ADD EAX, imm32 (opcode con w = 1 y s = 0)
 * 
 * Operando inmediato de 32 bits.
 * Se suma directamente a EAX.
 * Ejemplo: ADD EAX, 0x0000007F (suma 127 a EAX).
 * Instrucción: ADD EAX, imm8 con extensión de signo (opcode con w = 1 y s = 1)
 * 
 * Operando inmediato de 8 bits.
 * Se extiende con signo a 32 bits y luego se suma a EAX.
 * Ejemplo: ADD EAX, 0xFF (suma -1 a EAX debido a la extensión de signo desde 8 bits a 32 bits, donde 0xFF se convierte en 0xFFFFFFFF).
 * ¿El campo w puede anularse si s está activo?
 * El campo w no se anula cuando el campo s está activo; más bien, trabajan juntos para definir cómo se maneja el operando inmediato:
 * 
 * w define el tamaño del operando destino.
 * s define si el operando inmediato de 8 bits se extiende con signo para ajustarse al tamaño definido por w.
 * Por lo tanto, si ambos campos están activos (w = 1 y s = 1), el operando inmediato de 8 bits se extiende con signo para convertirse en un operando de 32 bits (o 16 bits en modo real), y luego se utiliza en la operación.
 * 
 * Resumen
 * El campo w determina el tamaño del operando destino.
 * El campo s especifica si un operando inmediato de 8 bits debe ser extendido con signo para coincidir con el tamaño del operando destino.
 * Cuando ambos campos están activos (w = 1 y s = 1), el operando inmediato de 8 bits se extiende con signo a 32 bits (en modo protegido) y se utiliza en la operación.
 * En conclusión, el campo w no se anula cuando el campo s está activo; en su lugar, s indica que el operando inmediato de 8 bits debe ser sign-extended para coincidir con el tamaño especificado por w.
 * 
 * 
 * Extender con signo (sign extension) es un proceso utilizado en la computación para convertir un número de una cantidad menor de bits a una cantidad mayor de bits, preservando su valor numérico y su signo (positivo o negativo). Este proceso es crucial cuando se trata de operaciones aritméticas que involucran operandos de diferentes tamaños.
 * 
 * ¿Qué es la Extensión de Signo?
 * La extensión de signo se usa principalmente cuando tienes un número con signo en una representación más pequeña (como 8 bits) y necesitas convertirlo a una representación más grande (como 16 o 32 bits) sin cambiar su valor numérico. Esto se hace copiando el bit de signo (el bit más significativo) del número original en todos los bits adicionales de la representación más grande.
 * 
 * Ejemplo en 8 bits a 16 bits
 * Supongamos que tienes un número en 8 bits:
 * 
 * 0x7F (127 en decimal, positivo)
 * 0xFF (-1 en decimal, negativo usando complemento a dos)
 * Para extender estos números a 16 bits preservando el signo:
 * 
 * Número Positivo (0x7F):
 * 
 * Binario (8 bits): 0111 1111
 * El bit de signo es 0 (positivo).
 * Extensión de signo a 16 bits: 0000 0000 0111 1111
 * Los 8 bits adicionales a la izquierda se rellenan con 0, manteniendo el valor positivo.
 * Número Negativo (0xFF):
 * 
 * Binario (8 bits): 1111 1111
 * El bit de signo es 1 (negativo).
 * Extensión de signo a 16 bits: 1111 1111 1111 1111
 * Los 8 bits adicionales a la izquierda se rellenan con 1, manteniendo el valor negativo.
 * Ejemplo en 8 bits a 32 bits
 * Aplicando el mismo principio a la extensión de 8 bits a 32 bits:
 * 
 * Número Positivo (0x7F):
 * 
 * Binario (8 bits): 0111 1111
 * El bit de signo es 0.
 * Extensión de signo a 32 bits: 0000 0000 0000 0000 0000 0000 0111 1111
 * Número Negativo (0xFF):
 * 
 * Binario (8 bits): 1111 1111
 * El bit de signo es 1.
 * Extensión de signo a 32 bits: 1111 1111 1111 1111 1111 1111 1111 1111
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * B.1.4.8 Bit de Dirección (d)
 * En muchas instrucciones de dos operandos, un bit de dirección (d) indica qué operando se considera origen y cuál
 * es el destino. Véase la Tabla B-11.
 * - Cuando se utiliza para instrucciones enteras, el bit d se encuentra en el bit 1 de un opcode primario de 1 byte. Tenga en cuenta que este bit
 * no aparece como el símbolo "d" en la Tabla B-13; se da la codificación real del bit como 1 ó 0.
 * - Cuando se utiliza para instrucciones de punto flotante (en la Tabla B-16), el bit d se muestra como bit 2 del primer byte del
 * opcode primario.
 * 
 * Si d = 0, REG es la  fuente, MOD R/M destino,  MOD R/M <- REG.
 * Si d = 1, REG es el destino, MOD R/M fuente    REG     <- MOD R/M.
 * 
 * 
 * fuente: https://en.wikipedia.org/wiki/ModR/M
 * Los opcodes en x86 son generalmente de un byte, aunque existen instrucciones y prefijos de dos bytes. 
 * ModR/M es el byte que sigue al opcode y añade información adicional sobre cómo se ejecuta la instrucción: 
 * El formato es:
 * 
 *      | Bit | 7 6 | 5 4 3 | 2 1 0 |
 *      | Uso | MOD |  REG  |  R/M  |
 * 
 * donde REG especifica un registro y R/M puede contener un registro o especificar un modo de 
 * direccionamiento, dependiendo del valor de MOD:
 * 
 * Modo 00 (00b): El operando se codifica como un registro, sin desplazamiento (offset). En este modo, el campo 
 *      "R/M" especifica el registro.
 * 
 * Modo 01 (01b): El operando se codifica como un registro con un desplazamiento de 8 bits. En este modo, el campo 
 *      "R/M" especifica el registro, y sigue un byte inmediato que proporciona el desplazamiento.
 * 
 * Modo 10 (10b): El operando se codifica como un registro con un desplazamiento de 32 bits. En este modo, el campo 
 *      "R/M" especifica el registro, y sigue una palabra doble (dword) inmediata que proporciona el desplazamiento.
 * 
 * Modo 11 (11b): El operando se codifica como un registro de memoria. En este modo, el campo "R/M" especifica el 
 *      registro de memoria.
 * 
 * 
 * 
 * Valores de REG (Campo Reg (reg) para modos que no son de 64 bits): (pagina 2871)
 * 
 * Cuando el campo "w" no esta presente:
 * |-------------------------------------------------------------------------|
 * | reg value | si tamaño de datos es 16bits | si tamaño de datos es 32bits |
 * |-------------------------------------------------------------------------|
 * |    000    |              ax              |             eax              |
 * |    001    |              cx              |             ecx              |
 * |    010    |              dx              |             edx              |
 * |    011    |              bx              |             ebx              |
 * |    100    |              sp              |             esp              |
 * |    101    |              bp              |             ebp              |
 * |    110    |              si              |             esi              |
 * |    111    |              di              |             edi              |
 * |-------------------------------------------------------------------------|
 * 
 * 
 * Registro especificado por el campo reg Durante operaciones de datos de 16 bits (en funcion del campo "w")
 * |------------------------------------------------------------------------|
 * | reg value |          Cuando w = 0       |         Cuando w = 1         |
 * |------------------------------------------------------------------------|
 * |    000    |             al              |              ax              |
 * |    001    |             cl              |              cx              |
 * |    010    |             dl              |              dx              |
 * |    011    |             bl              |              bx              |
 * |    100    |             ah              |              sp              |
 * |    101    |             ch              |              bp              |
 * |    110    |             dh              |              si              |
 * |    111    |             bh              |              di              |
 * |------------------------------------------------------------------------|
 * 
 * Registro especificado por el campo reg Durante operaciones de datos de 32 bits
 * |------------------------------------------------------------------------|
 * | reg value |          Cuando w = 0       |         Cuando w = 1         |
 * |------------------------------------------------------------------------|
 * |    000    |             al              |             eax              |
 * |    001    |             cl              |             ecx              |
 * |    010    |             dl              |             edx              |
 * |    011    |             bl              |             ebx              |
 * |    100    |             ah              |             esp              |
 * |    101    |             ch              |             ebp              |
 * |    110    |             dh              |             esi              |
 * |    111    |             bh              |             edi              |
 * |------------------------------------------------------------------------|
 * 
 * |-------------------------------------------------------------------------------------------------------|
 * | reg value | si tamaño de datos es 8bits | si tamaño de datos es 16bits | si tamaño de datos es 32bits |
 * |-------------------------------------------------------------------------------------------------------|
 * |    000    |             al              |              ax              |             eax              |
 * |    001    |             cl              |              cx              |             ecx              |
 * |    010    |             dl              |              dx              |             edx              |
 * |    011    |             bl              |              bx              |             ebx              |
 * |    100    |             ah              |              sp              |             esp              |
 * |    101    |             ch              |              bp              |             ebp              |
 * |    110    |             dh              |              si              |             esi              |
 * |    111    |             bh              |              di              |             edi              |
 * |-------------------------------------------------------------------------------------------------------|
 * 
 * Campo Reg (reg) para el modo de 64 bits
 * Al igual que en los modos que no son de 64 bits, el campo reg del byte ModR/M especifica un operando de registro de propósito general. El grupo de registros
 * grupo de registros especificado se modifica por la presencia y el estado del bit w en una codificación (consulte la Sección
 * B.1.4.3). La Tabla B-4 muestra la codificación del campo reg cuando el bit w no está presente en una codificación; la Tabla B-5
 * muestra la codificación del campo reg cuando el bit w está presente. * 
 * 
 * |-------------------------------------------------------------------------------------------------------|
 * | reg value |si tamaño de datos es 16bits | si tamaño de datos es 32bits | si tamaño de datos es 64bits |
 * |-------------------------------------------------------------------------------------------------------|
 * |    000    |             ax              |             eax              |             rax              |
 * |    001    |             cx              |             ecx              |             rcx              |
 * |    010    |             dx              |             edx              |             rdx              |
 * |    011    |             bx              |             ebx              |             rbx              |
 * |    100    |             sp              |             esp              |             rsp              |
 * |    101    |             bp              |             ebp              |             rbp              |
 * |    110    |             si              |             esi              |             rsi              |
 * |    111    |             di              |             edi              |             rdi              |
 * |-------------------------------------------------------------------------------------------------------|
 * Registro especificado por el campo reg Durante operaciones de datos de 16 bits (en funcion del campo "w")
 * |------------------------------------------------------------------------|
 * | reg value |          Cuando w = 0       |         Cuando w = 1         |
 * |------------------------------------------------------------------------|
 * |    000    |             al              |              ax              |
 * |    001    |             cl              |              cx              |
 * |    010    |             dl              |              dx              |
 * |    011    |             bl              |              bx              |
 * |    100    |            *ah              |              sp              |
 * |    101    |            *ch              |              bp              |
 * |    110    |            *dh              |              si              |
 * |    111    |            *bh              |              di              |
 * |------------------------------------------------------------------------|
 * 
 * Registro especificado por el campo reg Durante operaciones de datos de 32 bits
 * |------------------------------------------------------------------------|
 * | reg value |          Cuando w = 0       |         Cuando w = 1         |
 * |------------------------------------------------------------------------|
 * |    000    |             al              |             eax              |
 * |    001    |             cl              |             ecx              |
 * |    010    |             dl              |             edx              |
 * |    011    |             bl              |             ebx              |
 * |    100    |            *ah              |             esp              |
 * |    101    |            *ch              |             ebp              |
 * |    110    |            *dh              |             esi              |
 * |    111    |            *bh              |             edi              |
 * |------------------------------------------------------------------------|
 *  AH, CH, DH, BH no pueden codificarse cuando se utiliza el prefijo REX. Una expresión de este tipo aparece por defecto en el byte inferior.
 * 
 * Codificación del bit de tamaño de operando (w)
 * El atributo de tamaño de operando actual determina si el procesador está realizando operaciones de 16, 32 o 64 bits. Dentro de las limitaciones del atributo de tamaño de operando actual, el bit de tamaño de operando (w) puede utilizarse para indicar
 * operaciones en operandos de 8 bits o el tamaño completo del operando especificado con el atributo operand-size. La Tabla B-6 muestra la
 * codificación del bit w dependiendo del atributo operand-size actual
 * |-------------------------------------------------------------------------------------------------|
 * | w Bit | cuando el atributo Operand-Size es 16 bits | cuando el atributo Operand-Size es 32 bits |
 * |-------------------------------------------------------------------------------------------------|
 * |   0   |                     8Bits                  |                   8Bits                    |
 * |   1   |                    16Bits                  |                  32Bits                    |
 * |-------------------------------------------------------------------------------------------------|
 * 
 * Bit de extensión de signo (s)
 * El bit de signo extendido (s) aparece en instrucciones con campos de datos inmediatos que se amplían de 8 bits a 16 ó 32 bits.
 * o 32 bits. Véase la tabla B-7.
 * |-----------------------------------------------------------------------------------------------------------------|
 * | s Bit |        cuando el atributo Operand-Size es 16 bits          | cuando el atributo Operand-Size es 32 bits |
 * |-----------------------------------------------------------------------------------------------------------------|
 * |   0   |                      Nada                                  |                    Nada                    |
 * |   1   | Extender el signo para rellenar el destino de 16 o 32 bits |                    Nada                    |
 * |-----------------------------------------------------------------------------------------------------------------|
 * 
 * Campo de registro de segmento (sreg)
 * Cuando una instrucción opera sobre un registro de segmento, el campo reg del byte ModR/M se 
 * denomina campo sreg y se utiliza para especificar el registro de segmento.
 * se utiliza para especificar el registro de segmento. La Tabla B-8 muestra la codificación del 
 * campo sreg. Este campo es a veces un campo de 2 bits (sreg2) y otras veces de 3 bits (sreg3). 
 * 
 * |----------------------------------------------------|
 * | 2 Bit sreg2 | registro de segmento seleccionado    |
 * |----------------------------------------------------|
 * |      00     |                 ES                   |
 * |      01     |                 CS                   |
 * |      10     |                 SS                   |
 * |      11     |                 DS                   |
 * |----------------------------------------------------|
 * |----------------------------------------------------|
 * | 3 Bit sreg3 | registro de segmento seleccionado    |
 * |----------------------------------------------------|
 * |     000     |                 ES                   |
 * |     001     |                 CS                   |
 * |     010     |                 SS                   |
 * |     011     |                 DS                   |
 * |     100     |                 FS                   |
 * |     101     |                 GS                   |
 * |     110     |              reservado               |
 * |     111     |              reservado               |
 * |----------------------------------------------------|
 * 
 * Campo de registro de propósito especial (eee) (pagina 2873)
 * Cuando se hace referencia a registros de control o depuración en una instrucción, se codifican en el campo eee, situado en los bits 5
 * aunque 3 del byte ModR/M (una codificación alternativa del campo sreg). Ver Tabla B-9.
 * 
 *
 * B.1.4.7 Campo de Prueba de Condición (tttn)
 * Para instrucciones condicionales (como saltos condicionales y set on condition), el campo de prueba de condición (tttn) está
 * codifica la condición que se está comprobando. La parte ttt del campo indica la condición a comprobar y la parte n indica
 * indica si se utiliza la condición (n = 0) o su negación (n = 1).
 * - Para los opcodes primarios de 1 byte, el campo tttn se encuentra en los bits 3, 2, 1 y 0 del byte del opcode.
 * - Para opcodes primarios de 2 bytes, el campo tttn se encuentra en los bits 3, 2, 1 y 0 del segundo byte del opcode.
 * La Tabla B-10 muestra la codificación del campo tttn.
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
 * 
 * 
 * Casi todas las instrucciones que hacen referencia a un registro y/o a un operando de memoria tienen un byte de modo de registro y/o de dirección
 * a continuación del opcode. Este byte, el byte ModR/M, consta del campo mod (2 bits), el campo reg (3 bits; este campo
 * es a veces una extensión del opcode), y el campo R/M (3 bits). Algunas codificaciones del byte ModR/M indican que
 * debe utilizarse un segundo byte de modo de direccionamiento, el byte SIB.
 * Si el modo de direccionamiento especifica un desplazamiento, el valor del desplazamiento se coloca inmediatamente después del byte
 * byte ModR/M o byte SIB. Los tamaños posibles son 8, 16 o 32 bits. Si la instrucción especifica un valor inmediato, el valor
 * valor inmediato sigue a cualquier byte de desplazamiento. El valor inmediato, si se especifica, es siempre el último campo de la instrucción.
 * Consulte el Capítulo 2, "Formato de las instrucciones", del Manual del desarrollador de software de las arquitecturas Intel® 64 e IA-32,
 * Volumen 2A, para obtener más información sobre los opcodes.
 * 
 *  MOD R/M Byte and Addressing Modes:
 * 
 *  ModR/M especifica los operandos de una instrucción. El operando puede ser 
 *  un registro, una posición de memoria o un valor inmediato. Este 
 *  componente de una instrucción consta de 3 partes más pequeñas: 
 *  ✄ El campo mod, o campo modificador, se combina con el campo r/m para 
 *       un total de 5 bits de información para codificar 32 
 *       valores posibles: 8 registros y 24 modos de direccionamiento.
 *  ✄ El campo reg/opcode codifica un operando de registro o amplía el campo Campo opcode con 3 bits más.
 *  ✄ El campo r/m codifica un operando de registro o puede combinarse con 
 *       el campo mod para codificar un modo de direccionamiento.
 * 
 * campo MOD:
 *      - 00 Modo de direccionamiento indirecto de registro o SIB sin desplazamiento (cuando R/M = 100) o modo de direccionamiento de sólo desplazamiento (cuando R/M = 101).
 *      - 01 El desplazamiento con signo de un byte sigue a los bytes del modo de direccionamiento.
 *      - 10 El desplazamiento con signo de cuatro bytes sigue a los bytes del modo de direccionamiento.
 *      - 11 Modo de direccionamiento de registro.
 * 
 * mod A anula el valor 11 en mod. por lo tanto solo se puede usar mod 10, 01 y mod 00
 * para mod B se anula los valores 01 y 10 en mod. por lo tanto solo se puede usar mod 11 y mod 00ç
 * para mod AB se anula los valores 00, 01 y 10. por lo tanto solo se puede usar mod 00
 * 
 * Formas con el Byte ModR/M para 32bits y 16bits:
 * |--------------------------------------------------------------------------------------------------------------|
 * | mod |        00        |                 01              |                  10               |       11      |
 * | rm  | 16bits  | 32bits |      16bits     |    32bits     |      16bits      |      32bits    |   r/m // REB  |
 * |--------------------------------------------------------------------------------------------------------------|
 * | 000 | [bx+si] | [eax]  | [bx+si] + disp8 | [eax] + disp8 | [bx+si] + disp16 | [eax] + disp32 | al / ax / eax |
 * | 001 | [bx+di] | [ecx]  | [bx+di] + disp8 | [ecx] + disp8 | [bx+di] + disp16 | [ecx] + disp32 | cl / cx / ecx |
 * | 010 | [bp+si] | [edx]  | [bp+si] + disp8 | [edx] + disp8 | [bp+si] + disp16 | [edx] + disp32 | dl / dx / edx |
 * | 011 | [bp+di] | [ebx]  | [bp+di] + disp8 | [ebx] + disp8 | [bp+di] + disp16 | [ebx] + disp32 | bl / bx / ebx |
 * | 100 |  [si]   |  sib   |    [si] + disp8 |  sib  + disp8 |    [si] + disp16 |  sib  + disp32 | ah / sp / esp |
 * | 101 |  [di]   | disp32 |    [di] + disp8 | [ebp] + disp8 |    [di] + disp16 | [ebp] + disp32 | ch / bp / ebp |
 * | 110 | disp16  | [esi]  |    [bp] + disp8 | [esi] + disp8 |    [bp] + disp16 | [esi] + disp32 | dh / si / esi |
 * | 111 |  [bx]   | [edi]  |    [bx] + disp8 | [edi] + disp8 |    [bx] + disp16 | [edi] + disp32 | bh / di / edi |
 * |--------------------------------------------------------------------------------------------------------------|
 * 
 * La nomenclatura disp16 denota un desplazamiento de 16 bits que sigue al byte ModR/M y que se añade al índice.
 * 
 * Formas con el Byte ModR/M solo para 32bits:
 *  | === | === | ================================= |
 *  | MOD | R/M | Addressing Mode                   | 
 *  | === | === | ================================= |
 *  |  00 | 000 | [ eax ]                           |            
 *  |  01 | 000 | [ eax + disp8 ]               (1) |                   
 *  |  10 | 000 | [ eax + disp32 ]                  |  
 *  |  11 | 000 | register  ( al / ax / eax )   (2) |
 *  |  00 | 001 | [ ecx ]                           |            
 *  |  01 | 001 | [ ecx + disp8 ]                   | 
 *  |  10 | 001 | [ ecx + disp32 ]                  |                     
 *  |  11 | 001 | register  ( cl / cx / ecx )       |             
 *  |  00 | 010 | [ edx ]                           |            
 *  |  01 | 010 | [ edx + disp8 ]                   |                                                          
 *  |  10 | 010 | [ edx + disp32 ]                  |                     
 *  |  11 | 010 | register  ( dl / dx / edx )       |                                
 *  |  00 | 011 | [ ebx ]                           |            
 *  |  01 | 011 | [ ebx + disp8 ]                   |                    
 *  |  10 | 011 | [ ebx + disp32 ]                  |                     
 *  |  11 | 011 | register  ( bl / bx / ebx )       |             
 *  |  00 | 100 | SIB  Mode                     (3) |                   
 *  |  01 | 100 | SIB  +  disp8  Mode               |     
 *  |  10 | 100 | SIB  +  disp32  Mode              |                         
 *  |  11 | 100 | register  ( ah / sp / esp )       |             
 *  |  00 | 101 | 32-bit Displacement-Only Mode (4) |                   
 *  |  01 | 101 | [ ebp + disp8 ]                   |                    
 *  |  10 | 101 | [ ebp + disp32 ]                  |                     
 *  |  11 | 101 | register  ( ch / bp / ebp )       |             
 *  |  00 | 110 | [ esi ]                           |            
 *  |  01 | 110 | [ esi + disp8 ]                   |                    
 *  |  10 | 110 | [ esi + disp32 ]                  |                     
 *  |  11 | 110 | register  ( dh / si / esi )       |                                
 *  |  00 | 111 | [ edi ]                           |            
 *  |  01 | 111 | [ edi + disp8 ]                   |                    
 *  |  10 | 111 | [ edi + disp32 ]                  |                     
 *  |  11 | 111 | register  ( bh / di / edi )       |     
 *  | === | === | ================================= |       
 * 
 * 
 *    
 * 
 * Los modos de direccionamiento con desplazamiento de 8 bits caen en el 
 * rango -128..+127 y sólo requieren un desplazamiento de un byte después del opcode (¡Más rápido!)
 * El bit de tamaño en el opcode especifica el tamaño del registro de 8 o 32 bits. Para seleccionar un 
 * registro de 16 bits se requiere un byte de prefijo.
 * Los llamados modos de direccionamiento indexado escalado, SIB = scaled index byte mode.
 * Nótese que no hay direccionamiento [ ebp ]. Su ranura está ocupada por el modo de direccionamiento de 
 * sólo desplazamiento de 32 bits. Intel decidió que los programadores pueden usar el modo de direccionamiento 
 * [ ebp+ disp8 ] en su lugar, con su desplazamiento de 8 bits igual a cero (aunque la instrucción es un poco más larga).                 
 * 
 * 
 * 
 * Algunas instrucciones no pueden hacer uso de la parte REG del byte ModR/M. Muchas de estas 
 * instrucciones son "multiplexadas". Muchas de estas instrucciones están "multiplexadas" utilizando 
 * este campo, donde un único opcode puede referirse a múltiples instrucciones, y el campo REG determina 
 * la instrucción. En los listados de opcode, éstos se especifican siguiendo el opcode con una barra (/) y 
 * un dígito 0-7.
 * 
 * fuente: http://www.c-jump.com/CIS77/CPU/x86/X77_0060_mod_reg_r_m_byte.htm
 *         https://www.cs.loyola.edu/~binkley/371/Encoding_Real_x86_Instructions.html
 *         http://www.c-jump.com/CIS77/images/x86_instruction_format.png
 * 
 * El campo R/M, combinado con MOD, especifica o bien
 * el segundo operando en una instrucción de dos operandos, o bien
 * el único operando en una instrucción de un solo operando como NOT o NEG.
 * El bit d del opcode determina qué operando es la fuente y cuál es el destino:
 *      d = 0: MOD R/M <- REG, REG es la fuente
 *      d = 1: REG <- MOD R/M, REG es el destino
 *      
 * (*) Técnicamente, los registros no tienen dirección, pero aun así aplicamos el término modo de direccionamiento a los registros.
 * 
 * Cambios en 64 bits:
 * El byte ModR/M es fundamental para los cambios introducidos con la extensión de 64 bits de AMD al 
 * conjunto de instrucciones original. En modo largo, cualquier opcode cuyos cuatro bits más altos 
 * sean 0100 (decimal 4) se considera un nuevo prefijo, el prefijo REX
 * Los cuatro bits más bajos del byte de prefijo sirven para varios propósitos, 
 * incluyendo un bit extra para los campos REG y R/M del byte ModR/M que le sigue. 
 * Entre otros cambios, la ampliación de estos valores de tres bits a cuatro duplica el número 
 * de registros disponibles en el procesador de ocho a dieciséis.
 * 
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
 * Los bytes SIB tienen un formato similar a los bytes ModR/M, y toman la forma de 
 * (scale * index) + base + displacement, donde SCALE es 1, 2, 4, u 8. BASE e INDEX codifican cada 
 * registro. BASE e INDEX codifican cada uno un registro[3] El desplazamiento es un desplazamiento constante 
 * codificado después del byte SIB que se aplica a la dirección final[4].
 * 
 * Un prefijo REX puede permitir opcionalmente que el byte SIB utilice registros SSE.
 * 
 *                          |-----|-------|----------------|-------|
 *                          | Uso | SCALE |     INDEX      |  BASE |
 *                          | Bit |  7 6  |     5 4 3      | 2 1 0 |
 *                          |-----|-v-----|-----v----------|---v---|
 *      ____________________________|           |_________     |_____________________
 *      |                                                 |                          |
 *  |---v-------------|------------------------ |       |-v-----|----------|       |-v-----|----------------------------------------------------|
 *  | valor de escala | Index * valor de escala |       | index | registro |       |  base |                    registro                        |
 *  |-----------------|-------------------------|       |-------|----------|       |-------|----------------------------------------------------|
 *  |       00        |        index * 1        |       |  000  |   eax    |       |  000  |   eax                                              |
 *  |       01        |        index * 2        |       |  001  |   ecx    |       |  001  |   ecx                                              |
 *  |       10        |        index * 4        |       |  010  |   edx    |       |  010  |   edx                                              |
 *  |       11        |        index * 8        |       |  011  |   ebx    |       |  011  |   ebx                                              |
 *  |-----------------|-------------------------|       |  100  |  ilegal  |       |  100  |   esp                                              |
 *                                                      |  101  |   ebp    |       |  101  |  desplazar solo si MOD es 00, EBP si MOD = 01 o 10 |
 *                                                      |  110  |   esi    |       |  110  |   esi                                              |
 *                                                      |  111  |   edi    |       |  111  |   edi                                              |
 *                                                      |-------|----------|       |-------|----------------------------------------------------|
 * 
 * 
 * Modo de direccionamiento indexado escalonado:
 * Nota: n = 1, 2, 4, u 8.
 * En cada modo de direccionamiento indexado escalado, el campo MOD del byte MOD-REG-R/M especifica el tamaño del desplazamiento. Puede ser cero, uno o cuatro bytes:
 *     MOD R/M Modo de direccionamiento
 *     --- --- --------------------------- 
 *      00 100 SIB
 *      01 100 SIB + disp8
 *      10 100 SIB + disp32
 * 
 * Los campos Base e Índice del byte SIB seleccionan los registros base e índice, respectivamente.
 * Tenga en cuenta que este modo de direccionamiento no permite el uso del registro ESP como registro de índice. Presumiblemente, 
 * Intel dejó este modo particular sin definir para ofrecer la posibilidad de ampliar los modos de direccionamiento en una versión futura de la CPU.
 * 
 * [ reg32 + eax*n ] MOD = 00
 * [ reg32 + ebx*n ] 
 * [ reg32 + ecx*n ]
 * [ reg32 + edx*n ]
 * [ reg32 + ebp*n ]
 * [ reg32 + esi*n ]
 * [ reg32 + edi*n ]
 * 
 * [ disp + reg8 + eax*n ] MOD = 01
 * [ disp + reg8 + ebx*n ]
 * [ disp + reg8 + ecx*n ]
 * [ disp + reg8 + edx*n ]
 * [ disp + reg8 + ebp*n ]
 * [ disp + reg8 + esi*n ]
 * [ disp + reg8 + edi*n ]
 * 
 * [ disp + reg32 + eax*n ] MOD = 10
 * [ disp + reg32 + ebx*n ]
 * [ disp + reg32 + ecx*n ]
 * [ disp + reg32 + edx*n ]
 * [ disp + reg32 + ebp*n ]
 * [ disp + reg32 + esi*n ]
 * [ disp + reg32 + edi*n ]
 * 
 * [ disp + eax*n ] MOD = 00, and
 * [ disp + ebx*n ] BASE field = 101
 * [ disp + ecx*n ]
 * [ disp + edx*n ]
 * [ disp + ebp*n ]
 * [ disp + esi*n ]
 * [ disp + edi*n ]
 * 
 * 
 * Instrucciones de proposito general y formatos para modos no de 64bits(pagina 2875)
 * ADD – Add 
 *  register1 to register2      -> 0000 000w : mod(11)       reg1 reg2 (si d no esta activo)
 *  register2 to register1      -> 0000 001w : mod(11)       reg1 reg2 (si d si esta activo)
 *  memory to register          -> 0000 001w : mod           reg  r/m
 *  register to memory          -> 0000 000w : mod           reg  r/m
 *  immediate to register       -> 1000 00sw : mod(11) ¿r/m?(000) reg : immediate data
 *  immediate to AL, AX, or EAX -> 0000 010w :                          immediate data
 *  immediate to memory         -> 1000 00sw : mod       reg(000) r/m : immediate data
 * 
 * AND – Logical AND
 *  register1 to register2 0010 000w : 11 reg1 reg2
 *  register2 to register1 0010 001w : 11 reg1 reg2
 *  memory to register 0010 001w : mod reg r/m
 *  register to memory 0010 000w : mod reg r/m
 *  immediate to register 1000 00sw : 11 100 reg : immediate data
 *  immediate to AL, AX, or EAX 0010 010w : immediate data
 *  immediate to memory 1000 00sw : mod 100 r/m : immediate data
 * 
 * ARPL – Adjust RPL Field of Selector
 *  from register 0110 0011 : 11 reg1 reg2
 *  from memory 0110 0011 : mod reg r/m
 * 
 * BOUND – Check Array Against Bounds 0110 0010 : modA reg r/m
 * 
 * BSF – Bit Scan Forward
 *  register1, register2 0000 1111 : 1011 1100 : 11 reg1 reg2
 *  memory, register 0000 1111 : 1011 1100 : mod reg r/m
 *
 * BSR – Bit Scan Reverse
 *  register1, register2 0000 1111 : 1011 1101 : 11 reg1 reg2
 *  memory, register 0000 1111 : 1011 1101 : mod reg r/m
 * 
 * BSWAP – Byte Swap 0000 1111 : 1100 1 reg
 * 
 * BT – Bit Test
 *  register, immediate 0000 1111 : 1011 1010 : 11 100 reg: imm8 data
 *  memory, immediate 0000 1111 : 1011 1010 : mod 100 r/m : imm8 data
 *  register1, register2 0000 1111 : 1010 0011 : 11 reg2 reg1
 *  memory, reg 0000 1111 : 1010 0011 : mod reg r/m
 * 
 * BTC – Bit Test and Complement
 *  register, immediate 0000 1111 : 1011 1010 : 11 111 reg: imm8 data
 *  memory, immediate 0000 1111 : 1011 1010 : mod 111 r/m : imm8 data
 *  register1, register2 0000 1111 : 1011 1011 : 11 reg2 reg1
 *  memory, reg 0000 1111 : 1011 1011 : mod reg r/m
 * 
 * BTR – Bit Test and Reset
 *  register, immediate 0000 1111 : 1011 1010 : 11 110 reg: imm8 data
 *  memory, immediate 0000 1111 : 1011 1010 : mod 110 r/m : imm8 data
 *  register1, register2 0000 1111 : 1011 0011 : 11 reg2 reg1
 *  memory, reg 0000 1111 : 1011 0011 : mod reg r/m
 * 
 * BTS – Bit Test and Set
 *  register, immediate 0000 1111 : 1011 1010 : 11 101 reg: imm8 data
 *  memory, immediate 0000 1111 : 1011 1010 : mod 101 r/m : imm8 data
 *  register1, register2 0000 1111 : 1010 1011 : 11 reg2 reg1
 *  memory, reg 0000 1111 : 1010 1011 : mod reg r/m
 * 
 * CALL – Call Procedure (in same segment)
 *  direct 1110 1000 : full displacement
 *  register indirect 1111 1111 : 11 010 reg
 *  memory indirect 1111 1111 : mod 010 r/m
 * 
 * CALL – Call Procedure (in other segment)
 *  direct 1001 1010 : unsigned full offset, selector
 *  indirect 1111 1111 : mod 011 r/m
 * 
 * CBW – Convert Byte to Word 1001 1000
 * CDQ – Convert Doubleword to Qword 1001 1001
 * CLC – Clear Carry Flag 1111 1000
 * CLD – Clear Direction Flag 1111 1100
 * CLI – Clear Interrupt Flag 1111 1010
 * CLTS – Clear Task-Switched Flag in CR0 0000 1111 : 0000 0110
 * CMC – Complement Carry Flag 1111 0101
 * 
 * CMP – Compare Two Operands
 *  register1 with register2      0011 100w : 11 reg1 reg2
 *  register2 with register1      0011 101w : 11 reg1 reg2
 *  memory with register          0011 100w : mod reg r/m
 *  register with memory          0011 101w : mod reg r/m
 *  immediate with register       1000 00sw : 11 111 reg : immediate data
 *  immediate with AL, AX, or EAX 0011 110w : immediate data
 *  immediate with memory         1000 00sw : mod 111 r/m : immediate data
 * 
 * CMPS/CMPSB/CMPSW/CMPSD – Compare String Operands 1010 011w
 * 
 * CMPXCHG – Compare and Exchange
 *  register1, register2 0000 1111 : 1011 000w : 11 reg2 reg1
 *  memory, register 0000 1111 : 1011 000w : mod reg r/m
 * CPUID – CPU Identification 0000 1111 : 1010 0010
 * 
 * CWD – Convert Word to Doubleword 1001 1001
 * 
 * CWDE – Convert Word to Doubleword 1001 1000
 * 
 * DAA – Decimal Adjust AL after Addition 0010 0111
 * 
 * DAS – Decimal Adjust AL after Subtraction 0010 1111
 * 
 * DEC – Decrement by 1
 *  register 1111 111w : 11 001 reg
 *  register (alternate encoding) 0100 1 reg
 *  memory 1111 111w : mod 001 r/m
 * 
 * DIV – Unsigned Divide
 *  AL, AX, or EAX by register 1111 011w : 11 110 reg
 *  AL, AX, or EAX by memory 1111 011w : mod 110 r/m
 * 
 * HLT – Halt 1111 0100
 * 
 * IDIV – Signed Divide
 *  AL, AX, or EAX by register 1111 011w : 11 111 reg
 *  AL, AX, or EAX by memory 1111 011w : mod 111 r/m
 * 
 * IMUL – Signed Multiply
 *  AL, AX, or EAX with register 1111 011w : 11 101 reg
 *  AL, AX, or EAX with memory 1111 011w : mod 101 reg
 *  register1 with register2 0000 1111 : 1010 1111 : 11 : reg1 reg2
 *  register with memory 0000 1111 : 1010 1111 : mod reg r/m
 *  register1 with immediate to register2 0110 10s1 : 11 reg1 reg2 : immediate data
 *  memory with immediate to register 0110 10s1 : mod reg r/m : immediate data
 * 
 * IN – Input From Port
 *  fixed port 1110 010w : port number
 *  variable port 1110 110w
 * 
 * INC – Increment by 1
 *  reg 1111 111w : 11 000 reg
 *  reg (alternate encoding) 0100 0 reg
 *  memory 1111 111w : mod 000 r/m
 * 
 * INS – Input from DX Port 0110 110w
 * 
 * INT n – Interrupt Type n 1100 1101 : type
 * 
 * INT – Single-Step Interrupt 3 1100 1100
 * 
 * INTO – Interrupt 4 on Overflow 1100 1110
 * 
 * INVD – Invalidate Cache 0000 1111 : 0000 1000
 * 
 * INVLPG – Invalidate TLB Entry 0000 1111 : 0000 0001 : mod 111 r/m
 * 
 * INVPCID – Invalidate Process-Context Identifier 0110 0110:0000 1111:0011 1000:1000 0010: mod reg r/m
 * 
 * IRET/IRETD – Interrupt Return 1100 1111
 * 
 * Jcc – Jump if Condition is Met
 *  8-bit displacement 0111 tttn : 8-bit displacement
 *  full displacement 0000 1111 : 1000 tttn : full displacement
 * 
 * JCXZ/JECXZ – Jump on CX/ECX Zero
 *  Address-size prefix differentiates JCXZ
 *  and JECXZ
 * 
 * JMP – Unconditional Jump (to same segment)
 *  short 1110 1011 : 8-bit displacement
 *  direct 1110 1001 : full displacement
 *  register indirect 1111 1111 : 11 100 reg
 *  memory indirect 1111 1111 : mod 100 r/m
 * 
 * JMP – Unconditional Jump (to other segment)
 *  direct intersegment 1110 1010 : unsigned full offset, selector
 *  indirect intersegment 1111 1111 : mod 101 r/m
 * 
 * LAHF – Load Flags into AHRegister 1001 1111
 * 
 * LAR – Load Access Rights Byte
 *  from register 0000 1111 : 0000 0010 : 11 reg1 reg2
 *  from memory 0000 1111 : 0000 0010 : mod reg r/m
 * 
 * LDS – Load Pointer to DS 1100 0101 : modA,B reg r/m
 * 
 * LEA – Load Effective Address 1000 1101 : modA reg r/m
 * 
 * LEAVE – High Level Procedure Exit 1100 1001
 * 
 * LES – Load Pointer to ES 1100 0100 : modA,B reg r/m
 * 
 * LFS – Load Pointer to FS 0000 1111 : 1011 0100 : modA reg r/m
 * 
 * LGDT – Load Global Descriptor Table Register 0000 1111 : 0000 0001 : modA 010 r/m
 * 
 * LGS – Load Pointer to GS 0000 1111 : 1011 0101 : modA reg r/m
 * 
 * LIDT – Load Interrupt Descriptor Table Register 0000 1111 : 0000 0001 : modA 011 r/m
 * 
 * LLDT – Load Local Descriptor Table Register
 *  LDTR from register 0000 1111 : 0000 0000 : 11 010 reg
 *  LDTR from memory 0000 1111 : 0000 0000 : mod 010 r/m
 * 
 * LMSW – Load Machine Status Word
 *  from register 0000 1111 : 0000 0001 : 11 110 reg
 *  from memory 0000 1111 : 0000 0001 : mod 110 r/m
 * 
 * LOCK – Assert LOCK# Signal Prefix 1111 0000
 * 
 * LODS/LODSB/LODSW/LODSD – Load String Operand 1010 110w
 * 
 * LOOP – Loop Count 1110 0010 : 8-bit displacement
 * 
 * LOOPZ/LOOPE – Loop Count while Zero/Equal 1110 0001 : 8-bit displacement
 * 
 * LOOPNZ/LOOPNE – Loop Count while not Zero/Equal 1110 0000 : 8-bit displacement
 * 
 * LSL – Load Segment Limit
 *  from register 0000 1111 : 0000 0011 : 11 reg1 reg2
 *  from memory 0000 1111 : 0000 0011 : mod reg r/m
 * 
 * LSS – Load Pointer to SS 0000 1111 : 1011 0010 : modA reg r/m
 * 
 * LTR – Load Task Register
 *  from register 0000 1111 : 0000 0000 : 11 011 reg
 *  from memory 0000 1111 : 0000 0000 : mod 011 r/m
 * 
 * MOV – Move Data
 *  register1 to register2                     1000 100w  : 11 reg1 reg2
 *  register2 to register1                     1000 101w  : 11 reg1 reg2
 *  memory to reg                              1000 101w  : mod reg r/m
 *  reg to memory                              1000 100w  : mod reg r/m
 *  immediate to register                      1100 011w  : 11 000 reg : immediate data
 *  immediate to register (alternate encoding) 1011 w reg : immediate data
 *  immediate to memory                        1100 011w  : mod 000 r/m : immediate data
 *  memory to AL, AX, or EAX                   1010 000w  : full displacement
 *  AL, AX, or EAX to memory                   1010 001w  : full displacement
 * 
 * MOV – Move to/from Control Registers
 *  CR0 from register 0000 1111 : 0010 0010 : -- 000 reg
 *  CR2 from register 0000 1111 : 0010 0010 : -- 010reg
 *  CR3 from register 0000 1111 : 0010 0010 : -- 011 reg
 *  CR4 from register 0000 1111 : 0010 0010 : -- 100 reg
 *  register from CR0-CR4 0000 1111 : 0010 0000 : -- eee reg
 * 
 * MOV – Move to/from Debug Registers
 *  DR0-DR3 from register 0000 1111 : 0010 0011 : -- eee reg
 *  DR4-DR5 from register 0000 1111 : 0010 0011 : -- eee reg
 *  DR6-DR7 from register 0000 1111 : 0010 0011 : -- eee reg
 *  register from DR6-DR7 0000 1111 : 0010 0001 : -- eee reg
 *  register from DR4-DR5 0000 1111 : 0010 0001 : -- eee reg
 *  register from DR0-DR3 0000 1111 : 0010 0001 : -- eee reg
 * 
 * MOV – Move to/from Segment Registers
 *  register to segment register 1000 1110 : 11 sreg3 reg
 *  register to SS 1000 1110 : 11 sreg3 reg
 *  memory to segment reg 1000 1110 : mod sreg3 r/m
 *  memory to SS 1000 1110 : mod sreg3 r/m
 *  segment register to register 1000 1100 : 11 sreg3 reg
 *  segment register to memory 1000 1100 : mod sreg3 r/m
 * 
 * MOVBE – Move data after swapping bytes
 *  memory to register 0000 1111 : 0011 1000:1111 0000 : mod reg r/m
 *  register to memory 0000 1111 : 0011 1000:1111 0001 : mod reg r/m
 * 
 * MOVS/MOVSB/MOVSW/MOVSD – Move Data from String to String
 *  1010 010w
 * 
 * MOVSX – Move with Sign-Extend
 *  memory to reg 0000 1111 : 1011 111w : mod reg r/m
 * 
 * MOVZX – Move with Zero-Extend
 *  register2 to register1 0000 1111 : 1011 011w : 11 reg1 reg2
 *  memory to register 0000 1111 : 1011 011w : mod reg r/m
 * 
 * MUL – Unsigned Multiply
 *  AL, AX, or EAX with register 1111 011w : 11 100 reg
 *  AL, AX, or EAX with memory 1111 011w : mod 100 r/m
 * 
 * NEG – Two's Complement Negation
 *  register 1111 011w : 11 011 reg
 *  memory 1111 011w : mod 011 r/m
 * 
 * NOP – No Operation 1001 0000
 * 
 * NOP – Multi-byte No Operation1 ( 1. La instrucción NOP multibyte no altera el contenido del registro y no emitirá una operación de memoria. )
 *  register 0000 1111 0001 1111 : 11 000 reg
 *  memory   0000 1111 0001 1111 : mod 000 r/m
 * 
 * NOT – One's Complement Negation
 *  register 1111 011w : 11 010 reg
 *  memory 1111 011w : mod 010 r/m
 * 
 * OR – Logical Inclusive OR
 *  register1 to register2 0000 100w : 11 reg1 reg2
 *  register2 to register1 0000 101w : 11 reg1 reg2
 *  memory to register 0000 101w : mod reg r/m
 *  register to memory 0000 100w : mod reg r/m
 *  immediate to register 1000 00sw : 11 001 reg : immediate data
 *  immediate to AL, AX, or EAX 0000 110w : immediate data
 *  immediate to memory 1000 00sw : mod 001 r/m : immediate data
 * 
 * OUT – Output to Port
 *  fixed port 1110 011w : port number
 *  variable port 1110 111w
 * 
 * OUTS – Output to DX Port 0110 111w
 * 
 * POP – Pop a Word from the Stack
 *  register 1000 1111 : 11 000 reg
 *  register (alternate encoding) 0101 1 reg
 *  memory 1000 1111 : mod 000 r/m
 * 
 * POP – Pop a Segment Register from the Stack (Note: CS cannot be sreg2 in this usage.)
 *  segment register DS, ES 000 sreg2 111
 *  segment register SS 000 sreg2 111
 *  segment register FS, GS 0000 1111: 10 sreg3 001
 * 
 * POPA/POPAD – Pop All General Registers 0110 0001
 * 
 * POPF/POPFD – Pop Stack into FLAGS or EFLAGS Register 1001 1101
 * 
 * PUSH – Push Operand onto the Stack
 *  register 1111 1111 : 11 110 reg
 *  register (alternate encoding) 0101 0 reg
 *  memory 1111 1111 : mod 110 r/m
 *  immediate 0110 10s0 : immediate data
 * 
 * PUSH – Push Segment Register onto the Stack
 *  segment register CS,DS,ES,SS 000 sreg2 110
 *  segment register FS,GS 0000 1111: 10 sreg3 000
 * 
 * PUSHA/PUSHAD – Push All General Registers 0110 0000
 * 
 * PUSHF/PUSHFD – Push Flags Register onto the Stack 1001 1100
 * 
 * RCL – Rotate thru Carry Left
 *  register by 1 1101 000w : 11 010 reg
 *  memory by 1 1101 000w : mod 010 r/m
 *  register by CL 1101 001w : 11 010 reg
 *  memory by CL 1101 001w : mod 010 r/m
 *  register by immediate count 1100 000w : 11 010 reg : imm8 data
 *  memory by immediate count 1100 000w : mod 010 r/m : imm8 data
 * 
 * RCR – Rotate thru Carry Right
 *  register by 1 1101 000w : 11 011 reg
 *  memory by 1 1101 000w : mod 011 r/m
 *  register by CL 1101 001w : 11 011 reg
 *  memory by CL 1101 001w : mod 011 r/m
 *  register by immediate count 1100 000w : 11 011 reg : imm8 data
 *  memory by immediate count 1100 000w : mod 011 r/m : imm8 data
 * 
 * RDMSR – Read from Model-Specific Register 0000 1111 : 0011 0010
 * 
 * RDPMC – Read Performance Monitoring Counters 0000 1111 : 0011 0011
 * 
 * RDTSC – Read Time-Stamp Counter 0000 1111 : 0011 0001
 * 
 * RDTSCP – Read Time-Stamp Counter and Processor ID 0000 1111 : 0000 0001: 1111 1001
 * 
 * REP INS – Input String 1111 0011 : 0110 110w
 * 
 * REP LODS – Load String 1111 0011 : 1010 110w
 * 
 * REP MOVS – Move String 1111 0011 : 1010 010w
 * 
 * REP OUTS – Output String 1111 0011 : 0110 111w
 * 
 * REP STOS – Store String 1111 0011 : 1010 101w
 * 
 * REPE CMPS – Compare String 1111 0011 : 1010 011w
 * 
 * REPE SCAS – Scan String 1111 0011 : 1010 111w
 * 
 * REPNE CMPS – Compare String 1111 0010 : 1010 011w
 * 
 * REPNE SCAS – Scan String 1111 0010 : 1010 111w
 * 
 * RET – Return from Procedure (to same segment)
 *  no argument 1100 0011
 *  adding immediate to SP 1100 0010 : 16-bit displacement
 * 
 * RET – Return from Procedure (to other segment)
 *  intersegment 1100 1011
 *  adding immediate to SP 1100 1010 : 16-bit displacement
 * 
 * ROL – Rotate Left
 *  register by 1 1101 000w : 11 000 reg
 *  memory by 1 1101 000w : mod 000 r/m
 *  register by CL 1101 001w : 11 000 reg
 *  memory by CL 1101 001w : mod 000 r/m
 *  register by immediate count 1100 000w : 11 000 reg : imm8 data
 *  memory by immediate count 1100 000w : mod 000 r/m : imm8 data
 * 
 * ROR – Rotate Right
 *  register by 1 1101 000w : 11 001 reg
 *  memory by 1 1101 000w : mod 001 r/m
 *  register by CL 1101 001w : 11 001 reg
 *  memory by CL 1101 001w : mod 001 r/m
 *  register by immediate count 1100 000w : 11 001 reg : imm8 data
 *  memory by immediate count 1100 000w : mod 001 r/m : imm8 data
 * 
 * RSM – Resume from System Management Mode 0000 1111 : 1010 1010
 * 
 * SAHF – Store AH into Flags 1001 1110
 * 
 * SAL – Shift Arithmetic Left same instruction as SHL
 * 
 * SAR – Shift Arithmetic Right
 *  register by 1 1101 000w : 11 111 reg
 *  memory by 1 1101 000w : mod 111 r/m
 *  register by CL 1101 001w : 11 111 reg
 *  memory by CL 1101 001w : mod 111 r/m
 *  register by immediate count 1100 000w : 11 111 reg : imm8 data
 *  memory by immediate count 1100 000w : mod 111 r/m : imm8 data
 * 
 * SBB – Integer Subtraction with Borrow
 *  register1 to register2 0001 100w : 11 reg1 reg2
 *  register2 to register1 0001 101w : 11 reg1 reg2
 *  memory to register 0001 101w : mod reg r/m
 *  register to memory 0001 100w : mod reg r/m
 *  immediate to register 1000 00sw : 11 011 reg : immediate data
 *  immediate to AL, AX, or EAX 0001 110w : immediate data
 *  immediate to memory 1000 00sw : mod 011 r/m : immediate data
 * 
 * SCAS/SCASB/SCASW/SCASD – Scan String 1010 111w
 * 
 * SETcc – Byte Set on Condition
 *  register 0000 1111 : 1001 tttn : 11 000 reg
 *  memory 0000 1111 : 1001 tttn : mod 000 r/m
 * 
 * SGDT – Store Global Descriptor Table Register 0000 1111 : 0000 0001 : modA 000 r/m
 * 
 * 
 * SHL – Shift Left
 *  register by 1 1101 000w : 11 100 reg
 *  memory by 1 1101 000w : mod 100 r/m
 *  register by CL 1101 001w : 11 100 reg
 *  memory by CL 1101 001w : mod 100 r/m
 *  register by immediate count 1100 000w : 11 100 reg : imm8 data
 *  memory by immediate count 1100 000w : mod 100 r/m : imm8 data
 * 
 * SHLD – Double Precision Shift Left
 *  register by immediate count 0000 1111 : 1010 0100 : 11 reg2 reg1 : imm8
 *  memory by immediate count 0000 1111 : 1010 0100 : mod reg r/m : imm8
 *  register by CL 0000 1111 : 1010 0101 : 11 reg2 reg1
 *  memory by CL 0000 1111 : 1010 0101 : mod reg r/m
 * 
 * SHR – Shift Right
 *  register by 1 1101 000w : 11 101 reg
 *  memory by 1 1101 000w : mod 101 r/m
 *  register by CL 1101 001w : 11 101 reg
 *  memory by CL 1101 001w : mod 101 r/m
 *  register by immediate count 1100 000w : 11 101 reg : imm8 data
 *  memory by immediate count 1100 000w : mod 101 r/m : imm8 data
 * 
 * SHRD – Double Precision Shift Right
 *  register by immediate count 0000 1111 : 1010 1100 : 11 reg2 reg1 : imm8
 *  memory by immediate count 0000 1111 : 1010 1100 : mod reg r/m : imm8
 *  register by CL 0000 1111 : 1010 1101 : 11 reg2 reg1
 *  memory by CL 0000 1111 : 1010 1101 : mod reg r/m
 * 
 * SIDT – Store Interrupt Descriptor Table Register 0000 1111 : 0000 0001 : modA 001 r/m
 * 
 * SLDT – Store Local Descriptor Table Register
 *  to register 0000 1111 : 0000 0000 : 11 000 reg
 *  to memory 0000 1111 : 0000 0000 : mod 000 r/m
 * 
 * SMSW – Store Machine Status Word
 *  to register 0000 1111 : 0000 0001 : 11 100 reg
 *  to memory 0000 1111 : 0000 0001 : mod 100 r/m
 * 
 * STC – Set Carry Flag 1111 1001
 * 
 * STD – Set Direction Flag 1111 1101
 * 
 * STI – Set Interrupt Flag 1111 1011
 * 
 * STOS/STOSB/STOSW/STOSD – Store String Data 1010 101w
 * 
 * STR – Store Task Register
 *  to register 0000 1111 : 0000 0000 : 11 001 reg
 *  to memory 0000 1111 : 0000 0000 : mod 001 r/m
 * 
 * SUB – Integer Subtraction
 *  register1 to register2 0010 100w : 11 reg1 reg2
 *  register2 to register1 0010 101w : 11 reg1 reg2
 *  memory to register 0010 101w : mod reg r/m
 *  register to memory 0010 100w : mod reg r/m
 *  immediate to register 1000 00sw : 11 101 reg : immediate data
 *  immediate to AL, AX, or EAX 0010 110w : immediate data
 *  immediate to memory 1000 00sw : mod 101 r/m : immediate data
 * 
 * TEST – Logical Compare
 *  register1 and register2 1000 010w : 11 reg1 reg2
 *  memory and register 1000 010w : mod reg r/m
 *  immediate and register 1111 011w : 11 000 reg : immediate data
 *  immediate and AL, AX, or EAX 1010 100w : immediate data
 *  immediate and memory 1111 011w : mod 000 r/m : immediate dat
 * 
 * Estas instrucciones no definidas son para futuro uso(algunos compiladores las usan para crear trampas/excepciones)
 *  UD0 – Undefined instruction 0000 1111 : 1111 1111 
 *  UD1 – Undefined instruction 0000 1111 : 0000 1011
 *  UD2 – Undefined instruction 0000 FFFF : 0000 1011
 * 
 * VERR – Verify a Segment for Reading
 *  register 0000 1111 : 0000 0000 : 11 100 reg
 *  memory 0000 1111 : 0000 0000 : mod 100 r/m
 * 
 * VERW – Verify a Segment for Writing
 *  register 0000 1111 : 0000 0000 : 11 101 reg
 *  memory 0000 1111 : 0000 0000 : mod 101 r/m
 * 
 * WAIT – Wait 1001 1011
 * 
 * WBINVD – Writeback and Invalidate Data Cache 0000 1111 : 0000 1001
 * 
 * WRMSR – Write to Model-Specific Register 0000 1111 : 0011 0000
 * 
 * XADD – Exchange and Add
 *  register1, register2 0000 1111 : 1100 000w : 11 reg2 reg1
 *  memory, reg 0000 1111 : 1100 000w : mod reg r/m
 * 
 * XCHG – Exchange Register/Memory with Register
 *  register1 with register2 1000 011w : 11 reg1 reg2
 *  AX or EAX with reg 1001 0 reg
 *  memory with reg 1000 011w : mod reg r/m
 * 
 * XLAT/XLATB – Table Look-up Translation 1101 0111
 * 
 * XOR – Logical Exclusive OR
 *  register1 to register2 0011 000w : 11 reg1 reg2
 *  register2 to register1 0011 001w : 11 reg1 reg2
 *  memory to register 0011 001w : mod reg r/m
 *  register to memory 0011 000w : mod reg r/m
 *  immediate to register 1000 00sw : 11 110 reg : immediate data
 *  immediate to AL, AX, or EAX 0011 010w : immediate data
 *  immediate to memory 1000 00sw : mod 110 r/m : immediate data
 * 
 * Prefix Bytes
 *  address size 0110 0111
 *  LOCK 1111 0000
 *  operand size 0110 0110
 *  CS segment override 0010 1110
 *  DS segment override 0011 1110
 *  ES segment override 0010 0110
 *  FS segment override 0110 0100
 *  GS segment override 0110 0101
 *  SS segment override 0011 0110
 * 
 * 
 * Instrucciones y formatos de proposito general para 64bits (pagina 2886):
 * 
 * Símbolo Aplicación
 *  S -> Si el valor de REX.W es 1, anula la presencia de 66H.
 *  w -> El valor del bit W en REX no tiene efecto.
 * 
 * ADC – ADD with Carry
 *  register1 to register2 0100 0R0B : 0001 000w : 11 reg1 reg2
 *  qwordregister1 to qwordregister2 0100 1R0B : 0001 0001 : 11 qwordreg1 qwordreg2
 *  register2 to register1 0100 0R0B : 0001 001w : 11 reg1 reg2
 *  qwordregister1 to qwordregister2 0100 1R0B : 0001 0011 : 11 qwordreg1 qwordreg2
 *  memory to register 0100 0RXB : 0001 001w : mod reg r/m
 *  memory to qwordregister 0100 1RXB : 0001 0011 : mod qwordreg r/m
 *  register to memory 0100 0RXB : 0001 000w : mod reg r/m
 *  qwordregister to memory 0100 1RXB : 0001 0001 : mod qwordreg r/m
 *  immediate to register 0100 000B : 1000 00sw : 11 010 reg : immediate
 *  immediate to qwordregister 0100 100B : 1000 0001 : 11 010 qwordreg : imm32
 *  immediate to qwordregister 0100 1R0B : 1000 0011 : 11 010 qwordreg : imm8
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * Ejemplo de codificacion de  instruccion:
 *  add ecx, eax (ejemplo medio bien, ver mas abajo otros ejemplos):
 * 
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 0  0  0  0  0  0 | 0 | 1 | 1  1 | 0  0  0 | 0  0  1 | == 0x01 0xc1 == add ecx, eax
 *      |  uso | opcode ( add )   | d | s |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 *      primer byte:
 *          - El campo "opcode" 0000 00 representa la instruccion ADD.
 *          - El campo "s" del opcode indica que se usara 32bits al poner 1 como valor.
 *          - El campo "d" del opcode, indica que estamos añadiendo el campo REG al campo R/M al poner 0 como valor.
 *      segundo byte:
 *          - El campo "mod" en este caso al poner 11 indica que lo que hay en R/M es un registro.
 *          - El campo "reg", en este caso junto al campo "d" del opcode, indica que el campo fuente es el registro eax.
 *          - El campo "r/m", en este caso junto al campo "d" del opcode indica que el registro de destino es ecx
 * 
 * 
 * add edx, [desplazamiento32]: aqui desplazamiento es representado mediante una direccion de 32bits x =>  0xww 0xxx 0xyy 0xzz. ww es el byte mas bajo y zz el mas alto (littel endian)
 * 
 *      |------|--------------------------|------|---------|---------|---------------------------------------|
 *      | bits | 0  0  0  0  0  0 | 1 | 1 | 0  0 | 0  1  1 | 1  0  1 |  desplazamiento 0xww 0xxx 0xyy 0xzz   | == 0x03 0x1d 0xww 0xxx 0xyy 0xzz == add edx, [ 0xww 0xxx 0xyy 0xzz ]
 *      |  uso | opcode ( add )   | d | s |  mod |   reg   |   R/M   | desplazamiento de 32bits en este caso |
 *      |------|--------------------------|------|---------|---------|---------------------------------------|
 *      primer byte:
 *          - El campo "opcode" 0000 00 representa la instruccion ADD.
 *          - El campo "s" del opcode indica que se usara 32bits al poner 1 como valor.
 *          - El campo "d" del opcode, indica que estamos añadiendo el campo r/m al campo reg al poner 1 como valor.
 *      segundo byte:
 *          - El campo "reg", en este caso junto al campo "d" del opcode, indica que el campo destino es el registro edx.
 *          - El campo "mod" con valor 00 junto al campo "r/m" 101 indica que se trata del modo de direccionamiento de solo desplazamiento
 *      tercer a sexto byte:
 *          - direccion de desplazamiento de 32bits en este caso.
 * 
 * 
 * add edi, [ebx]
 * 
 *      |------|--------------------------|------|---------|---------|---------------------------------------|
 *      | bits | 0  0  0  0  0  0 | 1 | 1 | 0  0 | 1  1  1 | 0  1  1 |  desplazamiento 0xww 0xxx 0xyy 0xzz   | == 0x03 0x1b == add edx, [ ebx ]
 *      |  uso | opcode ( add )   | d | s |  mod |   reg   |   R/M   | desplazamiento de 32bits en este caso |
 *      |------|--------------------------|------|---------|---------|---------------------------------------|
 *      primer byte:
 *          - El campo "opcode" 0000 00 representa la instruccion ADD.
 *          - El campo "s" del opcode indica que se usara 32bits al poner 1 como valor.
 *          - El campo "d" del opcode, indica que estamos añadiendo el campo r/m al campo reg al poner 1 como valor.
 *      segundo byte:
 *          - El campo "reg", en este caso junto al campo "d" del opcode, indica que el campo destino es el registro edi.
 *          - El campo "mod" en este caso indica que el desplazamiento es de 0 bytes.
 *          - El campo "r/m", en este caso el valor 011 indica el modo de uso de direccionamiento en [ebx]
 * 
 * 
 * add ebx, [ebp + desplazamiento32]: aqui desplazamiento es representado mediante una direccion de 32bits x =>  0xww 0xxx 0xyy 0xzz. ww es el byte mas bajo y zz el mas alto (littel endian)
 * 
 *      |------|--------------------------|------|---------|---------|---------------------------------------|
 *      | bits | 0  0  0  0  0  0 | 1 | 1 | 1  0 | 0  1  1 | 1  0  1 |  desplazamiento 0xww 0xxx 0xyy 0xzz   | == 0x03 0x9d 0xww 0xxx 0xyy 0xzz == add edx, [ 0xww 0xxx 0xyy 0xzz ]
 *      |  uso | opcode ( add )   | d | s |  mod |   reg   |   R/M   | desplazamiento de 32bits en este caso |
 *      |------|--------------------------|------|---------|---------|---------------------------------------|
 *      primer byte:
 *          - El campo "opcode" 0000 00 representa la instruccion ADD.
 *          - El campo "s" del opcode indica que se usara 32bits al poner 1 como valor.
 *          - El campo "d" del opcode, indica que estamos añadiendo el campo r/m al campo reg al poner 1 como valor.
 *      segundo byte:
 *          - El campo "reg", en este caso junto al campo "d" del opcode, indica que el campo destino es el registro ebx.
 *          - El campo "mod" con valor 10 indica que se usara un desplazamiento de 32bits
 *          - El campo "r/m", en este caso indica [ebp]
 *      tercer a sexto byte:
 *          - direccion de desplazamiento de 32bits en este caso.
 * 
 * 
 * add ecx, [ebx + edi * 4]
 * 
 *      |------|--------------------------|------|---------|---------|--------------------------------|
 *      | bits | 0  0  0  0  0  0 | 1 | 1 | 0  0 | 0  0  1 | 1  0  0 |  1  0  |  1  1  1  |  0  1  1  | == 0x03 0x0c 0xbb == add edx, [ebx + edi * 4]
 *      |  uso | opcode ( add )   | d | s |  mod |   reg   |   R/M   | SIB                 \    base  |     
 *      |------|--------------------------|------|---------|---------|--------------------------------|
 *      primer byte:
 *          - El campo "opcode" 0000 00 representa la instruccion ADD.
 *          - El campo "s" del opcode indica que se usara 32bits al poner 1 como valor.
 *          - El campo "d" del opcode, indica que estamos añadiendo el campo r/m al campo reg al poner 1 como valor.
 *      segundo byte:
 *          - El campo "reg", en este caso indica que el registro destino es el registro ecx.
 *          - El campo "mod" con valor 00 junto al campo "r/m" 100 indica que se trata del modo SIB
 *      tercer a sexto byte:
 *          - En este caso el campo "sib" que contiene un campo "base", indica que se usara el modo de indice escalado o SIB con EDI * 4, siendo 10 el *4 y 111 el registro edi
 *          - El campo "base" del campo "sib" indica que el registro base es ebx
 * 
 * lea eax, [ebx + edi * 8]
 * 
 *      |------|--------------------------|------|---------|---------|--------------------------------|
 *      | bits | 1  0  0  0  1  1 | 0 | 1 | 0  0 | 0  0  0 | 1  0  0 |  1  1  |  1  1  1  |  0  1  1  | == 0x8d 0x04 0xfb == lea eax, [ebx + edi * 8]
 *      |  uso | opcode ( lea )   | d | s |  mod |   reg   |   R/M   | SIB                 \    base  |     
 *      |------|--------------------------|------|---------|---------|--------------------------------|
 *      primer byte:
 *          - El campo "opcode" 1000 11 representa la instruccion LEA.
 *          - El campo "s" del opcode indica que se usara 32bits al poner 1 como valor.
 *          - El campo "d" del opcode, indica que estamos añadiendo el campo REG al campo R/M al poner 0 como valor.
 *      segundo byte:
 *          - El campo "reg", en este caso indica que el registro destino es el registro eax.
 *          - El campo "mod" con valor 00 junto al campo "r/m" 100 indica que se trata del modo SIB
 *      tercer a sexto byte:
 *          - En este caso el campo "sib" que contiene un campo "base", indica que se usara el modo de indice escalado o 
 *              SIB con EDI * 8, siendo 11 el *8 y 111 el registro edi
 *          - El campo "base" del campo "sib" indica que el registro base es ebx
 * 
 * jmp word ptr [0x1234]
 * 
 *      |------|---------------------------------------------|------|---------|---------|--------------------|
 *      | bits |    1    1    1    1    1    1       | 1 | 1 | 0  0 | 0  0  1 | 1  1  0 |        0x1234      | == 0xff 0x26 0x34 0x12 == jmp [0x1234]
 *      |  uso | opcode (cmp, push, call, inc, dec ) | d | s |  mod |   reg   |   R/M   | direccion a saltar |
 *      |------|---------------------------------------------|------|---------|---------|--------------------|
 *      primer byte:
 *          - El campo "opcode" 1111 11 representa la instruccion cmp, push, call, inc, dec.
 *          - El campo "s" del opcode indica que se usara 32bits al poner 1 como valor.
 *          - El campo "d" del opcode, indica que estamos añadiendo el campo REG al campo R/M al poner 0 como valor.
 *      segundo byte:
 *          - El campo "mod" con valor 00 junto al campo "r/m" 110 con 16 bits indica disp16, un desplazamiento de 16bits
 *          - El campo "reg", en este caso indica que el registro destino es el registro ecx.
 *      tercer a cuarto byte:
 *          - direccion de memoria a saltar
 * 
 * 
 * add eax, ecx 
 * 
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 0  0  0  0  0  0 | 0 | 1 | 1  1 | 0  0  1 | 0  0  0 | == 0x66 0x01 0xc8  == add eax, ecx
 *      |  uso | opcode ( add )   | d | s |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 *      primer byte:
 *          - 0x66 es un prefijo, recuerde que para cada instruccion ensamblador, habra un prefijo de instruccion
 *              opcional. Segun el manual de intel vol1, El prefijo operand-size override permite a un programa 
 *              cambiar entre tamaños de operando de 16 y 32 bits. Cualquiera de los dos tamaños puede ser el 
 *              predeterminado; el uso del prefijo selecciona el tamaño no predeterminado. en este caso el modo 
 *              no predeterminado seria 32bits, es por eso que decimos que sea este. No se puede cambiar de 16 o de 32 a 64 ni al reves 
 *              mediante este prefijo, se a de usar la correspondiente instruccion para esta tarea
 *      segundo byte:
 *          - El campo "opcode" 0000 00 representa la instruccion ADD.
 *          - El campo "s" del opcode indica que se usara 32bits al poner 1 como valor.
 *          - El campo "d" del opcode, indica que estamos añadiendo el campo REG al campo R/M al poner 0 como valor.
 *      tercer byte:
 *          - El campo "mod" en este caso al poner 11 indica que lo que hay en R/M es un registro.
 *          - El campo "reg", en este caso indica que el registro destino es el registro ecx.
 *          - El campo "r/m", en este caso junto al campo "mod" indica que el registro fuente es eax
 * 
 * jmp [eax*2 + ebx]
 * 
 *      |------|---------------------------------------------|------|---------|---------|--------------------------------|
 *      | bits |    1    1    1    1    1    1       | 1 | 1 | 0  0 | 1  0  0 | 1  0  0 |  0  1  |  0  0  0  |  0  1  1  | == 0x67 0xff 0x24 0x43 == jmp [eax * 2 + ebx]
 *      |  uso | opcode (cmp, push, call, inc, dec ) | d | s |  mod |   reg   |   R/M   | SIB                 \    base  |
 *      |------|---------------------------------------------|------|---------|---------|--------------------------------|
 *      primer byte:
 *          - 0x67 es un prefijo,  predefinido para el prefijo de anulación de tamaño de dirección. Su significado es que si una instrucción ejecuta un tamaño 
 *          de dirección predeterminado, por ejemplo, 16 bits, el uso del prefijo permite que
 *          la instrucción utilice un tamaño de dirección distinto al predeterminado, por 
 *          ejemplo, 32 bits en modo de 16. Como se supone que el binario es de 16 bits, 0x67 
 *          cambia la instrucción al modo de 32 bits.
 *      segundo byte:
 *          - El campo "opcode" 1111 11 representa la instruccion cmp, push, call, inc, dec.
 *          - El campo "s" del opcode indica que se usara 32bits al poner 1 como valor.
 *          - El campo "d" del opcode, indica que estamos añadiendo el campo REG al campo R/M al poner 0 como valor.
 *      tercer byte:
 *          - El campo "mod" con valor 00 junto al campo "r/m" 100 indica que se trata del modo SIB
 *          - El campo "reg", en este caso indica que el registro destino es el registro eax.
 *      cuarto a cuarto byte:
 *          - En este caso el campo "sib" que contiene un campo "base", indica que se usara el modo de indice escalado o 
 *              SIB con eax * 2, siendo 01 el *2 y 000 el registro eax
 *          - El campo "base" del campo "sib" indica que el registro base es ebx con 011
 * 
 * mov eax, dword ptr [ebx]
 * 
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 1  0  0  0  1  0 | 1 | 1 | 0  0 | 0  0  0 | 0  1  1 | == 0x67, 0x66, 0x8b, 0x03  == mov eax, dword ptr ds:[ebx]
 *      |  uso | opcode ( mov )   | d | s |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 *      primer byte:
 *          - 0x67 es un prefijo de Anulación del tamaño de la dirección, suponiendo que la instruccion se ejecute en modo de direccionamiento real(16bits)
 *          -       podemos cambiar al modo de direcionamiento protegido con esta prefijo.
 *      segundo byte:
 *          - 0x66 es un prefijo de Anulación del tamaño operativo. nos permite cambiar el tamño de los operandos de 16 bits a 32bits en este caso, aunque 
 *                  podria ser a la inversa
 *      tercer byte:
 *          - El campo "opcode" 1010 11 representa la instruccion mov.
 *          - El campo "s" del opcode indica que se usara 32bits al poner 1 como valor.
 *          - El campo "d" del opcode, indica que estamos añadiendo el campo REG al campo R/M al poner 0 como valor.
 *      cuarto a cuarto byte:
 *          - El campo "reg", en este caso junto al campo "d" del opcode, indica que el campo destino es el registro eax.
 *          - El campo "mod" en este caso indica que el desplazamiento es de 0 bytes.
 *          - El campo "r/m", en este caso el valor 011 indica el modo de uso de direccionamiento en [ebx]
 * 
 * add cl, al
 * 
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 0  0  0  0  0  0 | 0 | 0 | 1  1 | 0  0  0 | 0  0  1 | == 0x00 0xC1 (si d esta a 0) == add cl, al
 *      |  uso | opcode ( add )   | d | s |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 *      primer byte:
 *          - El campo "opcode" 0000 00 representa la instruccion ADD.
 *          - El campo "s" del opcode indica que se usara 8bits al poner 0 como valor.
 *          - El campo "d" del opcode, indica que estamos añadiendo el campo REG al campo R/M al poner 0 como valor,
 *              en otras palabras, que el campo fuente es el especificado en reg y el campo destino es r/m.
 *      segundo byte:
 *          - El campo "mod" en este caso al poner 11 indica que se usara el mododo de direccionamiento de registro.
 *          - El campo "reg", en este caso indica que el registro fuente es el registro al.
 *          - El campo "r/m", en este caso junto al campo "mod" indica que el registro destino es cl
 * 
 * add cl, al
 *      
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 0  0  0  0  0  0 | 1 | 0 | 1  1 | 0  0  1 | 0  0  0 | == 0x02 0xC8 (si d esta a 1) == add cl, al
 *      |  uso | opcode ( add )   | d | s |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 *      primer byte:
 *          - El campo "opcode" 0000 00 representa la instruccion ADD.
 *          - El campo "s" del opcode indica que se usara 8bits al poner 0 como valor.
 *          - El campo "d" del opcode, indica que estamos añadiendo el campo R/M al campo REG al poner 1 como valor,
 *              en otras palabras, que el campo fuente es el especificado en R/M y el campo destino es REG.
 *      segundo byte:
 *          - El campo "mod" en este caso al poner 11 indica que se usara el mododo de direccionamiento de registro.
 *          - El campo "reg", en este caso indica que el registro destino es el registro cl.
 *          - El campo "r/m", en este caso junto al campo "mod" indica que el registro fuente es al
 * 
 * add al, cl
 *      
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 0  0  0  0  0  0 | 0 | 0 | 1  1 | 0  0  1 | 0  0  0 | == 0x00, 0xc8 (si d esta a 0) == add al, cl
 *      |  uso | opcode ( add )   | d | s |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 *      primer byte:
 *          - El campo "opcode" 0000 00 representa la instruccion ADD.
 *          - El campo "s" del opcode indica que se usara 8bits al poner 0 como valor.
 *          - El campo "d" del opcode, indica que estamos añadiendo el campo REG al campo R/M al poner 0 como valor,
 *              en otras palabras, que el campo fuente es el especificado en reg y el campo destino es r/m.
 *      segundo byte:
 *          - El campo "mod" en este caso al poner 11 indica que se usara el mododo de direccionamiento de registro.
 *          - El campo "reg", en este caso indica que el registro fuente es el registro cl.
 *          - El campo "r/m", en este caso junto al campo "mod" indica que el registro destino es al
 * 
 * add al, cl
 *      
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 0  0  0  0  0  0 | 1 | 0 | 1  1 | 0  0  0 | 0  0  1 | == 0x02, 0xc1 (si d esta a 1) == add al, cl
 *      |  uso | opcode ( add )   | d | s |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 *      primer byte:
 *          - El campo "opcode" 0000 00 representa la instruccion ADD.
 *          - El campo "s" del opcode indica que se usara 8bits al poner 0 como valor.
 *          - El campo "d" del opcode, indica que estamos añadiendo el campo R/M al campo REG al poner 1 como valor,
 *              en otras palabras, que el campo fuente es el especificado en R/M y el campo destino es REG.
 *      segundo byte:
 *          - El campo "mod" en este caso al poner 11 indica que se usara el mododo de direccionamiento de registro.
 *          - El campo "reg", en este caso indica que el registro fuente es el registro al.
 *          - El campo "r/m", en este caso junto al campo "mod" indica que el registro destino es cl.
 * 
 * Tabla A6 de manual de intel (pagina 2856)
 * Algunas instrucciones de 1 o 2 bytes de opcode usan extensiones de opcode, donde el byte modR/M codifica la instruccion extendida.
 * por ejemplo, un opcode que usa 0xFE, puede ser INC o DEC dependiendo del campo reg, si el campo reg es 001, es DEC, si es 000, es INC:
 * 
 * inc byte ptr [eax]
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 1  1  1  1  1  1 | 1 | 0 | 0  0 | 0  0  0 | 0  0  0 | == 0xFE 0x00 == inc byte ptr [eax] (en 32bits)
 *      |  uso | opcode( dec/inc )| d | s |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 * 
 * inc byte ptr [ecx]
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 1  1  1  1  1  1 | 1 | 0 | 0  0 | 0  0  0 | 0  0  1 | == 0xFE 0x01 == inc byte ptr [ecx] (en 32bits)
 *      |  uso | opcode( dec/inc )| d | s |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 * 
 * inc byte ptr [edx]
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 1  1  1  1  1  1 | 1 | 0 | 0  0 | 0  0  0 | 0  1  0 | == 0xFE 0x02 == inc byte ptr [edx] (en 32bits)
 *      |  uso | opcode( dec/inc )| d | s |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 * 
 * inc byte ptr [ebx]
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 1  1  1  1  1  1 | 1 | 0 | 0  0 | 0  0  0 | 0  1  1 | == 0xFE 0x03 == inc byte ptr [ebx] (en 32bits)
 *      |  uso | opcode( dec/inc )| d | s |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 * 
 * inc byte ptr [esi + edi*8]
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 1  1  1  1  1  1 | 1 | 0 | 0  0 | 0  0  0 | 1  0  0 | == 0xFE 0x04 0xFE == inc byte ptr [esi + edi*8] (en 32bits)
 *      |  uso | opcode( dec/inc )| d | s |  mod |   reg   |   R/M   | (Escribir SIB)
 *      |------|--------------------------|------|---------|---------|
 * 
 * inc byte ptr [0x7fe06fe]
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 1  1  1  1  1  1 | 1 | 0 | 0  0 | 0  0  0 | 1  0  1 | == 0xFE 0x05 0xFE 0x06 0xFE 0x07 == inc byte ptr [0x7fe06fe] (en 32bits)
 *      |  uso | opcode( dec/inc )| d | s |  mod |   reg   |   R/M   | (Escribir desplazamiento 32bits)
 *      |------|--------------------------|------|---------|---------|
 * 
 * inc byte ptr [esi]
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 1  1  1  1  1  1 | 1 | 0 | 0  0 | 0  0  0 | 1  1  0 | == 0xFE 0x06 == inc byte ptr [edi] (en 32bits)
 *      |  uso | opcode( dec/inc )| d | s |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 * 
 * inc byte ptr [edi]
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 1  1  1  1  1  1 | 1 | 0 | 0  0 | 0  0  0 | 1  1  1 | == 0xFE 0x07 == inc byte ptr [edi] (en 32bits)
 *      |  uso | opcode( dec/inc )| d | s |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 * 
 * dec byte ptr [eax]
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 1  1  1  1  1  1 | 1 | 0 | 0  0 | 0  0  1 | 0  0  0 | == 0xFE 0x08 == dec byte ptr [eax] (en 32bits)
 *      |  uso | opcode( dec/inc )| d | s |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 * 
 * dec byte ptr [ecx]
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 1  1  1  1  1  1 | 1 | 0 | 0  0 | 0  0  1 | 0  0  1 | == 0xFE 0x09 == dec byte ptr [ecx] (en 32bits)
 *      |  uso | opcode( dec/inc )| d | s |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 * 
 * dec byte ptr [bx + di]
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 1  1  1  1  1  1 | 1 | 0 | 0  0 | 0  0  1 | 0  0  1 | == 0xFE 0x09 == dec byte ptr [bx + di] (en 16bits)
 *      |  uso | opcode( dec/inc )| d | s |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 * 
 * inc byte ptr [bx]
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 1  1  1  1  1  1 | 1 | 0 | 0  0 | 0  0  0 | 1  1  1 | == 0xFE 0x07 == inc byte ptr [bx] (en 16bits)
 *      |  uso | opcode( dec/inc )| d | s |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 * 
 * las INC y DEC que hacen uso del opcode 0xFF, usan el tamaño palabra de la cpu para 32 y 16bits, por ejemplo FF 01 es inc word ptr [bx + di] para 16bits, word == 16bits
 * mientras en 32bits es inc dword ptr [ecx], donde se usa dword(32bits). Como excepcion en 64bits se usara dword inc dword ptr [rcx] == FF 01
 * Ejemplo usando el opcode 0xFF, mirar tabla A-6 para ver las extensiones. INC(000), DEC(001), near CALL(010), far CALL(011), near JMP(100), far JMP(101) PUSH(110)
 * 
 * inc dword ptr [ecx]
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 1  1  1  1  1  1 | 1 | 0 | 0  0 | 0  0  0 | 0  0  1 | == 0xFF 0x01 == inc dword ptr [ecx] (en 32bits)
 *      |  uso | opcode           | d | s |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 * 
 * dec dword ptr [ecx]
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 1  1  1  1  1  1 | 1 | 0 | 0  0 | 0  0  1 | 0  0  1 | == 0xFF 0x09 == dec dword ptr [ecx] (en 32bits)
 *      |  uso | opcode           | d | s |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 * 
 * call dword ptr [ecx]
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 1  1  1  1  1  1 | 1 | 0 | 0  0 | 0  1  0 | 0  0  1 | == 0xFF 0x11 == call dword ptr [ecx] (en 32bits)
 *      |  uso | opcode           | d | s |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 * 
 * lcall [ecx] o tmb llamado en algunos ensambladores como call far [ecx]
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 1  1  1  1  1  1 | 1 | 0 | 0  0 | 0  1  1 | 0  0  1 | == 0xFF 0x19 == lcall [ecx] (en 32bits)
 *      |  uso | opcode           | d | s |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 * 
 * jmp dword ptr [ecx]
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 1  1  1  1  1  1 | 1 | 0 | 0  0 | 1  0  0 | 0  0  1 | == 0xFF 0x21 == jmp dword ptr [ecx] (en 32bits)
 *      |  uso | opcode           | d | s |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 * 
 * ljmp [ecx] o tmb llamado en algunos ensambladores como jmp far [ecx]
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 1  1  1  1  1  1 | 1 | 0 | 0  0 | 1  0  1 | 0  0  1 | == 0xFF 0x29 == ljmp [ecx] (en 32bits)
 *      |  uso | opcode           | d | s |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 * 
 * push dword ptr [ecx]
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 1  1  1  1  1  1 | 1 | 0 | 0  0 | 1  1  0 | 0  0  1 | == 0xFF 0x31 == push dword ptr [ecx] (en 32bits)
 *      |  uso | opcode           | d | s |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
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
 * 
 * 
 * ADC – ADD with Carry ->  0001 00dw : mod reg r/m (de memoria a registro y de registro a memoria)
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 0  0  0  1  0  0 | 1 | 0 | 0  0 | 1  1  0 | 0  0  1 | == 0x12 0x31 == adc dh, byte ptr [bx + di]
 *      |  uso | opcode           | d | w |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 * 
 * ADC – ADD with Carry ->  0001 001w : mod reg r/m
 *      |------|--------------------------|------|---------|---------|--------------------------|
 *      | bits | 0  0  0  1  0  0 | 1 | 0 | 0  1 | 1  1  0 | 0  0  1 |  0  0  0  0  1  0  1  0  | == 0x12 0x71 0x0a== adc dh, byte ptr [bx + di + 0xa]
 *      |  uso | opcode           | d | w |  mod |   reg   |   R/M   |   desplazamiento 8 bits  |
 *      |------|--------------------------|------|---------|---------|---------------------------
 *      
 *      primer byte:
 *          - El campo "opcode" 0001 0010 representa la instruccion ADC.
 *          - El campo "d"(direccion) del opcode, indica que estamos añadiendo el campo R/M al campo REG al poner 1 como valor,
 *              en otras palabras, que el campo fuente es el especificado en R/M y el campo destino es REG.
 *                  d = 1: REG <- MOD R/M, REG es el destino
 *          - El campo "w" en 0 especifica registro de 8bits, de eso dh, [bx + di] depende del tamaño de palabra de la cpu,
 *              en el caso de que este activo el bit, la instruccion pasa a "adc si, word ptr [bx + di + 0xa]"
 *      segundo byte:
 *          - El campo "mod" en 01 especifica un [registro + desp8(desplazamiento de 8bits)].
 *          - El campo "R/M" del opcode indica el registro y desplazamiento a usar, en caso de descodificar ls instruccion en 32bits:
 *                  | MOD | R/M | Addressing Mode |
 *                  |  01 | 001 | [ ecx + disp8 ] |
 *              En caso de 16bits es:
 *                  | MOD | R/M |  Addressing Mode  |
 *                  |  01 | 001 | [bx + di + disp8] |
 *                  
 *          - El campo "reg" del opcode, en este caso especifica donde almacenar el valor, en este caso "dh" por ser "w" = 0,
 *              En caso de que sea W = 1, y la instruccion se descodifique para 16bits, el registro es "si", o en caso
 *              de ser 32bits, sera "si"
 *      tercer byte:
 *          - desplazamiento de 8bits a realizar, 0xa en este caso
 * 
 * 
 * ADC – ADD with Carry ->  0001 001w : mod reg r/m
 * 
 *  adc ax/eax, valor: (para 32bits)
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 0  0  0  0  0  0 | 0 | 0 | 1  0 | 1  1  0 | 0  0  1 | == 15 01 01 01 01 == adc eax, 0x1010101
 *      |  uso | opcode               | w |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 *  adc ax/eax, valor: (para 16bits)
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 0  0  0  0  0  0 | 0 | 0 | 1  0 | 1  1  0 | 0  0  1 | == 15 01 01 == adc ax, 0x101
 *      |  uso | opcode               | w |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 * 
 *  adc al, valor:
 *      |------|--------------------------|------|---------|---------|
 *      | bits | 0  0  0  0  0  0 | 0 | 0 | 0  0 | 0  0  0 | 0  0  1 | == 14 01 == adc al, 1
 *      |  uso | opcode               | w |  mod |   reg   |   R/M   |
 *      |------|--------------------------|------|---------|---------|
 * 
 * adc ch, [ebx + edi*4]
 *                                     mod = 00/01/10    rm = 100 == SIB
 *      |------|--------------------------|------|---------|---------|--------------------------------|
 *      | bits | 0  0  0  1  0  0 | 1 | 0 | 0  0 | 0  0  1 | 1  0  0 |  1  0  |  1  1  1  |  0  1  1  | == 0x12 0x2c 0xbb == adc ch, [ebx + edi*4]
 *      |  uso | opcode ( add )   | d | s |  mod |   reg   |   R/M   | SIB                 \    base  |     
 *      |------|--------------------------|------|---------|---------|--------------------------------|
 *      primer byte:
 *          - El campo "opcode" 0001 00 representa la instruccion ADC.
 *          - El campo "s" del opcode indica que se usara el registro de 8bits especificado por reg al poner 0 como valor.
 *          - El campo "d" del opcode, indica el valor de la memoria se guardara en el registro ch
 *      segundo byte:
 *          - El campo "reg", en este caso indica que el registro destino es el registro ch.
 *          - El campo "mod" con valor 00 junto al campo "r/m" 100 indica que se trata del modo SIB:
 *				00 100 SIB
 *      		01 100 SIB + disp8
 *      		10 100 SIB + disp32
 *      tercer byte:
 *          - En este caso el campo "sib" que contiene un campo "base", indica que se usara el modo de indice escalado o SIB con EDI * 4, siendo 10 el *4 y 111 el registro edi
 *          - El campo "base" del campo "sib" indica que el registro base es ebx
 * 
 * adc [ebx + edi*4], ch
 *                                     mod = 00/01/10    rm = 100 == SIB
 *      |------|--------------------------|------|---------|---------|--------------------------------|
 *      | bits | 0  0  0  1  0  0 | 0 | 0 | 0  0 | 0  0  1 | 1  0  0 |  1  0  |  1  1  1  |  0  1  1  | == 0x10 0x2c 0xbb == adc [ebx + edi*4], ch
 *      |  uso | opcode ( add )   | d | s |  mod |   reg   |   R/M   | SIB                 \    base  |     
 *      |------|--------------------------|------|---------|---------|--------------------------------|
 *      primer byte:
 *          - El campo "opcode" 0001 00 representa la instruccion ADC.
 *          - El campo "s" del opcode indica que se usara el registro de 8bits especificado por reg al poner 0 como valor.
 *          - El campo "d" del opcode, en valor 0 indica que el valor de ch sera almacenado en un lugar de la memoria
 *      segundo byte:
 *          - El campo "reg", en este caso indica que el registro destino es el registro ch.
 *          - El campo "mod" con valor 00 junto al campo "r/m" 100 indica que se trata del modo SIB:
 *				00 100 SIB
 *      		01 100 SIB + disp8
 *      		10 100 SIB + disp32c
 *      tercer byte:
 *          - En este caso el campo "sib" que contiene un campo "base", indica que se usara el modo de indice escalado o SIB con EDI * 4, siendo 10 el *4 y 111 el registro edi
 *          - El campo "base" del campo "sib" indica que el registro base es ebx
 * 
 * 
 * adc [ebx + edi*4 + 0x12], ch
 *                                     mod = 00/01/10    rm = 100 == SIB
 *      |------|--------------------------|------|---------|---------|--------------------------------|-----------------------------------|
 *      | bits | 0  0  0  1  0  0 | 0 | 0 | 0  1 | 0  0  1 | 1  0  0 |  1  0  |  1  1  1  |  0  1  1  |  desplazamiento  == 0xzz          | == 0x10 0x6c 0xbb 0x12 == adc [ebx + edi*4 + 0x12], ch
 *      |  uso | opcode ( add )   | d | s |  mod |   reg   |   R/M   | SIB                 \    base  | desplazamiento  de 8bits/1 byte   |
 *      |------|--------------------------|------|---------|---------|--------------------------------|-----------------------------------|
 *      primer byte:
 *          - El campo "opcode" 0001 00 representa la instruccion ADC.
 *          - El campo "s" del opcode indica que se usara el registro de 8bits especificado por reg al poner 0 como valor.
 *          - El campo "d" del opcode, en valor 0 indica que el valor de ch sera almacenado en un lugar de la memoria
 *      segundo byte:
 *          - El campo "reg", en este caso indica que el registro destino es el registro ch.
 *          - El campo "mod" con valor 01 junto al campo "r/m" 100 indica que se trata del modo SIB mas 8bits de desplazamiento:
 *				00 100 SIB
 *      		01 100 SIB + disp8
 *      		10 100 SIB + disp32
 *      tercer byte:
 *          - En este caso el campo "sib" que contiene un campo "base", indica que se usara el modo de indice escalado o SIB con EDI * 4, siendo 10 el *4 y 111 el registro edi
 *          - El campo "base" del campo "sib" indica que el registro base es ebx
 *      cuarto byte:
 *          - En este caso el campo es el desplazamiento de 8bits a summar, este campo es un valor signed, eso quiere decir que el valor
 *            especificado puede restar o sumar. (Complemento a 2) -> -1 == 0xff, -2 = 0xfe, ...
 *            valor maximo a poder sumarse = 127, numero maximos a restarse = -128, rango de signed char 0 int8_t == (127, -128)
 * 
 * adc [ebx + edi*4 + 0x12345678], ch
 *                                     mod = 00/01/10    rm = 100 == SIB
 *      |------|--------------------------|------|---------|---------|--------------------------------|-----------------------------------------|
 *      | bits | 0  0  0  1  0  0 | 0 | 0 | 1  0 | 0  0  1 | 1  0  0 |  1  0  |  1  1  1  |  0  1  1  |  desplazamiento  == 0xww 0xxx 0xyy 0xzz | == 0x10 0xAc 0xbb 0x78 0x56 0x34 0x12 == adc [ebx + edi*4 + 0x12345678], ch
 *      |  uso | opcode ( add )   | d | s |  mod |   reg   |   R/M   | SIB                 \    base  |    desplazamiento  de 32bits/4 bytes    |
 *      |------|--------------------------|------|---------|---------|--------------------------------|-----------------------------------------|
 *      primer byte:
 *          - El campo "opcode" 0001 00 representa la instruccion ADC.
 *          - El campo "s" del opcode indica que se usara el registro de 8bits especificado por reg al poner 0 como valor.
 *          - El campo "d" del opcode, en valor 0 indica que el valor de ch sera almacenado en un lugar de la memoria
 *      segundo byte:
 *          - El campo "reg", en este caso indica que el registro destino es el registro ch.
 *          - El campo "mod" con valor 10 junto al campo "r/m" 100 indica que se trata del modo SIB mas 32bits de desplazamiento:
 *				00 100 SIB
 *      		01 100 SIB + disp8
 *      		10 100 SIB + disp32
 *      tercer byte:
 *          - En este caso el campo "sib" que contiene un campo "base", indica que se usara el modo de indice escalado o SIB con EDI * 4, siendo 10 el *4 y 111 el registro edi
 *          - El campo "base" del campo "sib" indica que el registro base es ebx
 *      cuarto byte a septimo byte:
 *          - En este caso el campo es el desplazamiento de 32bits a summar, este campo es un valor signed, eso quiere decir que el valor
 *            especificado puede restar o sumar. (Complemento a 2) -> -1 == 0xffffffff, -2 = 0xfffffffe, ...
 *            valor maximo a poder sumarse = 2.147.483.647, numero maximos a restarse = -2.147.483.648, rango de int32_t == (2.147.483.647, -2.147.483.648)
 * 
 * 
 *    * Ejemplo de SIB con base = 101 (EBP):
 *       Solo puede usarse EBP como base cuando se use mod 10 o 01
 *	     	12 ac bd 11 22 11 22 == adc ch, [ebp + edi*4 + 0x22112211] (mod = 10)
 *       	12 6c bd 11 22 11 22 == adc ch, [ebp + edi*4 + 0x22112211] (mod = 01)
 *
 *		 En caso de ser mod 00 no puede añadirse el registro EBP como base, quedando asi:
 *	     	12 2c bd 11 22 11 22 == adc ch, [edi*4 + 0x22112211]       (mod = 00)
 *		Se recuerda tambien que cualquier instruccion con index = 100 sera una instruccion ilegal.
 *
 */