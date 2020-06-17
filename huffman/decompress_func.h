#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "byte.h"
#include "file.h"
#include "data_structure.h"


/*
    check if the file name is valid
    valid name has .huff at the end
    valid return true, invalid return false
*/
bool check_valid_file_name(const char *filename);


// based on the input file name, remove the .huff suffix
// add "dehuff_" at the start of the filename
char* create_output_file_name(const char* filename);


/* 
    Read the header of the compressed file, construct the huffman tree
    Tasks:
    1. Read first byte of header => total number of different characters
    2. Skip the second byte of the header => 00000111 => body last character pad 3 zeros
    3. Read the entire header, until get same number of different characters then stop
       Need to mark that position? or simply transfer the file pointer is enough. 

*/
Tree* read_header(FILE*);


/*
    Decompress the body. 
    The fp_in is already at the body paragraph, do not rewind to the header position!!
    When the reading body reaches EOF, go to the second byte of the whole file,
    and discard the padding 0.
*/
void decompress_body(FILE* fp_in, FILE* fp_out, const Tree* tr);