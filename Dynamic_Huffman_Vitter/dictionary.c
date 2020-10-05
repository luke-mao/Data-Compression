#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "tree.h"
#include "list.h"
#include "dictionary.h"


Dictionary DictionaryCreate(int size){
    assert(size >= 1);
    Dictionary d = (Dictionary) malloc(size * sizeof(ListNode));
    assert(d != NULL);

    // null every entry
    for (int i = 0; i < size; i++){
        d[i] = NULL;
    }

    return d;
}


// before destroying, the tree and list has been already free
// so only free the outermost structure is enough
Dictionary DictionaryDestroy(Dictionary d, int size){
    assert (d != NULL);

    free(d);
    d = NULL;
    return d;
}


ListNode DictionarySearch(Dictionary d, int c){
    assert(d != NULL);
    assert(c >= 0);
    return d[c];
}          


void DictionaryInsert(Dictionary d, ListNode listn){
    assert(d != NULL);
    assert(listn != NULL);
    assert(listn->trn->c >= 0);

    d[listn->trn->c] = listn;
    return;
}        
