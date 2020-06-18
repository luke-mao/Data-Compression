#ifndef FREQ_TABLE_H_
#define FREQ_TABLE_H_

#include <stdio.h>
#include "byte.h"

// Bucket, for the frequent table
typedef struct char_bucket{
    long freq;
    int cw_count;
} Bucket;

// FreqTable
typedef struct freqtable{
    int counter;         // total variety of char in the file, include p_eof
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

#endif