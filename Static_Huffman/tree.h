#ifndef _TREE_H_
#define _TREE_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define ROOT_C -2
#define INTERNAL_NODE_C -3



struct _TreeNode{
    int c;
    int occ;
    struct _TreeNode *left;
    struct _TreeNode *right;
};

typedef struct _TreeNode *TreeNode;

struct _Tree{
    TreeNode root;
};

typedef struct _Tree *Tree;


TreeNode TreeNodeCreate(int c, int occ, TreeNode left, TreeNode right);
TreeNode TreeNodeDelete(TreeNode);


Tree TreeCreate(void);
Tree TreeDestroy(Tree);


bool IsRootNode(TreeNode);
bool IsInternalNode(TreeNode);
bool IsLeafNode(TreeNode);

int GetC(TreeNode);
int GetOcc(TreeNode);

void SetOcc(TreeNode trn, int occ);
int SumTwoOcc(TreeNode trn1, TreeNode trn2);

void ConnectAsLeftChild(TreeNode child, TreeNode parent);
void ConnectAsRightChild(TreeNode child, TreeNode parent);
void ConnectAsChild(TreeNode child, TreeNode parent, bool isRightChild);


#endif 