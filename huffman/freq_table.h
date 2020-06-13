#ifndef FREQ_TABLE_H_
#define FREQ_TABLE_H_

#include "main.h"

typedef char Byte;

typedef struct char_bucket{
    long freq;
    Byte codeword;
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
void print_table(const FreqTable*);


#endif