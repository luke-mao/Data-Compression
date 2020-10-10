#ifndef _CODEWORD_H_
#define _CODEWORD_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"
#include "priority_queue.h"
#include "frequency_table.h"
#include "util.h"
#include "file.h"


struct _CodeWordNode{
    int c;
    int bit_num;        // total number of bits
    int byte_num;        // total number of bytes
    int *s;
};

typedef struct _CodeWordNode *CodeWordNode;

struct _CodeWord{
    int size;
    CodeWordNode *list;
};

typedef struct _CodeWord *CodeWord;


CodeWordNode CodeWordNodeCreate(TreeNode);
CodeWordNode CodeWordNodeDestroy(CodeWordNode);

void CodeWordNodeShow(CodeWordNode);

CodeWord CodeWordCreate(int size);
CodeWord CodeWordDestroy(CodeWord);

void CodeWordInsertNode(CodeWord, CodeWordNode);

CodeWordNode CodeWordGetNode(CodeWord, int c);

void CodeWordShow(CodeWord);

void PrintCodeWordToFile(FILE* fp, int* buffer_p, int* buffer_len_p, CodeWordNode cwn);

bool IsCodeWordNodeValid(CodeWordNode cwn);


#endif 