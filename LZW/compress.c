#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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
    char* prev_c;
    int c;
    CodeWord cw;
    
    // output use
    int b = 0;
    int b_position = 0;       

    while ((c=getc(fp)) != EOF){
        // form pc
        prev_c = string_concat(prev, c);

        // if prev_buffer
        if (dictionary_search(d, prev_c) != -1){
            prev = prev_c;
            
        }    
        else{
            // insert prev_c
            CodeWord ccc = dictionary_insert(d, prev_c);
            // fprintf(stdout, "code = %d\n", ccc);
            
            // output code(p)
            // print 12 bits
            int check = dictionary_search(d, prev);
            if (check < 0){
                printf("output %s| %d | %d\n", prev, prev[0], check);
                printf("error\n");
                exit(EXIT_FAILURE);
            }
            
            // printf("output:%d\n", check);
            print_to_file(fp_out, &b, &b_position, check);

            // prev = c, but use the function to make it a string
            prev = string_concat(NULL, c);

            // if the dictionary becomes full, print reflush and reset 
            if (dictionary_is_full(d)){
                print_to_file(fp_out, &b, &b_position, INDEX_REFLUSH);
                
                // clear all memory and start again
                d = dictionary_reset(d);
                prev = NULL;
                prev_c = NULL;
            }
        }      
    }


    // here we need to print code(prev)
    // then print EOF sign which is 256
    // and then pad, if necessary, and close files
    int check = dictionary_search(d, prev);
    print_to_file(fp_out, &b, &b_position, check);

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


void print_to_file(FILE* fp, int* b, int* b_position, const CodeWord cw){
    // bits position length
    int cw_position = BITS;
    int mask;

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