#ifndef _FILE_H_
#define _FILE_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const int power_of_2[] = {1, 2, 4, 8, 16, 32, 64, 128, 256};


FILE* OpenFileWithMode(char* filename, char* mode);
FILE* CloseFile(FILE* fp);

int GetOneBit(FILE* fp, int* buffer_p, int* unread_num_p, int* buffer_next_p);
int GetOneByte(FILE* fp, int* buffer_p, int* unread_num_p, int* buffer_next_p);

void PrintOneBit(FILE* fp, int* buffer_p, int* buffer_len_p, int this_bit);
void PrintOneByte(FILE* fp, int* buffer_p, int* buffer_len_p, int this_byte);

int PadLastByte(FILE* fp, int* buffer_p, int* buffer_len_p);

void PrintFirstByteEmpty(FILE* fp);
void RePrintFirstByteWithPadNumber(FILE* fp, int pad_num);


#endif 