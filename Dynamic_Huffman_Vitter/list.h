/*
* In order to maintain the implicit numbering, 
* use a linked list structure here !!
* Each node are listed in increasing order of occ.
* And for the same occ, leaf nodes always precede the internal nodes.
* Structure: dummy head + double linked 
*/


#ifndef _LIST_H_
#define _LIST_H_


#include <stdio.h>
#include <stdlib.h>
#include "tree.h"


// define the node structure
// double link
struct _ListNode{
    TreeNode trn;
    struct _ListNode *prev;
    struct _ListNode *next;
};


// define the pointer
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
ListNode ListGetFromTreeNode(List, TreeNode);


#endif