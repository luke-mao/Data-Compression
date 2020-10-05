CC=gcc
CFLAGS=-Wall -g -c
LIBS=tree.o list.o file.o dictionary.o update.o compress.o
BINS=a

all : $(LIBS) $(BINS)

a 					: main.c $(LIBS)
						$(CC) -o a main.c $(LIBS)

compress.o			: compress.c tree.o list.o dictionary.o file.o
update.o 			: update.c tree.o list.o dictionary.o
dictionary.o		: dictionary.c tree.o list.o
file.o				: file.c list.o tree.o
list.o				: list.c tree.o
tree.o				: tree.c

clean : 
	rm -f $(BINS) $(LIBS) *.v