#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "util.h"
#include "data_structure.h"
#include "file.h"


char* create_decompressed_file_name(const char*);
void decompress_status(const char* file_in, const char* file_out);


int main(int argc, char** argv){

    // first check the input number
    if (argc != 2){
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // second check if the file has .LZW as the end
    int file_name_length = strlen(argv[1]);
    if (! (file_name_length>=5 && strcmp(".LZW", argv[1] + file_name_length - 4)==0)){
        fprintf(stderr, "Usage: %s <filename.LZW>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // first create the new name for decompressed file, prefix = deLZW_
    char* file_in = argv[1];
    char* file_out = create_decompressed_file_name(file_in);
    // printf("out file = %s", file_out);

    // create the file descriptor
    FILE* fp_in = open_file_for_read(file_in);
    FILE* fp_out = open_file_for_write(file_out);

    // create the array
    Array a = array_create();

    // read input file, every time extract BITS (12) bits from the input buffer
    // then determine the content using the array

    // pseudo_code: read(c), output c, and then p = c
    int mask = 0;
    int buffer = getc(fp_in);
    int buffer_bits_not_read = 0;       // = 3: means 3 bits left to read next time

    int c_bits = buffer;     // read c
    int c_bits_in = 8;  // have got 8 bits now, so need 4 extra bits
    buffer = getc(fp_in);    
    buffer_bits_not_read = 8;       // total 8 bits not read

    while (c_bits_in != BITS){
        mask = (1 << (buffer_bits_not_read - 1));
        c_bits <<= 1;
        c_bits |= ((buffer & mask) >> (buffer_bits_not_read-1));

        c_bits_in += 1;
        buffer_bits_not_read -= 1;

        if (buffer_bits_not_read == 0){
            buffer = getc(fp_in);
            buffer_bits_not_read = 8;
        }
    }

    // now we have the c, output c
    Key c_key, prev_key;
    c_key = array_search(a, c_bits, NULL);
    fputs(c_key, fp_out);
    prev_key = c_key;

    // clear c
    c_bits = 0;
    c_bits_in = 0;

    // we stop the reading when read pseudo_eof (index = 256)
    // reflush the array when read index = 257
    while (true){
        // read bits
        while (c_bits_in != BITS){
            // read another char if needed
            if (buffer_bits_not_read == 0){
                buffer = fgetc(fp_in);
                buffer_bits_not_read = 8;
            }

            // each time just add one bit, easier to code
            mask = 1 << (buffer_bits_not_read - 1);
            c_bits <<= 1;
            c_bits |= ((buffer & mask) >> (buffer_bits_not_read-1));

            c_bits_in += 1;
            buffer_bits_not_read -= 1;
        }

        printf("%d\n", c_bits);

        // now we have the char, determine if it is 256 or 257
        if (c_bits == INDEX_EOF){
            break;
        }

        if (c_bits == INDEX_REFLUSH){
            a = array_reset(a);

            puts("reflush ok");
            // update the bit
            c_bits = 0;
            c_bits_in = 0;
            continue;
        }

        // for normal index
        // output dict[c], 
        c_key = array_search(a, c_bits, prev_key);
        fputs(c_key, fp_out);

        // add p + dict[c][0] to dict
        prev_key = (Key)realloc(prev_key, strlen(prev_key)+1+1);
        if (prev_key == NULL){
            fprintf(stderr, "Memory error, decomprss, prev_key\n");
            exit(EXIT_FAILURE);
        }
        // only add one char from c
        strncat(prev_key, c_key, 1);
        // insert into dictionary
        array_insert(a, prev_key);

        // p = dict[c]
        prev_key = c_key;   

        // clear c_bits
        c_bits = 0;
        c_bits_in = 0;
    }


    // finish, close the file
    fclose(fp_in);
    fclose(fp_out);

    decompress_status(file_in, file_out);
    free(file_out);

    return 0;
}


char* create_decompressed_file_name(const char* file_in){
    assert(file_in != NULL);

    // allocate the size
    // -4 => minus .LZW
    // +6 => prefix deLZW_
    // +1 => \0
    char* result = (char*)malloc(strlen(file_in) - 4 + 6 + 1);
    if (result == NULL){
        fprintf(stderr, "Memory error: create decompressed file name\n");
        exit(EXIT_FAILURE);
    }

    result[0] = '\0';
    strcpy(result, "deLZW_");
    strncat(result, file_in, strlen(file_in)-4);

    return result;
}


void decompress_status(const char* file_in, const char* file_out){
    assert(file_in != NULL && file_out != NULL);

    fprintf(stdout, "LZW decompression statistics:\n");
    fprintf(stdout, "Input file = \"%s\". Output file = \"%s\"\n", file_in, file_out);

    return ;
}