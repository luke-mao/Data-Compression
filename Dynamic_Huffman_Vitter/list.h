/*
* In order to maintain the implicit numbering, 
* use a linked list structure here !!
* Each node are listed in increasing order of occ.
* And for the same occ, leaf nodes always precede the internal nodes.
* A linked list node contains a tree node, and a pointer to next linked list node.
*/


#ifndef _LIST_H_
#define _LIST_H_


#include <stdio.h>
#include <stdlib.h>
#include "tree.h"


// define the node structure
// need two pointers, one for prev, one for next
struct _ListNode{
    TreeNode trn;
    struct _ListNode *prev;
    struct _ListNode *next;
};


// define the pointer
// the linked list will have a dummy head structure
typedef struct _ListNode *ListNode;
typedef struct _ListNode *List;


// ListNode
ListNode ListNodeCreate(TreeNode);      // just include the treenode, with a null next pointer
ListNode ListNodeDestroy(ListNode);     // recursion, delete this node and all next nodes


// List
List ListCreate(void);
List ListDestroy(List);
void ListInsert(List, ListNode);              // insert listnode into list, similar to insertion sort
void ListShow(List);


// after the first char is inserted into the tree
// update the list based on the tree
void ListInitialUpdate(List, Tree);


// most important function in Vitter's algorithm
// slide and increment: one step only
void SlideAndIncrement(List, ListNode);

// swap this node with the leader of the corresponding block
void SwapWithLeader(List, ListNode);

#endif