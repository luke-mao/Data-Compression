#ifndef _FILE_H_
#define _FILE_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "util.h"
#include "codeword.h"

FILE* OpenFileWithMode(char* filename, char* mode);
FILE* CloseFile(FILE* fp);

int GetOneBit(FILE* fp, int* buffer_p, int* unread_num_p, int* buffer_next_p);
int GetOneByte(FILE* fp, int* buffer_p, int* unread_num_p, int* buffer_next_p);
int GetOneByteSimple(FILE* fp);         // getc

void PrintOneBit(FILE* fp, int* buffer_p, int* buffer_len_p, int this_bit);
void PrintOneByte(FILE* fp, int* buffer_p, int* buffer_len_p, int this_byte);

int PadByte(FILE* fp, int* buffer_p, int* buffer_len_p);

void PrintFirstByteEmpty(FILE* fp);
void RePrintFirstByteWithPadNumber(FILE* fp, int pad_num);

int ReadFirstByteGetPadNumber(FILE* fp);

void PrintCodeWord(FILE* fp, int* buffer_p, int* buffer_len_p, CodeWordNode cwn);

#endif 