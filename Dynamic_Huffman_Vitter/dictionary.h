/*
* In order to quickly find the corresponding list node for a specific char
* we need an array structure, with size 256 as in ascii table
* the array will have 256 list nodes
* call it dictionary, use listnode->n->c to locate
*/



#ifndef _ARRAY_H_
#define _ARRAY_H_


#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "list.h"


// define the structure
typedef ListNode* Dictionary; 


// functions
Dictionary DictionaryCreate(int size);
Dictionary DictionaryDestroy(Dictionary);
ListNode DictionarySearch(int c);           // return either NULL, or the list node
void DictionaryInsert(ListNode);        


#endif 