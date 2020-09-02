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


Dictionary DictionaryDestroy(Dictionary d, int size){
    assert (d != NULL);

    // since the dictionary is full of list nodes
    // i will check if an entry is null or not
    // if not null, destroy that list node

    for (int i = 0; i < size; i++){
        if (d[i] != NULL){
            d[i] = ListNodeDestroy(d[i]);
        }
    }

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
