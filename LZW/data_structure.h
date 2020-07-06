#ifndef _DATA_STRUCTURE_H_
#define _DATA_STRUCTURE_H_

/*
This part defines the data structure used for LZW algorithm, 
mainly the hash dictionary used in compression, 
and the array used in decompression.

User can self defined the length of the codeword, 
commonly 12, but can increase to 15, 16 if necessary. 
Longer codeword can increase the compression ration, 
however it also fails to reflect more local characteristics. 
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


// define the fundamental amounts
#define BITS 14                         // length for the codeword
#define SIZE_LIMIT (int) pow(2, BITS)   // array size
#define CAPACITY_FACTOR 2               // dictionary size = array size * factor, 
                                        //      larger size to reduce hash collision


// hash dictionary component
typedef long Index;          // for 12 bits, max = 4096
typedef long CodeWord;       // for 12 bits, max  = 4096
typedef char* Key;          // key is a string


// reserve 256 for EOF, and 257 for reflush dictionary
#define INDEX_EOF 256
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
// input the string and calculate the hash
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
CodeWord dictionary_insert(Dictionary, const Key k);

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
Key array_search(Array, const Index);
// insert
Index array_insert(Array, const Key k);
// check if a codeword is in the table
bool array_has_this_codeword(Array, const Index);

// debug use
void array_print(const Array);


#endif 