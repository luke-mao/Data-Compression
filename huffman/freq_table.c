#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "freq_table.h"
#include "byte.h"
#include "file.h"


// create the freq table
FreqTable* create_table(void){
    FreqTable* t;
    t = (FreqTable*) malloc (1 * sizeof(FreqTable));
    if (t == NULL){
        fprintf(stderr, "memory allocation error create_table\n");
        exit(EXIT_FAILURE);
    }

    // initialize the counter
    t->counter = 0;

    // "t" is created, now allocate each separate part
    t->buckets = (Bucket**) malloc (ASCII_NUMBER * sizeof(Bucket*));
    if (t->buckets == NULL){
        fprintf(stderr, "memory allocation error, t->buckets\n");
        exit(EXIT_FAILURE);
    }

    // NULL all the each bucket, 
    // since not all of them will appear in the file
    for (int i = 0; i < ASCII_NUMBER; i++){
        t->buckets[i] = NULL;
    }

    return t;
}


// destroy the freq table
FreqTable* destroy_table(FreqTable* t){

    // free from inner towards outer
    for (int i = 0; i < ASCII_NUMBER; i++){
        if (t->buckets[i] != NULL){
            free(t->buckets[i]);
            t->buckets[i] = NULL;
        }
    }

    free(t->buckets);
    t->buckets = NULL;

    free(t);
    t = NULL;

    return t;
}

// fill in the table, given fp
void fill_table(FILE* fp, FreqTable* t){
    // check the inputs
    assert(fp != NULL && t != NULL);
    
    // now read byte by byte, then record
    long n_bytes = file_size(fp);           // fp is rewind to the beginning
    Byte buffer;

    for (long i = 0; i < n_bytes; i++){
        // get the new byte
        buffer = getc(fp);

        if (t->buckets[(int)buffer] == NULL){
            // assign the memory
            t->buckets[(int)buffer] = (Bucket*) malloc(sizeof(Bucket));
            if (t->buckets[(int)buffer] == NULL){
                fprintf(stderr, "memory error: t->buckets[buffer]\n");
                exit(EXIT_FAILURE);
            }

            // adjust the number, start from 1
            t->buckets[(int)buffer]->freq = 1;
            t->buckets[(int)buffer]->cw_count = 0;

            // update the counter
            t->counter++;
        }
        else{
            t->buckets[(int)buffer]->freq += 1;
        }
    }

    /*  no need to find the pesudo_eof
        in the header, include how many bits for pad at the end of file
        so during reading, omit the last few bits is enough.*/

    return;    
}


// printout the frequency statistics
void print_table_freq(const FreqTable* t){
    assert(t != NULL);

    fprintf(stdout, "Freq table (freq)\n");

    for (int i = 0; i < ASCII_NUMBER; i++){
        if (t->buckets[i] != NULL){
            fprintf(stdout, "%c (%d) : %ld\n", i, i, t->buckets[i]->freq);
        }
    }

    return;
}


// print out the codeword
void print_table_codeword(const FreqTable* t){
    assert(t != NULL);

    fprintf(stdout, "Freq table (codeword)\n");

    for (int i = 0; i < ASCII_NUMBER; i++){
        if (t->buckets[i] != NULL){
            // print, separate into two parts
            fprintf(stdout, "%c (%d): ", i, i);
            print_byte(t->buckets[i]->codeword, t->buckets[i]->cw_count);
            fprintf(stdout, "\n");
        }
    }
    
    return;
}