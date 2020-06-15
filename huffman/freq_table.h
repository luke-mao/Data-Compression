#ifndef FREQ_TABLE_H_
#define FREQ_TABLE_H_

#include <stdio.h>

/*
    Byte = unsigned char, 
    so that i can cover the whole ANSCII table.
    EOF is not used, rather than i count how many bytes
    using fseek and ftell with a for loop. 
*/
typedef unsigned char Byte;

typedef struct char_bucket{
    long freq;
    Byte codeword;
    int cw_count;
} Bucket;

typedef struct freqtable{
    int counter;         // total variety of char in the file, include p_eof
    Byte p_eof;         // pesudo_eof
    Bucket** buckets;
} FreqTable;


// create the freq table
FreqTable* create_table(void);

// destroy the freq table
FreqTable* destroy_table(FreqTable*);

// fill in the table, given fp
// also find the pesudo eof
// exit if no potential candidate for pesudo eof
void fill_table(FILE*, FreqTable*);


// printout the frequency statistics
void print_table_freq(const FreqTable*);
void print_table_codeword(const FreqTable*);


#endif