#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tree.h"
#include "list.h"
#include "dictionary.h"
#include "compress.h"


// add .vitter suffix
char* compression_create_output_filename(char* filename_in){
    assert(filename_in != NULL);

    int len = strlen(filename_in);
    char* filename_out = (char*) malloc((len+7+1) * sizeof(char));

    strcpy(filename_out, filename_in);
    strcat(filename_out, ".vitter");

    return filename_out;
}


void compress_file_and_output(FILE* fp_in, FILE* fp_out){
///////////////



    return;
}


void compression_status(char* name_in, char* name_out, FILE* fp_in, FILE* fp_out){
    assert(fp_in != NULL && fp_out != NULL);
    assert(name_in != NULL && name_out != NULL);

    fseek(fp_in, 0, SEEK_END);
    fseek(fp_out, 0, SEEK_END);

    long size_in = ftell(fp_in);
    long size_out = ftell(fp_out);

    printf("Input file: %s\nSize: %.3f KB\n", name_in, (float) size_in / 1024);
    printf("Output file: %s\nSize: %.3f KB\n", name_out, (float) size_out / 1024);
    printf("Space saving: %.3f%%\n", (float) 1 - (float)size_out / (float) size_in);

    return;
}

