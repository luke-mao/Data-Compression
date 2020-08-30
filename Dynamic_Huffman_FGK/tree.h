#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define ROOT_C -1
#define INTERNAL_NODE_C -2
#define NYT_C -3
#define LABEL_START 256
#define ALPHABET_SIZE 256


struct _Node {
    int label;
    int occ;
    int c;      // for leaf nodes represent the char
    struct _Node *left;
    struct _Node *right;
    struct _Node *parent;
};


typedef struct _Node *Node;

Node NodeCreate(int c, int label, int occ, Node left, Node right, Node parent);
Node NodeDestroy(Node);

////////////////////////////////////////////

// the tree structure has the root and current NYT node
struct _Tree{
    Node root;
    Node NYT;
};

typedef struct _Tree *Tree;


Tree TreeCreate(void);      // also have prepared the root node
Tree TreeDestroy(Tree);
void TreeShow(Tree);
void TreeUpdate(Tree, NodeList, Node NYT, int);

///////////////////////////////////////////

typedef Node* NodeList;

NodeList NodeListCreate(void);
NodeList NodeListDestroy(NodeList);
void NodeListAddNode(NodeList, Node);
Node NodeListFindNode(NodeList, int c);
