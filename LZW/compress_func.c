#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "file.h"
#include "data_structure.h"
#include "util.h"
#include "compress_func.h"


// create the compressed file name, add .LZW at the end
char* create_compressed_file_name(const char* filename){
    assert(filename != NULL);

    // create the file name for output
    char* filename_out = (char*)malloc((strlen(filename)+4+1) * sizeof(char));
    assert(filename_out != NULL);

    strcpy(filename_out, filename);
    strcat(filename_out, ".LZW");

    return filename_out;
}


// print this byte to the file
// use pointer so that the remaining bits can be saved and printed later
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


// pad the remaining bits in the buffer, and print out.
// if no remaining bits, then do nothing. 
void final_print_to_file(FILE* fp, int* b, int* b_position){
    if (*b_position != 0){
        *b <<= (8 - *b_position);
        putc(*b, fp);
        
        *b = 0;
        *b_position = 0;
    }

    return;
}


// printout the compression ratio
void compress_stats(const char* original, const char* compressed){
    // print out the file size change, and calculate the compress ratio
    FILE* before = open_file_for_read(original);
    FILE* after = open_file_for_read(compressed);

    fseek(before, 0L, SEEK_END);
    fseek(after, 0L, SEEK_END);

    long before_size = ftell(before);
    long after_size = ftell(after);

    fprintf(stdout, "LZW compression statistics:\n");

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