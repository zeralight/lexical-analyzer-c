Flags=-Wall -g -std=c99
LinkingInfo=-L/home/ff/cs61c/bin/static #-l61Ccc -lexer

OBJS = main.o \
	   parser.o \
	   parser-helpers.o \
	   parser-errors.o \
	   ast.o \
	   ast-print.o \
	   tokenizer.o \
	   string-helpers.o \
	   tokens.o \
	   tokenizer-errors.o \
	   utils.o \
	   preprocessor.o

61Ccc : $(OBJS) 
	gcc $(Flags) -o 61Ccc $(OBJS) $(LinkingInfo)

main.o : main.c tokens.h tokenizer.h parser.h parser-errors.h preprocessor.h ast.h ast-print.h
	gcc $(Flags) -c main.c

parser.o: parser.c parser.h ast.h tokens.h
	gcc $(Flags) -c parser.c

parser-helpers.o: parser-helpers.c parser-helpers.h ast.h tokens.h
	gcc $(Flags) -c parser-helpers.c

parser-errors.o: parser-errors.c parser-errors.h ast.h
	gcc $(Flags) -c parser-errors.c

ast.o: ast.c ast.h
	gcc $(Flags) -c ast.c

ast-print.o: ast-print.c ast-print.h ast.h
	gcc $(Flags) -c ast-print.c

%.o: %.c
	gcc $(Flags) -c $<

clean:
	rm -f *.o
	rm -f 61Ccc

.PHONY: clean
