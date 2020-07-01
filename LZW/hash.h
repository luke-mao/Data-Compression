#ifndef _HASH_H_
#define _HASH_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// hash dictionary component
typedef char* Key;
typedef unsigned int Index;


// hash dictionary node and format
struct _Node{
    Key key;
    Index i;
    struct _Node* next;
};
typedef struct _Node Node;

struct _Dic{
    Index threshold;
    Index size;
    Index current_num;
    Node** nodes;
};
typedef struct _Dic* Dictionary;


// hash table functions
// create
Dictionary dictionary_create(void);
// destroy
Dictionary dictionary_destroy(Dictionary);
// reset,  = destroy + create
Dictionary dictionary_reset(Dictionary);
// check if is full, if full, need reset
bool dictionary_is_full(Dictionary);

// check if the key exist, 
// if exist return the index, if not return -1 
Index dictionary_is_key_exist(Dictionary, const char* key);
// insert
Index dictionary_insert(Dictionary, const char* key);


#endif 