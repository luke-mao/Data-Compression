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


// top functions
char* compression_create_output_filename(char* filename_in);
void compress_file_and_output(FILE* fp_in, FILE* fp_out);
void compression_status(char* name_in, char* name_out, FILE* fp_in, FILE* fp_out);
void FilePrintDigit(int* buffer_p, int* buffer_len_p, FILE* fp, int new_digit);
void FilePrintNode(int* buffer_p, int* buffer_len_p, FILE* fp, TreeNode trn);


// compression implementation
void TreeUpdateForFirstChar(Tree tr, int c);
void ListUpdateForFirstChar(List L, Tree tr);
void UpdateAndPrint(Tree tr, List L, Dictionary d, int* buffer_p, int* buffer_len_p, int c, FILE* fp);
void SlideAndIncrement(List, ListNode);
void SwapWithLeader(List, ListNode);


#endif 