CC=gcc
CFLAGS=-Wall -g -c
LIBS=file.o util.o data_structure.o compress_func.o decompress_func.o
BINS=compress decompress

all : $(LIBS) $(BINS)

compress 			: compress.c $(LIBS)
						$(CC) -o compress compress.c $(LIBS)
decompress			: decompress.c $(LIBS)
						$(CC) -o decompress decompress.c $(LIBS)

decompress_func.o	: decompress_func.c
compress_func.o		: compress_func.c
data_structure.o	: data_structure.c
util.o				: util.c
file.o				: file.c

clean : 
	rm -f $(BINS) $(LIBS) *LZW deLZW*