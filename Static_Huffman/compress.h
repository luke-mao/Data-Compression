#ifndef _COMPRESS_H_
#define _COMPRESS_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"
#include "priority_queue.h"
#include "frequency_table.h"
#include "file.h"
#include "util.h"
#include "codeword.h"


// first byte = number of bits pad
// second byte = number of symbols used !! so that the stack can rebuild the tree
// then the header: tree
// then the main body


char* CreateCompressedFileName(char* filename);

void compression_status(char* name_in, char* name_out, FILE* fp_in, FILE* fp_out);

FreqTable ReadFileCountFrequency(FILE* fp);

PriorityQueue UseFreqTableProducePriorityQueue(FreqTable);

Tree UsePriorityQueueProduceTree(PriorityQueue);

CodeWord UseTreeProduceCodeWord(Tree tr);

void PrintCompressionTree(FILE* fp, Tree tr);

// return the pad number
int ReadFilePrintCompression(FILE* fp_in, FILE* fp_out, CodeWord cw);


#endif 