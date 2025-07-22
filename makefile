# Compilation macros
 CC = gcc
 CFLAGS = -ansi -Wall -pedantic -g # Flags
 OUT_P = ./OutFiles/
 GBL = ./Global/
 DATA = $(GBL)/Data/
 PRE_PCSR = ./Preproccessor/
 ERR_H = ./ErrorHandling/
 FIRST_PASS = ./first_pass/
 GLOBAL_DEPS = $(GBL)defines.h # Dependencies for everything
 EXE_DEPS = assembler.o  util.o tables.o mcro_expan.o Errors.o structures.o FileHandler.o main_pass.o extern_and_entry.o # Deps for exe

 ## Executable
assembler: $(EXE_DEPS) $(GLOBAL_DEPS)
	$(CC) -g $(EXE_DEPS) $(CFLAGS) -o $@

assembler.o:  $(OUT_P) $(GLOBAL_DEPS)
	$(CC) -c $(GBL)assembler.c $(CFLAGS) -o $@

mcro_expan.o: $(OUT_P) $(GLOBAL_DEPS)
	$(CC) -c $(PRE_PCSR)mcro_expan.c $(CFLAGS) -o $@

tables.o: $(OUT_P) $(GLOBAL_DEPS)
	$(CC) -c $(DATA)tables.c $(CFLAGS) -o $@

util.o: $(OUT_P) $(GLOBAL_DEPS)
	$(CC) -c $(GBL)util.c $(CFLAGS) -o $@

Errors.o: $(OUT_P) $(GLOBAL_DEPS)
	$(CC) -c $(ERR_H)Errors.c $(CFLAGS) -o $@

structures.o: $(OUT_P) $(GLOBAL_DEPS)
	$(CC) -c $(DATA)structures.c $(CFLAGS) -o $@

FileHandler.o: $(OUT_P) $(GLOBAL_DEPS)
	$(CC) -c ./IO/FileHandler.c $(CFLAGS) -o $@

main_pass.o: $(OUT_P) $(GLOBAL_DEPS)
	$(CC) -c $(FIRST_PASS)main_pass.c $(CFLAGS) -o $@
	
extern_and_entry.o: $(OUT_P) $(GLOBAL_DEPS)
	$(CC) -c $(FIRST_PASS)/instruction_handle/extern_and_entry.c $(CFLAGS) -o $@

clean:
	rm -rf assembler *.o *.am *.ob *.ent *.ext