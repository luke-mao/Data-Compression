#ifndef _DECOMPRESS_FUNC_H_
#define _DECOMPRESS_FUNC_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "util.h"
#include "data_structure.h"
#include "file.h"


// check if the input file has .LZW at the end
// if yes, return true
bool check_input_file(const char*);

// delete .LZW, and add deLZW at front
char* create_decompressed_file_name(const char*);

// decompression cycle, each cycle ends when reach index = reflush
bool decompression_cycle(
    int* buffer, int* buffer_bits_not_read,
    Array a, FILE* fp_in, FILE* fp_out
);

// print input and output file names
void decompress_status(const char* file_in, const char* file_out);

#endif