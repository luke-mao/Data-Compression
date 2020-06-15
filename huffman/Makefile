CC=gcc
CFLAGS=-Wall -g -c
LIBS=compress_func.o data_structure.o freq_table.o file.o byte.o
BINS=compress 

all : $(LIBS) $(BINS)

compress : compress.c $(LIBS)
	$(CC) -o compress compress.c $(LIBS)

compress_func.o : compress_func.c freq_table.o data_structure.o file.o
data_structure.o : data_structure.c freq_table.o byte.o
freq_table.o : freq_table.c byte.o file.o
file.o : file.c
byte.o : byte.c

clean : 
	rm -f $(BINS) $(LIBS)