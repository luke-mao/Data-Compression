#ifndef _STACK_H_
#define _STACK_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "util.h"
#include "tree.h"


struct _Stack{
    int size;
    int count;
    TreeNode* trns;
};

typedef struct _Stack *Stack;

Stack StackCreate(int size);
Stack StackDestroy(Stack);
Stack StackIncreaseSize(Stack);

bool IsStackValid(Stack);
bool IsStackFull(Stack);
bool IsStackEmpty(Stack);

void StackPush(Stack, TreeNode);
TreeNode StackPop(Stack);
TreeNode StackTop(Stack);

int GetStackCount(Stack);

#endif 