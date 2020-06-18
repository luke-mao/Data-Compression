#ifndef COMPRESS_FUNC_H_
#define COMPRESS_FUNC_H_

#include <stdio.h>
#include <stdlib.h>
#include "freq_table.h"
#include "data_structure.h"

/*  
    Compression main functions

    Task: 
    1. create the name of the output file, add .huff at the end
    2. write header
    3. write body
    4. refine the second byte of header
    
    Header includes the following:
    First byte: number of different chars present in the file, i.e. number of leaf nodes in the tree
    Second byte: number of bits padded to the end of body (in this way omit the use of pesudo_eof).
                 it is set as 0 first during header writing, then fseek(fp, 1L, SEEK_SET) to change this byte.
                 pads = 00000111 => last three digits of the body are pad
    Remaining bytes: post order traversal of the tree
*/

char* compress(char* file_name, const FreqTable* t, const Tree* tr, CodeWord**);
void compress_stats(const char* original, const char* compress);

#endif