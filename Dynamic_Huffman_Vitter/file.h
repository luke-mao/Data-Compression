#ifndef _FILE_H_
#define _FILE_H_


#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "list.h"


// file print
void FilePrintDigit(int* buffer_p, int* buffer_len_p, FILE* fp, int new_digit);
void FilePrintByte(int* buffer_p, int* buffer_len_p, FILE* fp, int new_byte);
void FilePrintNodePath(int* buffer_p, int* buffer_len_p, FILE* fp, TreeNode trn);


// compression: first byte records the num of bits pad
//              the last byte is pad
void FilePrintEmptyByte(FILE* fp);
void FileRePrintFirstByte(int, FILE* fp);
int FilePrintPad(int* buffer_p, int* buffer_len_p, FILE* fp);


#endif 
