CC = gcc
CFLAGS = -g -Wall -Wshadow 
DEPS = token.h character.h testTree.h scanner.h parser.h statSem.h identifierStack.h generator.h testTree.c scanner.c parser.c statSem.c identifierStack.c generator.c p4.c
OBJ = testTree.o scanner.o parser.o statSem.o identifierStack.o generator.o p4.o
LDFLAGS = -static
OUTPUT = genCode

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<
genCode: $(OBJ)
	$(CC)  $(CFLAGS) -o $@ $^

.PHONY: all clean
clean:
	/bin/rm -f $(OUTPUT) *.o *.asm

