CC       := gcc
CFLAGS   := -flto-compression-level=19 -Wall -march=native -fvisibility-inlines-hidden -fvisibility=hidden -O3
OBJDIR   := file_objects
OBJSTRIP := --localize-hidden --strip-unneeded 
EXE      := code.exe
D        := -D DEBUG_ENABLE

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

create:
	mkdir $(file_objects)

clean:
	del code.exe

.PHONY: clean
