CC=gcc
LIBS=tree.o list.o file.o dictionary.o update.o compress.o decompress.o
BINS=vitter

all : $(LIBS) $(BINS)

vitter 				: main.c $(LIBS)
						$(CC) main.c $(LIBS) -lm -o vitter
decompress.o		: decompress.c tree.o list.o dictionary.o file.o update.o
						$(CC) -c -o decompress.o decompress.c -lm
compress.o			: compress.c tree.o list.o dictionary.o file.o update.o
update.o 			: update.c tree.o list.o dictionary.o
dictionary.o		: dictionary.c tree.o list.o
file.o				: file.c list.o tree.o
list.o				: list.c tree.o
tree.o				: tree.c

clean : 
	rm -f $(BINS) $(LIBS) *.v deVitter_* testfile/deVitter_* testfile/*.v
