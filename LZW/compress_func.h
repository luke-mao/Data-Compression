#ifndef _COMPRESS_FUNC_H_
#define _COMPRESS_FUNC_H_

#include <stdio.h>
#include <stdlib.h>
#include "file.h"
#include "data_structure.h"
#include "util.h"

// create the output file name, add .LZW at the end
char* create_compressed_file_name(const char* original);

// output this byte to the file, use pointer to transfer information
void print_to_file(FILE* fp, int* b, int* b_position, const CodeWord cw);

// output the last byte, pad if necessary
void final_print_to_file(FILE* fp, int* b, int* b_position);

// print the compression status, including compression ratio
void compress_stats(const char* original, const char* compressed);

#endif