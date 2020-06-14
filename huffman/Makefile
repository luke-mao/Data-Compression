CC=gcc
CFLAGS=-Wall

a: main.h main.c file.h file.c freq_table.h freq_table.c huffman_tree.h huffman_tree.c byte.h byte.c compress.h compress.c
	$(CC) $(CFLAGS) -o a main.h main.c file.h file.c freq_table.h freq_table.c huffman_tree.h huffman_tree.c byte.h byte.c compress.h compress.c
clean:
	rm -f a