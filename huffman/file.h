#ifndef FILE_H_
#define FILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


FILE* open_file(const char* file_name);         // "rb"
FILE* open_file_write(const char* file_name);   // "wb"
FILE* close_file(FILE* fp);
long file_size(FILE* fp);


#endif