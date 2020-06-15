CC=gcc
CFLAGS=-Wall

a: main.h main.c file.h file.c freq_table.h freq_table.c huffman_tree.h huffman_tree.c byte.h byte.c compress.h compress.c
	$(CC) $(CFLAGS) -g main.h main.c file.h file.c freq_table.h freq_table.c huffman_tree.h huffman_tree.c byte.h byte.c compress.h compress.c -o a
clean:
	rm -f a