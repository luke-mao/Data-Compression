#ifndef _DECOMPRESS_H_
#define _DECOMPRESS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"
#include "priority_queue.h"
#include "frequency_table.h"
#include "file.h"
#include "util.h"
#include "codeword.h"
#include "stack.h"

// remove .huff suffix, add dehuff_ prefix
char* CreateDecompressedFileName(char* filename);

void decompression_status(char* name_in, char* name_out, FILE* fp_in, FILE* fp_out);

int ReadFileGetPadNumber(FILE* fp);
int ReadFileGetCharCount(FILE* fp);

Tree ReadHeaderProduceTree(FILE* fp, int char_count);

void ReadFilePrintDecompression(FILE* fp_in, FILE* fp_out, Tree tr, int pad_num);


#endif 