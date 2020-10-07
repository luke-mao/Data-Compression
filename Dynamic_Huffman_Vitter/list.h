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


// connection
ListNode GetPrev(ListNode);
ListNode GetNext(ListNode);
void ConnectAsPrev(ListNode cur, ListNode prev);
void ConnectAsNext(ListNode cur, ListNode next);


// List
List ListCreate(void);
List ListDestroy(List);
void ListInsert(List, ListNode);              // insert listnode into list, similar to insertion sort
void ListShow(List);


// the following two search for related tree node and list node
TreeNode GetTreeNode(ListNode);
ListNode GetListNode(List, TreeNode);


// for a given listnode, use its treenode to find its parent treenode,
// and return the listnode that encapsulate its parent treenode
ListNode FindParentListNode(ListNode LN);


// get the first element of the list, usually the listnode that packs the NYT treenode
ListNode GetListHead(List);


// reassign the list first element
void AssignListHead(List, ListNode);


#endif