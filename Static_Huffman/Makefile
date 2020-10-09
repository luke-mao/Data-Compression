CC=gcc
LIBS=util.o file.o frequency_table.o tree.o priority_queue.o codeword.o compress.o stack.o decompress.o
BINS=a

all : $(LIBS) $(BINS)

a 					: main.c $(LIBS)
						$(CC) main.c $(LIBS) -Wall -o  a 
decompress.o		: decompress.c util.o file.o tree.o stack.o
compress.o 			: compress.c util.o file.o frequency_table.o tree.o priority_queue.o codeword.o
codeword.o			: codeword.c util.o file.o frequency_table.o tree.o priority_queue.o
stack.o				: stack.c tree.o util.o
priority_queue.o	: priority_queue.c tree.o util.o 
tree.o				: tree.c 
frequency_table.o	: frequency_table.c util.o
file.o				: file.c util.o
util.o				: util.c

clean : 
	rm -f $(BINS) $(LIBS) *.huff dehuff.*
