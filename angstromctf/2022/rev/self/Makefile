CC ?= gcc
CFLAGS := -std=c11 -Wall -Wextra -O3 $(CFLAGS)
LDFLAGS := -Wl,-s
NASMFLAGS := 
HEADERS =
BUILD=build
BINS=bin
OBJ=obj
SRC=src
PROG=prog


RELEASE=RELEASE

ifeq ($(RELEASE),DEBUG)
	CFLAGS += -D$(RELEASE)
	NASMFLAGS += -d $(RELEASE)
endif

default: mkdirs patched_prog.bin gen prog.bin chall

mkdirs:
	mkdir -p $(BUILD)/$(BINS)
	mkdir -p $(BUILD)/$(OBJ)

chall: chall.o bin.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BUILD)/$(BINS)/$@ $(addprefix $(BUILD)/$(OBJ)/,$^)

prog.bin: $(SRC)/$(PROG)/prog.s $(SRC)/$(PROG)/assembler.mac
	nasm $(NASMFLAGS) -o $(BUILD)/$(OBJ)/$@ $<

patched_prog.bin: $(SRC)/$(PROG)/load.s $(SRC)/$(PROG)/assembler.mac
	nasm $(NASMFLAGS) -o $(SRC)/$(PROG)/$@ $<

%.o: $(SRC)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $(BUILD)/$(OBJ)/$@ $<

%.o: $(SRC)/%.s prog.bin
	nasm $(NASMFLAGS) -f elf64 -o $(BUILD)/$(OBJ)/$@ $<

gen: $(SRC)/$(PROG)/flag_gen.py
	python3 $<
