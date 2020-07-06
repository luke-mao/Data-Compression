#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "util.h"
#include "data_structure.h"
#include "file.h"
#include "decompress_func.h"


// check if the input file has .LZW at the end
bool check_input_file(const char* filename){
   // second check if the file has .LZW as the end
    int file_name_length = strlen(filename);
    return ( file_name_length>=5 
                && strcmp(".LZW", filename + file_name_length - 4)==0);
}


// remove .LZW at the end, and add deLZW at the front
char* create_decompressed_file_name(const char* file_in){
    assert(file_in != NULL);

    // allocate the size
    // -4 => minus .LZW
    // +6 => prefix deLZW_
    // +1 => \0
    char* result = (char*)malloc(strlen(file_in) - 4 + 6 + 1);
    assert(result != NULL);

    result[0] = '\0';
    strcpy(result, "deLZW_");
    strncat(result, file_in, strlen(file_in)-4);

    return result;
}


// each cycle stops when meet reflush or pesudo eof
bool decompression_cycle(int* buffer, int* buffer_bits_not_read,
                            Array a, FILE* fp_in, FILE* fp_out){
    // each cycle ends whether meet reflush, or eof
    // if reflush, return false
    // if eof, return true
    bool flag = false;
    
    int mask;
    CodeWord cw = 0;
    int cw_bits = 0;

    // read in a code, normally around 12 bits
    while (cw_bits != BITS){
        // read buffer if necessary
        if (*buffer_bits_not_read == 0){
            *buffer = fgetc(fp_in);
            *buffer_bits_not_read = 8;
        }

        mask = 1 << (*buffer_bits_not_read - 1);
        
        cw <<= 1;
        cw |= ((*buffer & mask) >> (*buffer_bits_not_read - 1));

        cw_bits += 1;
        *buffer_bits_not_read -= 1;
    }

    // main part for decompression
    // define the variables
    char *prev_key, *curr_key;
    prev_key = NULL; curr_key = NULL;

    // assign prev_key, this is the first bit
    prev_key = array_search(a, cw);
    // output the first byte
    fputs(prev_key, fp_out);

    // while we have not reach the index eof = 256
    while (true){
        // every time, clear the code first
        cw = 0;
        cw_bits = 0;

        // read in a code, store in cw
        while (cw_bits != BITS){
            // read buffer if necessary
            if (*buffer_bits_not_read == 0){
                *buffer = fgetc(fp_in);
                *buffer_bits_not_read = 8;
            }

            mask = 1 << (*buffer_bits_not_read - 1);

            cw <<= 1;
            cw |= ((*buffer & mask) >> (*buffer_bits_not_read - 1));

            cw_bits += 1;
            *buffer_bits_not_read -= 1;
        }

        // if code = 256, eof
        if (cw == INDEX_EOF){
            flag = true;
            break;
        }

        // if code = 257, reflush
        if (cw == INDEX_REFLUSH){
            break;
        }

        // if cw is in the table
        if (array_has_this_codeword(a, cw)){
            // output string(cw)
            curr_key = array_search(a, cw);
            fputs(curr_key, fp_out);

            // string(prev) + first char of string(cw)
            prev_key = (char*)realloc(prev_key, strlen(prev_key)+1+1);
            assert(prev_key != NULL);
            strncat(prev_key, curr_key, 1);

            // insert into table
            array_insert(a, prev_key);
        }
        else{
            // special case for codeword created and used immediately
            // with no time gap
            // so that this codeword has not been inserted yet

            // prev_key + first char of prev_key
            // insert it into the table
            // and output it
            curr_key = (char*)malloc((strlen(prev_key)+1+1)*sizeof(char));
            assert(curr_key != NULL);

            // copy prev_key, and then concat the first char of the prev_key
            strcpy(curr_key, prev_key);
            strncat(curr_key, prev_key, 1);

            // insert into array
            array_insert(a, curr_key);
            
            // also print it out
            fputs(curr_key, fp_out);
        }

        // prev_key = curr_key
        free(prev_key);
        prev_key = NULL;
        prev_key = (char*)malloc((strlen(curr_key)+1)*sizeof(char));
        assert(prev_key != NULL);
        strcpy(prev_key, curr_key);

        // free curr_key
        free(curr_key);
        curr_key = NULL;
    }

    // return true if decompression finish
    // return false if meet reflush
    return flag;
}


// output the file name
void decompress_status(const char* file_in, const char* file_out){
    assert(file_in != NULL && file_out != NULL);

    fprintf(stdout, "LZW decompression statistics:\n");
    fprintf(stdout, "Input file = \"%s\". Output file = \"%s\"\n", file_in, file_out);

    return ;
}