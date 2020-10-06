/*
* Compression implementation:
* Include file handling, creating new file
* and the compression details
*/


#ifndef _DECOMPRESS_H_
#define _DECOMPRESS_H_


#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "list.h"
#include "dictionary.h"


// function
char* decompression_create_output_filename(char* filename_in);
void decompress_file_and_output(FILE* fp_in, FILE* fp_out);
void decompression_status(char* name_in, char* name_out, FILE* fp_in, FILE* fp_out);

#endif 