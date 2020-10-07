#ifndef _FILE_H_
#define _FILE_H_


#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "list.h"


// open and close the file
FILE* open_the_file(char* filename, char* mode);
FILE* close_the_file(FILE* fp);


// file print: print one bit, one byte or the node path
void FilePrintDigit(int* buffer_p, int* buffer_len_p, FILE* fp, int new_digit);
void FilePrintByte(int* buffer_p, int* buffer_len_p, FILE* fp, int new_byte);
void FilePrintNodePath(int* buffer_p, int* buffer_len_p, FILE* fp, TreeNode trn);


// at the beginning of the file,
// print one empty byte that will store the number of bits pad at the end 
void FilePrintEmptyByte(FILE* fp);
// at the end of file, pad the last byte if necesary and return the number
int FilePrintPad(int* buffer_p, int* buffer_len_p, FILE* fp);
// print the number of bytes pad, at the first byte of output file
void FileRePrintFirstByte(int, FILE* fp);


#endif 
