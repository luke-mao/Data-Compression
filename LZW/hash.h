#ifndef _HASH_H_
#define _HASH_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// hash dictionary component
typedef int Index;

// 12 bits
#define SIZE_LIMIT 4096

// reserve 256 for EOF, and 257 for reflush dictionary
#define INDEX_EOR 256
#define INDEX_REFLUSH 257

// hash dictionary node and format
struct _Node{
    char* key;
    Index idx;
    struct _Node* next;
};
typedef struct _Node* Node;

struct _Dictionary{
    Index size;
    Index current_num;
    Node* nodes;
};
typedef struct _Dictionary* Dictionary;

// hash function
Index calculate_index(const char*);

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
Index dictionary_search(Dictionary, const char* key);
// insert
Index dictionary_insert(Dictionary d, const char* key, Index idx);
// update current_num


#endif 