/*
* Update functions.
* Follow Vitter's paper to finalize the update function. 
* So when a symbol is extracted from the input file, either occ count increase by 1,
* or the NYT symbol is splitted into two symbols.
* For each procedure, update and balance the tree from bottom up. 
* Main process is done through SlideAndIncrement, 
* where both the list and the tree are reconstructed to maintain the balance of the tree,
* and the "leaf nodes precede internal nodes" statement in Vitter's paper.
*/


#ifndef _UPDATE_H_
#define _UPDATE_H_


#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "list.h"
#include "dictionary.h"


// Update the tree and the list after reading the first char
void TreeUpdateForFirstChar(Tree tr, int c);
ListNode ListUpdateForFirstChar(List L, Tree tr);


// top function for update
void UpdateTreeAndList(Tree tr, List L, Dictionary d, ListNode LN_p, int c);


// slide the block of internal nodes / leaf nodes, and increase count from bottom up
void SlideAndIncrement(List, ListNode*);


// when an existing symbol is input again, swap its node with the leader of the block,
// then do the slide and increment. 
void SwapWithLeader(List, ListNode);


#endif 