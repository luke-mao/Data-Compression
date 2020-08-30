#ifndef BYTE_H_
#define BYTE_H_

#include <stdio.h>
#include <stdlib.h>

#define ASCII_NUMBER 256

/*  Byte = unsigned char, 
    so that i can cover the whole ANSCII table.
    EOF is not used, rather than i count how many bytes
    using fseek and ftell with a for loop. */
typedef unsigned char Byte;
void print_byte(const Byte, const int);


/*  
    Define for the codeword. So that all bits are stored properly.
    And prevent overflow. 
*/
typedef struct CW{
    int num;
    int array[1];
} CodeWord;


CodeWord** create_cw(void);
void create_cw_with_index(CodeWord** cw, const int index, const int maxlen);
CodeWord** destroy_cw(CodeWord**);
void print_cw(CodeWord**);


#endif