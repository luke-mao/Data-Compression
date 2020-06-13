#ifndef FILE_H_
#define FILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



// open the file, exit if error during opening
FILE* open_file(const char* file_name);

// close the file
FILE* close_file(FILE* ffp);





#endif