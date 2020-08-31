CC=gcc
CFLAGS=-Wall -g -c
LIBS=tree.o FGK_functions.o
BINS=a

all : $(LIBS) $(BINS)

a					: FGK.c $(LIBS)
						$(CC) -o a FGK.c $(LIBS)

FGK_functions.o		: FGK_functions.c
tree.o				: tree.c

clean : 
	rm -f $(BINS) $(LIBS) *.FGK deFGK*