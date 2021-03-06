/*
* Main function.
* Usage: ./vitter <-d|-c> <input file>
* -d for decompression, -c for compression
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "file.h"
#include "update.h"
#include "compress.h"
#include "decompress.h"


// top-level compress and decompress functions
// will call the individual header files
void compress(char* filename);
void decompress(char* filename);


// main function
int main(int argc, char** argv){
    if (argc != 3){
        fprintf(stderr, "Usage: %s <-c|-d> <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }


    if (strcmp(argv[1], "-c") == 0){
        compress(argv[2]);
    }
    else if (strcmp(argv[1], "-d") == 0){
        decompress(argv[2]);
    }
    else{
        fprintf(stderr, "Usage: %s <-c|-d> <file>\n", argv[0]);
        exit(EXIT_FAILURE);        
    }

    return 0;
}


void compress(char* filename_in){
    assert(filename_in != NULL);
    
    char* filename_out = compression_create_output_filename(filename_in);

    // open both files
    FILE* fp_in = open_the_file(filename_in, "rb");
    FILE* fp_out = open_the_file(filename_out, "wb");

    // compression
    compress_file_and_output(fp_in, fp_out);
    
    // print some statistics
    compression_status(filename_in, filename_out, fp_in, fp_out);

    // close files
    close_the_file(fp_in);
    close_the_file(fp_out);

    free(filename_out);
    filename_out = NULL;

    return;
}


void decompress(char* filename){
    assert(filename != NULL);
    char* filename_out = decompression_create_output_filename(filename);

    FILE* fp_in = open_the_file(filename, "rb");
    FILE* fp_out = open_the_file(filename_out, "wb");

    decompress_file_and_output(fp_in, fp_out);

    decompression_status(filename, filename_out, fp_in, fp_out);

    close_the_file(fp_in);
    close_the_file(fp_out);

    free(filename_out);
    filename_out = NULL;
    return;
}
