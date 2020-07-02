#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "file.h"

// open the file, exit if error during opening
FILE* open_file_for_read(const char* file_name){
    FILE* fp = fopen(file_name, "rb");
    if (fp == NULL){
        fprintf(stderr, "Error opening file \"%s\"\n", file_name);
        exit(EXIT_FAILURE);
    }

    return fp;
}


FILE* open_file_for_write(const char* file_name){
    FILE* fp = fopen(file_name, "wb");
    if (fp == NULL){
        fprintf(stderr, "Error opening file \"%s\"\n", file_name);
        exit(EXIT_FAILURE);
    }

    return fp;
}


// close the file
FILE* close_file(FILE* fp){
    fclose(fp);
    fp = NULL;
    return fp;
}


// get the file size, return in long
// rewind the pointer
long file_size(FILE* fp){
    assert(fp != NULL);

    fseek(fp, 0L, SEEK_END);    // locate to the end of the file
    long result = ftell(fp);     // return the file size relative to the beginning
    fseek(fp, 0L, SEEK_SET);    // same as rewind(fp)

    return result;
}