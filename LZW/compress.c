#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "file.h"
#include "data_structure.h"
#include "util.h"
#include "compress_func.h"


/*
Main LZW compression programme. 
Adapative compression, only need to read the file once. 
Create the codeword during the reading. 
*/

int main(int argc, char** argv){
    
    // input check
    if (argc != 2){
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // get both file names and open
    char* filename = argv[1];
    char* filename_out = create_compressed_file_name(filename);

    FILE* fp = open_file_for_read(filename);
    FILE* fp_out = open_file_for_write(filename_out);

    // create the dictionary
    Dictionary d = dictionary_create();

    // create variable for reading
    char* prev = NULL;
    char* prev_c = NULL;
    int c;
    CodeWord cw;
    
    // output use, buffer and the buffer position
    // position = 8, meaning 8 bits ready, so ok to output
    int b = 0;
    int b_position = 0;      

    // after every insert, check if the dictionary is full
    // if full, output reflush sign and re-initiate

    while ((c=getc(fp)) != EOF){
        // prev_c = prev + c
        prev_c = string_concat(prev, c);

        // if dictionary contains prev+c
        if (dictionary_search(d, prev_c) != -1){
            // prev = prev + c, assign, first free if necessary
            if (prev != NULL){
                free(prev);
                prev = NULL;
            }

            // prev = prev_c
            prev = (char*)malloc((strlen(prev_c)+1)*sizeof(char));
            assert(prev != NULL);
            strcpy(prev, prev_c);
        }    
        else{
            // first output code(prev)
            cw = dictionary_search(d, prev);
            assert(cw >= 0);
            print_to_file(fp_out, &b, &b_position, cw);

            // add prev+c to dictionary
            cw = dictionary_insert(d, prev_c);
            assert(cw >= 0);

            // prev = c
            prev = string_concat(NULL, c);

            // when the dictionary is full, need to finish all output first, 
            // thenprint index for reflush
            if (dictionary_is_full(d)){
                // first finish all outputs
                cw = dictionary_search(d, prev);
                print_to_file(fp_out, &b, &b_position, cw);
                
                // then print index for reflush
                print_to_file(fp_out, &b, &b_position, INDEX_REFLUSH);
                
                // at last, clear all memory and start again
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

    // reach EOF, output the last codeword code(prev)
    cw = dictionary_search(d, prev);
    print_to_file(fp_out, &b, &b_position, cw);

    // print the pesudo index eof
    print_to_file(fp_out, &b, &b_position, INDEX_EOF);
    final_print_to_file(fp_out, &b, &b_position);

    // close file
    close_file(fp);
    close_file(fp_out);

    // free the memory
    dictionary_destroy(d);

    // statistics
    compress_stats(filename, filename_out);

    return 0;
}
