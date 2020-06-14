#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "file.h"

// open the file, exit if error during opening
FILE* open_file(const char* file_name){
    FILE* fp = fopen(file_name, "rb");
    if (fp == NULL){
        fprintf(stderr, "Error opening file \"%s\"\n", file_name);
        exit(EXIT_FAILURE);
    }

    return fp;
}


FILE* open_file_write(const char* file_name){
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