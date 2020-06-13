#include "freq_table.h"


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
    Byte buffer = getc(fp);
    while (buffer != EOF){

        // if the corresponding char bucket is empty
        if (t->buckets[(int)buffer] == NULL){
            // assign the memory
            t->buckets[(int)buffer] = (Bucket*) malloc(sizeof(Bucket));
            if (t->buckets[(int)buffer] == NULL){
                fprintf(stderr, "memory error: t->buckets[buffer]\n");
                exit(EXIT_FAILURE);
            }

            // adjust the number, start from 1
            t->buckets[(int)buffer]->freq = 1;

            // update the counter
            t->counter++;
        }
        else{
            t->buckets[(int)buffer]->freq += 1;
        }

        // get the next byte
        buffer = getc(fp);
    }


    // now determine the pesudo eof
    // use the first not occurred number as the pesudo eof
    // return error if no such number is found
    // but typically not possible

    bool found = false;

    for (int i = 0; i < ASCII_NUMBER; i++){
        if (t->buckets[i] == NULL){
            // found
            found = true;

            // assign the memory
            t->buckets[i] = (Bucket*) malloc(sizeof(Bucket));
            if (t->buckets[i] == NULL){
                fprintf(stderr, "memory error: t->buckets[buffer]\n");
                exit(EXIT_FAILURE);
            }

            // freq = 1
            t->buckets[i]->freq = 1;    
            t->p_eof = (Byte) i;    
            break;   
        }
    }

    if (! found){
        fprintf(stderr, "get_pesudo_eof: not found potential char\n");
        exit(EXIT_FAILURE);
    }

    return;    
}


// printout the frequency statistics
void print_table(const FreqTable* t){
    
    assert(t != NULL);

    for (int i = 0; i < ASCII_NUMBER; i++){
        if (t->buckets[i] != NULL){
            if ((Byte) i == t->p_eof){
                fprintf(stdout, "pseudo_eof: 1\n");
            }
            else{
                fprintf(stdout, "%c : %ld\n", i, t->buckets[i]->freq);
            }
        }
    }

    return;
}