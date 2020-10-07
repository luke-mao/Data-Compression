/*
* In order to quickly find the corresponding list node for a specific char
* we need an array structure, with size 256 as in ascii table
* the array will have 256 list nodes
* call it dictionary, use listnode->n->c to locate
*/


#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_


#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "list.h"


// an array of listnodes, size usually = 256
typedef ListNode* Dictionary; 


// create and destroy, size usually = 256
Dictionary DictionaryCreate(int size);
Dictionary DictionaryDestroy(Dictionary, int size);


// search, return array[c] or NULL
ListNode DictionarySearch(Dictionary, int c);    


// insert new listnode into position based on listnode->trn->c
void DictionaryInsert(Dictionary, ListNode);        


// debug use: print the dictionary
void DictionaryShow(Dictionary, int size);

#endif 