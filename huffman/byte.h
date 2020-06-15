#include <stdio.h>
#include <stdlib.h>

#define ASCII_NUMBER 256

/*  Byte = unsigned char, 
    so that i can cover the whole ANSCII table.
    EOF is not used, rather than i count how many bytes
    using fseek and ftell with a for loop. */
typedef unsigned char Byte;

void print_byte(const Byte, const int);