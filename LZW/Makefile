CC=gcc
CFLAGS=-Wall -g -c
LIBS=file.o util.o data_structure.o
BINS=compress decompress

all : $(LIBS) $(BINS)

compress 			: compress.c $(LIBS)
						$(CC) -o compress compress.c $(LIBS)
decompress			: decompress.c $(LIBS)
						$(CC) -o decompress decompress.c $(LIBS)

data_structure.o	: data_structure.c
util.o				: util.c
file.o				: file.c

clean : 
	rm -f $(BINS) $(LIBS) *LZW deLZW*