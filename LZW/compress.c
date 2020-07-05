#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "file.h"
#include "data_structure.h"
#include "util.h"


// output this byte to the file, use pointer to transfer information
void print_to_file(FILE* fp, int* b, int* b_position, const CodeWord cw);
// output the last byte, pad if necessary
void final_print_to_file(FILE* fp, int* b, int* b_position);

void compress_stats(const char* original, const char* compressed);


int main(int argc, char** argv){
    
    // input check
    if (argc != 2){
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // open file and read
    char* filename = argv[1];
    FILE* fp = open_file_for_read(filename);

    // create the file name for output
    char* filename_out = (char*)malloc((strlen(filename)+4+1) * sizeof(char));
    if (filename_out == NULL){
        fprintf(stderr, "Memory error, filename out\n");
        exit(EXIT_FAILURE);
    }

    strcpy(filename_out, filename);
    strcat(filename_out, ".LZW");

    FILE* fp_out = open_file_for_write(filename_out);


    // create the dictionary
    Dictionary d = dictionary_create();

    // create variable for reading
    char* prev = NULL;
    char* prev_c = NULL;
    int c;
    CodeWord cw;
    
    // output use
    int b = 0;
    int b_position = 0;      

    // after every insert, check if the dictionary is full
    // if full, output reflush sign and re-initiate

    while ((c=getc(fp)) != EOF){

        // prev_c = prev + c
        prev_c = string_concat(prev, c);

        // printf("prev=%s, c=%c, prev_c=%s\n", prev, c, prev_c);

        // if dictionary contains prev+c
        if (dictionary_search(d, prev_c) != -1){
            // prev = prev + c
            // assign, first free if necessary
            if (prev != NULL){
                free(prev);
                prev = NULL;
            }

            // prev = prev_c
            prev = (char*)malloc((strlen(prev_c)+1)*sizeof(char));
            assert(prev != NULL);
            strcpy(prev, prev_c);


            // printf("no code output, prev become %s\n", prev);
        }    
        else{
            // first output code(prev)
            cw = dictionary_search(d, prev);
            assert(cw >= 0);
            print_to_file(fp_out, &b, &b_position, cw);

            // printf("Code output %d, ", cw);

            // add prev+c to dictionary
            cw = dictionary_insert(d, prev_c);
            assert(cw >= 0);

            // prev = c
            prev = string_concat(NULL, c);

            // printf("Insert prev_c=%s into dic, return index cw=%d, prev become %s\n", 
            //     prev_c, cw, prev);


            // when the dictionary is full, need to finish all output first, 
            // thenprint index for reflush
            if (dictionary_is_full(d)){
                // finish output first
                cw = dictionary_search(d, prev);
                print_to_file(fp_out, &b, &b_position, cw);
                
                // print index for reflush
                print_to_file(fp_out, &b, &b_position, INDEX_REFLUSH);
                
                // clear all memory and start again
                d = dictionary_reset(d);
                prev = NULL;
                prev_c = NULL;
                continue;       // go to the start of the loop
            }
        }      

        // release the memory
        free(prev_c);
        prev_c = NULL;
    }


    // here we need to print code(prev)
    cw = dictionary_search(d, prev);
    print_to_file(fp_out, &b, &b_position, cw);
    // printf("output cw = %d\n", cw);

    // print the index eof
    print_to_file(fp_out, &b, &b_position, INDEX_EOF);
    final_print_to_file(fp_out, &b, &b_position);

    // printf("output %d\n", INDEX_EOF);

    // close file
    close_file(fp);
    close_file(fp_out);

    // free the memory
    // dictionary_print(d);
    // printf("dictionary cur-number = %d\n", d->current_num);
    dictionary_destroy(d);

    // statistics
    compress_stats(filename, filename_out);

    return 0;
}


void print_to_file(FILE* fp, int* b, int* b_position, const CodeWord cw){
    // bits position length
    int cw_position = BITS;
    int mask;

    // output codeword cw into a 12 bits

    // while cw has not been printed into 12 bits, continue
    while (cw_position > 0){
        *b <<= 1;       // left shift 1 position
        mask = (1 << (cw_position - 1));
        *b |= ((cw & mask) >> (cw_position - 1));

        cw_position -= 1;
        *b_position += 1;

        if (*b_position == 8){
            putc(*b, fp);
            *b_position = 0;
            *b = 0;
        }
    }

    return;
}


void final_print_to_file(FILE* fp, int* b, int* b_position){
    if (*b_position != 0){
        *b <<= (8 - *b_position);
        putc(*b, fp);
        
        *b = 0;
        *b_position = 0;
    }

    return;
}


void compress_stats(const char* original, const char* compressed){
    // print out the file size change, and calculate the compress ratio
    FILE* before = open_file_for_read(original);
    FILE* after = open_file_for_read(compressed);

    fseek(before, 0L, SEEK_END);
    fseek(after, 0L, SEEK_END);

    long before_size = ftell(before);
    long after_size = ftell(after);

    fprintf(stdout, "LZW Compression statistics:\n");

    fprintf(
        stdout, 
        "Input filename=\"%s\". Output filename=\"%s\"\n", original, compressed
    );

    fprintf(
        stdout, 
        "Size from %ld => %ld (Bytes)\n", before_size, after_size
    );

    fprintf(
        stdout, 
        "Space saving: %.2f%%\n", ((float)1 - (float)after_size / before_size)*100
    );

    close_file(before);
    close_file(after);
    return;
}   