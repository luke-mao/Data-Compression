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


char* CreateCompressedFileName(char* filename);

void compression_status(char* name_in, char* name_out, FILE* fp_in, FILE* fp_out);

FreqTable ReadFileCountFrequency(FILE* fp);

PriorityQueue UseFreqTableProducePriorityQueue(FreqTable);

Tree UsePriorityQueueProduceTree(PriorityQueue);

CodeWord UseTreeProduceCodeWord(Tree tr);

void ReadFileOutputCompression(FILE* fp_in, FILE* fp_out, CodeWord cw);


#endif 