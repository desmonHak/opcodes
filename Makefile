CC        := gcc
CFLAGS    := -flto-compression-level=19 -Wall -march=native -fvisibility-inlines-hidden -fvisibility=hidden -O3
OBJDIR    := file_objects
OBJSTRIP  := --localize-hidden --strip-unneeded 
EXE       := code.exe
D         := -D DEBUG_ENABLE
FLAG_WIN7 := -D __DISABLE_COLORS_FORE_BACK_GROUND__

# usar la flag __DISABLE_COLORS_FORE_BACK_GROUND__ para compilar en win 7 o plataformas donde alguna secuencia ANSI
# de foreground y background de problemas
# objcopy $(OBJSTRIP) opcodes_prefix.o

.PHONY: all

all:
	@cls
	$(CC) code.c $(CFLAGS) -o $(EXE) 
	
compile_debug:
	@cls
	$(CC) code.c $(D) $(CFLAGS) -o $(EXE) 

compile_hidden: $(EJS)
	@cls
	$(CC) -Wl,--strip-all code.c $(CFLAGS) -o $(EXE) 

all_dc:
	@cls
	$(CC) code.c $(FLAG_WIN7) $(CFLAGS) -o $(EXE) 

compile_debug_dc:
	@cls
	$(CC) code.c $(FLAG_WIN7) $(D) $(CFLAGS) -o $(EXE) 

clean:
	del code.exe

.PHONY: clean
