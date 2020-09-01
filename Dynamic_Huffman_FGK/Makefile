CC=gcc
CFLAGS=-Wall -g -c
LIBS=tree.o FGK_functions.o
BINS=FGK

all : $(LIBS) $(BINS)

FGK					: FGK.c $(LIBS)
						$(CC) -o FGK FGK.c $(LIBS)

FGK_functions.o		: FGK_functions.c
tree.o				: tree.c

clean : 
	rm -f $(BINS) $(LIBS) *.FGK deFGK*