#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "tree.h"
#include "util.h"
#include "stack.h"


Stack StackCreate(int size){
    assert(size > 0);

    Stack s = (Stack) malloc(sizeof(struct _Stack));
    assert(s != NULL);

    s->size = SIZE_FACTOR * size;
    s->count = 0;

    s->trns = (TreeNode*) malloc(s->size * sizeof(TreeNode));
    assert(s->trns != NULL);

    for (int i = 0; i < s->size; i++){
        s->trns[i] = NULL;
    }

    return s;
}


Stack StackDestroy(Stack s){
    assert(IsStackValid(s));

    free(s->trns);
    s->trns = NULL;

    free(s);
    s = NULL;
    return s;
}


// double the size
Stack StackIncreaseSize(Stack s){
    assert(IsStackValid(s));

    s->size *= 2;
    s->trns = (TreeNode*) realloc(s, s->size);
    assert(s->trns != NULL);

    return s;
}


bool IsStackValid(Stack s){
    return s != NULL && s->trns != NULL && s->size > 0 && s->count >= 0;
}

bool IsStackFull(Stack s){
    assert(IsStackValid(s));
    return s->size == s->count;
}


bool IsStackEmpty(Stack s){
    assert(IsStackValid(s));
    return s->count == 0;
}


void StackPush(Stack s, TreeNode trn){
    assert(IsStackValid(s));
    assert(IsTreeNodeValid(trn));
    assert(! IsStackFull(s));

    s->trns[s->count] = trn;
    s->count += 1;
    return;
}


TreeNode StackPop(Stack s){
    assert(IsStackValid(s));
    
    TreeNode trn = s->trns[s->count - 1];
    s->trns[s->count - 1] = NULL;
    s->count -= 1;

    return trn;
}


TreeNode StackTop(Stack s){
    assert(IsStackValid(s));
    return s->trns[s->count - 1];
}


int GetStackCount(Stack s){
    assert(IsStackValid(s));
    return s->count;
}