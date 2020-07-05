#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "util.h"
#include "data_structure.h"
#include "file.h"

// delete .LZW, and add deLZW at front
char* create_decompressed_file_name(const char*);

// decompression cycle, each cycle ends when reach index = reflush
bool decompression_cycle(
    int* buffer, int* buffer_bits_not_read,
    Array a, FILE* fp_in, FILE* fp_out
);

// print input and output file names
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
    
    // decompression
    int buffer = fgetc(fp_in); 
    int buffer_bits_not_read = 8;
    // return false if meet reflush, true if meet eof
    bool finish = decompression_cycle(
        &buffer, &buffer_bits_not_read,
        a, fp_in, fp_out
    );

    // continue the decompression until meet eof
    while (! finish){
        // meet reflush sign, refresh array
        a = array_reset(a);
        finish = decompression_cycle(
            &buffer, &buffer_bits_not_read,
            a, fp_in, fp_out
        );
    }

    // array_print(a);
    // printf("array curr-num = %d\n", a->current_num);

    // finish, close the file
    fclose(fp_in);
    fclose(fp_out);

    array_destroy(a);

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

    // printf("read cw = %d\n", cw);


    // main part for decompression
    // define the variables
    char *prev_key, *curr_key;
    prev_key = NULL; curr_key = NULL;

    // assign prev_key, this is the first bit
    prev_key = array_search(a, cw);
    // output the first byte
    fputs(prev_key, fp_out);

    // printf("output = %s\n", prev_key);

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

        // printf("read cw = %d\n", cw);

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
            Index jjj = array_insert(a, prev_key);


            // printf("has this cw, output %s, insert %s, return index = %d", curr_key, prev_key, jjj);
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
            Index jjj = array_insert(a, curr_key);
            
            // also print it out
            fputs(curr_key, fp_out);

            // printf("cw not in array, insert %s into array with index = %d, printout %s\n", curr_key, jjj, curr_key);
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

        // printf("prev_key = %s\n\n", prev_key);
    }

    // return true if decompression finish
    // return false if meet reflush
    return flag;
}