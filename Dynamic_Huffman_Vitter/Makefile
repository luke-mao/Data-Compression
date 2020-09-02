CC=gcc
CFLAGS=-Wall -g -c
LIBS=tree.o list.o dictionary.o compress.o decompress.o
BINS=a

all : $(LIBS) $(BINS)

a 					: main.c $(LIBS)
						$(CC) -o a main.c $(LIBS)

compress.o			: compress.c tree.o list.o dictionary.o
decompress.o		: decompress.c tree.o list.o dictionary.o
dictionary.o		: dictionary.c tree.o list.o
list.o				: list.c tree.o
tree.o				: tree.c

clean : 
	rm -f $(BINS) $(LIBS) *.vitter