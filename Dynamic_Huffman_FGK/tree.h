// dynamic huffman FGK tree construction
// Each time a char is inputed, the tree first print out something, and then updates


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


// define the pointers
typedef struct _Tree *Tree;


// alphabet node list, size = 256 for ascii table
// only used in compression to increase execution speed
// in stead of trasversing the tree every time to find the node of a particular char
typedef Node* NodeList;


// node related create and destroy functions
Node NodeCreate(int c, int label, int occ, Node left, Node right, Node parent);
Node NodeDestroy(Node);
// function to output the code for this node, use recursion to trace upwards
void NodePrintCode(Node n, FILE* fp_out, char* out_c, int* out_c_num);


// tree related functions
Tree TreeCreate(void);      
Tree TreeDestroy(Tree);
void TreeShow(Tree);
// update the tree: each time swap with the max label node in the block
void TreeUpdate(Tree, NodeList, int c, FILE* fp_out, char *out_c, int* out_c_num);
void TreeUpdateFunction(Tree tr, NodeList ndlist, Node n);


// node list related functions
NodeList NodeListCreate(void);
NodeList NodeListDestroy(NodeList);
void NodeListAddNode(NodeList, Node);
Node NodeListFindNode(NodeList, int c);


#endif 
