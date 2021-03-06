/*
* Decompression implementation
*/


#ifndef _DECOMPRESS_H_
#define _DECOMPRESS_H_


#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "list.h"
#include "dictionary.h"


// remove .v suffix, and add deVitter_ prefix
char* decompression_create_output_filename(char* filename_in);


// top function for decompression
void decompress_file_and_output(FILE* fp_in, FILE* fp_out);


// print the file names
void decompression_status(char* name_in, char* name_out, FILE* fp_in, FILE* fp_out);


#endif 