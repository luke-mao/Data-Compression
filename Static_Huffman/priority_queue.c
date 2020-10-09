#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "tree.h"
#include "priority_queue.h"
#include "util.h"


PriorityQueue PriorityQueueCreate(void){
    PriorityQueue pq = (PriorityQueue) malloc(sizeof(struct _PriorityQueue));
    assert(pq != NULL);

    pq->size = ASCII_SIZE * SIZE_FACTOR;
    pq->count = 0;

    pq->queue = (TreeNode*) malloc(pq->size * sizeof(TreeNode));
    assert(pq->queue != NULL);

    for (int i = 0; i < pq->size; i++){
        pq->queue[i] = NULL;
    }

    return pq;
}


PriorityQueue PriorityQueueDestroy(PriorityQueue pq){
    assert(IsPriorityQueueValid(pq));

    // before this destroy, the tree should be destroyed already
    free(pq->queue);
    pq->queue = NULL;
    free(pq);
    pq = NULL;

    return pq;
}


// double the size every time
void PriorityQueueIncreaseSize(PriorityQueue pq){
    assert(IsPriorityQueueValid(pq));
    assert(IsPriorityQueueFull(pq));

    pq->size *= 2;
    pq->queue = (TreeNode*) realloc(pq->queue, pq->size);
    assert(pq != NULL);

    for (int i = pq->count; i < pq->size; i++){
        pq->queue[i] = NULL;
    }

    return;
}


int GetSize(PriorityQueue pq){
    assert(IsPriorityQueueValid(pq));
    return pq->size;
}


int GetCount(PriorityQueue pq){
    assert(IsPriorityQueueValid(pq));
    return pq->count;
}


bool IsPriorityQueueValid(PriorityQueue pq){
    return pq != NULL && pq->size >= 0 && pq->count >= 0 && pq->queue != NULL;
}


bool IsPriorityQueueFull(PriorityQueue pq){
    assert(pq != NULL);
    return pq->count == pq->size;
}


void IncreaseCount(PriorityQueue pq){
    assert(IsPriorityQueueValid(pq));
    pq->count += 1;
    
    if (IsPriorityQueueFull(pq)){
        PriorityQueueIncreaseSize(pq);
    }

    return;
}


void DecreaseCount(PriorityQueue pq){
    assert(IsPriorityQueueValid(pq));
    pq->count -= 1;
    return;
}


// insertion sort: place trn in decreasing trend of trn->occ
void PriorityQueueInsertTreeNode(PriorityQueue pq, TreeNode trn){
    assert(IsPriorityQueueValid(pq));
    assert(IsTreeNodeValid(trn));


    // here uses linear scan to make life easier
    int idx = 0;
    while (idx <= GetCount(pq) && IsOccSmaller(trn, pq->queue[idx])){
        idx += 1;
    }

    // now the new trn should be inserted into "idx" this index position
    // increase count first, so that if necessary, memory space is enough
    IncreaseCount(pq);

    // move the treenodes from idx+1 to end one space further
    for (int i = idx+1; i < GetCount(pq); i++){
        pq->queue[i] = pq->queue[i-1];
    }

    pq->queue[idx] = trn;

    return;
}


TreeNode PriorityQueueGetTreeNode(PriorityQueue pq){
    assert(IsPriorityQueueValid(pq));

    // get the smallest occ treenode
    // return the node from the end of the queue
    DecreaseCount(pq);
    return pq->queue[pq->count];
}
