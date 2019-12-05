ASM = nasm
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -Wshadow
ASMFLAGS = -f elf64
DEBUG_FLAGS   = -O0 -DDEBUG -g
RELEASE_FLAGS = -O3 -DNDEBUG
OBJ = main.o threadf.o
TARGET_PROGRAM = testprog

all: CFLAGS += $(RELEASE_FLAGS)
all: $(TARGET_PROGRAM)

debug: ASMFLAGS += -F dwarf -g
debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET_PROGRAM)

$(TARGET_PROGRAM): $(OBJ)
	$(CC) -o $(TARGET_PROGRAM) $(OBJ) -lpthread

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

threadf.o: threadf.asm
	$(ASM) $(ASMFLAGS) -o threadf.o threadf.asm

clean:
	@echo Cleaning up...
	@rm -f *.o
	@rm -f $(TARGET_PROGRAM)
	@echo Done.
