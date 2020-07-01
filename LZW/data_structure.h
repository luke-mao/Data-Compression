#ifndef _DATA_STRUCTURE_H_
#define _DATA_STRUCTURE_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


// hash dictionary component
typedef int Index;          // max = 4096
typedef int CodeWord;       //  max  = 4096
typedef char* Key;          // key is a string

// 12 bits
#define SIZE_LIMIT 4096


// reserve 256 for EOF, and 257 for reflush dictionary
#define INDEX_EOR 256
#define INDEX_REFLUSH 257


//-----------hash dictionary functions------------
// hash dictionary node and format
struct _Node{
    Key k;
    CodeWord cw;
    struct _Node* next;
};
typedef struct _Node* Node;

struct _Dictionary{
    Index current_num;
    Node* nodes;
};
typedef struct _Dictionary* Dictionary;

// hash function
// during calculation we use long,
// but the output is in Code (int)
Index calculate_index(const char*);

// create
Dictionary dictionary_create(void);
// destroy
Dictionary dictionary_destroy(Dictionary);
// reset,  = destroy + create
Dictionary dictionary_reset(Dictionary);
// check if is full, if full, need reset
bool dictionary_is_full(const Dictionary);

// check if the key exist, 
// if exist return the index, if not return -1 
CodeWord dictionary_search(Dictionary, const Key k);
// insert
void dictionary_insert(Dictionary, const Key k, CodeWord cw);

// debug use
void dictionary_print(Dictionary);


// ------------Array definition----------------
struct _Array{
    Index current_num;
    char** nodes;
};

typedef struct _Array* Array;

// create
Array array_create(void);
// delete 
Array array_destroy(Array);
// reset = destroy + create
Array array_reset(Array);
// check if full
bool array_is_full(const Array);

// search using index, return the string
Key array_search(Array, const Index, Key prev);
// insert
void array_insert(Array, const Key k);

// debug use
void array_print(const Array);

#endif 