/*
* Compression implementation
*/


#ifndef _COMPRESS_H_
#define _COMPRESS_H_


#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "list.h"
#include "dictionary.h"
#include "file.h"
#include "update.h"


// generate the compressed file name: add .v suffix
char* compression_create_output_filename(char* filename_in);


// main function for compression
void compress_file_and_output(FILE* fp_in, FILE* fp_out);


// print both file names, and calculate the compression ratio
void compression_status(char* name_in, char* name_out, FILE* fp_in, FILE* fp_out);


#endif 