// Include FGK main functions for compression and decompression
// Also include the bit operation functions


#ifndef _FGK_FUNCTIONS_H_
#define _FGK_FUNCTIONS_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>


const int power_of_two[] = {1, 2, 4, 8, 16, 32, 64, 128};



// compression, decompression
void FGK_compress(const char* filename);
void FGK_decompress(const char* filename);


// bit operation
void print_to_file(FILE* fp_out, char* out_c, int* out_c_num, int new_c, int new_c_num);
void pad_last_bit(FILE* fp_out, char* out_c, int* out_c_num);

#endif 