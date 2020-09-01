/* 
* This file follows the dynamic huffman vitter algorithm
* Follows the paper (Vitter, 1987), "Design and Analysis of Dynamic Huffman Codes"
* The implementation follows the pesudo code, 
* not his paper about the Pascal implementation (Vitter, 1989).
* So the execution time will be a bit longer than the optimized program in his second paper.
* Reference: 
* Vitter, J., 1989. Algorithm 673. ACM Transactions on Mathematical Software (TOMS), 15(2), pp.158-167.
* Vitter, J., 1987. Design and analysis of dynamic Huffman codes. Journal of the ACM (JACM), 34(4), pp.825-845.
*/


#ifndef _TREE_H_
#define _TREE_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

// so the leaf node has char c >= 0
// and internal node, as well as root and NYT, has char c < 0
#define ROOT_C -1
#define INTERNAL_NODE_C -2
#define NYT_C -3


// label decreases from 256
// use the ASCII table, 0-255, total 256
#define LABEL_START 256
#define ALPHABET_SIZE 256


// a typical node structure for the dynamic huffman tree
// as a binary tree, it has left and right child.
// in order to trace upwards, it has a parent pointer.
// in addition, the node should contain "label" and "occ" used in swap.
// and also use integer to represent the char.
// "int" data type for char, so we can use negative integer to represent 
// the root and all internal nodes, as well as NYT
struct _Node {
    int label;
    int occ;
    int c; 
    struct _Node *left;
    struct _Node *right;
    struct _Node *parent;
};


// define the pointer
typedef struct _Node *Node;


// dynamic huffman FGK tree structure, store the root and current NYT node. 
struct _Tree{
    Node root;
    Node NYT;
};


typedef struct _Tree *Tree;
typedef Node* NodeList;

// for each slide & increment process, need to traversal the tree to obtain a list of nodes 
// with sam eocc, and same type (leaf block or internal node block, exclude the root node)
struct _List{
    Node* list;
    int len;
    int occ;
    bool isleaf;
};
typedef struct _List List;


Node NodeCreate(int c, int label, int occ, Node left, Node right, Node parent);
Node NodeDestroy(Node);
void NodePrintCode(Node n);


Tree TreeCreate(void);      
Tree TreeDestroy(Tree);
void TreeShow(Tree);
void TreeUpdate(Tree, NodeList, int c);


NodeList NodeListCreate(void);
NodeList NodeListDestroy(NodeList);
void NodeListAddNode(NodeList, Node);
Node NodeListFindNode(NodeList, int c);


// functions for list
List ListCreate(int occ, bool isleaf);
List ListDestroy(List);
void ListInsert(List, Node);
void ListShow(List);
bool ListIsEmpty(List);


#endif 