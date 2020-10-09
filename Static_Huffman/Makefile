CC=gcc
LIBS=util.o file.o frequency_table.o tree.o priority_queue.o codeword.o compress.o
BINS=a

all : $(LIBS) $(BINS)

a 					: main.c $(LIBS)
						$(CC) main.c $(LIBS) -Wall -o  a 

compress.o 			: compress.c util.o file.o frequency_table.o tree.o priority_queue.o codeword.o
codeword.o			: codeword.c util.o file.o frequency_table.o tree.o priority_queue.o
priority_queue.o	: priority_queue.c tree.o util.o 
tree.o				: tree.c 
frequency_table.o	: frequency_table.c util.o
file.o				: file.c util.o
util.o				: util.c

clean : 
	rm -f $(BINS) $(LIBS) *.huff dehuff.*
