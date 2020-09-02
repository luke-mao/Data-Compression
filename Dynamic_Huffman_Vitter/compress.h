/*
* Compression implementation:
* Include file handling, creating new file
* and the compression details
*/


#ifndef _COMPRESS_H_
#define _COMPRESS_H_


#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "list.h"
#include "dictionary.h"


// function
char* compression_create_output_filename(char* filename_in);
void compress_file_and_output(FILE* fp_in, FILE* fp_out);
void compression_status(char* name_in, char* name_out, FILE* fp_in, FILE* fp_out);
void compress_update_everything(Tree tr, List L, Dictionary d, int c);

#endif 