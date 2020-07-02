#ifndef FILE_H_
#define FILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// open file for read, mode = rb
FILE* open_file_for_read(const char* file_name);

// open file for write, mode = wb         
FILE* open_file_for_write(const char* file_name);   

// close file
FILE* close_file(FILE* fp);

// get the file size, return long
long file_size(FILE* fp);

#endif