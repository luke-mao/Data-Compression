#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "util.h"
#include "data_structure.h"
#include "file.h"
#include "decompress_func.h"


int main(int argc, char** argv){

    // first check the input number
    if (argc != 2){
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // check the input file name with .LZW at the end
    if (! check_input_file(argv[1])){
        fprintf(stderr, "Usage: %s <filename.LZW>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // first create the new name for decompressed file, prefix = deLZW_
    char* file_in = argv[1];
    char* file_out = create_decompressed_file_name(file_in);

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

    // finish, close the file
    fclose(fp_in);
    fclose(fp_out);

    // output the status
    decompress_status(file_in, file_out);

    // free the memory
    array_destroy(a);
    free(file_out);

    return 0;
}
