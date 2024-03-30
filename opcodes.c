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
 *      0xF3    Prefijo REP (utilizado sólo con instrucciones de string)
 *      0xF3    Prefijo REPE/REPZ (sólo se utiliza con instrucciones de string)
 *      0xF2    Prefijo REPNE/REPNZ (utilizado sólo con instrucciones de cadena)
 *      0xF0    Prefijo LOCK
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
 *      0x66    Anulación del tamaño operativo
 *      0x67    Anulación del tamaño de la dirección
 * 
 * los opcodes 1000 00 indican que es una instruccion inmediata (superior a 0x80 = 1000 00)
 * 
 * 
 * fuente: https://www-user.tu-chemnitz.de/~heha/hsn/chm/x86.chm/x86.htm
 * Si s = 0, los operandos son registros de 8 bits y posiciones de memoria.
 * Si s = 1, los operandos son de 16 bits o de 32 bits:
 * En sistemas operativos de 32 bits, si s = 1, los operandos son por defecto de 32 bits.
 * Para especificar un operando de 16 bits (en Windows o Linux) debe 
 * insertar un byte especial de prefijo de tamaño de operando delante de la instrucción 
 * (ejemplo de esto más adelante).
 * 
 * Muchas instrucciones tienen el campo d (dirección) en su opcode para elegir el papel del operando REG:
 * Si d=0, REG es la fuente, MOD R/M <- REG.
 * Si d=1, REG es el destino, REG <- MOD R/M.
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
 * Valores de REG:
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
 * Formas con el Byte ModR/M para 32bits y 16bits:
 * |--------------------------------------------------------------------------------------------------------------|
 * | mod |        00        |                 01              |                  10               |       11      |
 * | rm  | 16bits  | 32bits |      16bits     |    32bits     |      16bits      |      32bits    |   r/m // REB  |
 * |--------------------------------------------------------------------------------------------------------------|
 * | 000 | [bx+si] | [eax]  | [bx+si] + disp8 | [eax] + disp8 | [bx+si] + disp16 | [eax] + disp32 | al / ax / eax |
 * | 001 | [bx+di] | [ecx]  | [bx+di] + disp8 | [ecx] + disp8 | [bx+di] + disp16 | [ecx] + disp32 | cl / cx / ecx |
 * | 010 | [bp+si] | [edx]  | [bp+si] + disp8 | [edx] + disp8 | [bp+si] + disp16 | [edx] + disp32 | dl / dx / edx |
 * | 011 | [bp+si] | [ebx]  | [bp+di] + disp8 | [ebx] + disp8 | [bp+di] + disp16 | [ebx] + disp32 | bl / bx / ebx |
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
 * Ejemplo de codificacion de  instruccion:
 * 
 * 
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
 * 
 */