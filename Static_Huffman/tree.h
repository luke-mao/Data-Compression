#ifndef _TREE_H_
#define _TREE_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ROOT_NODE_C -2
#define INTERNAL_NODE_C -3


struct _TreeNode{
    int c;
    int occ;
    struct _TreeNode *left;
    struct _TreeNode *right;
    struct _TreeNode *parent;
};

typedef struct _TreeNode *TreeNode;

struct _Tree{
    TreeNode root;
};

typedef struct _Tree *Tree;

// create the treenode with c and occ only, leave connection to be done later
TreeNode TreeNodeCreate(int c, int occ);
TreeNode TreeNodeDelete(TreeNode);


Tree TreeCreate(TreeNode root);
Tree TreeDestroy(Tree);


bool IsTreeNodeValid(TreeNode);
bool IsRootNode(TreeNode);
bool IsInternalNode(TreeNode);
bool IsLeafNode(TreeNode);
bool IsOccSmaller(TreeNode trn1, TreeNode trn2);

bool IsLeftChild(TreeNode child, TreeNode parent);

int GetC(TreeNode);
int GetOcc(TreeNode);

void SetOcc(TreeNode trn, int occ);
int SumTwoOcc(TreeNode trn1, TreeNode trn2);

void ConnectAsLeftChild(TreeNode child, TreeNode parent);
void ConnectAsRightChild(TreeNode child, TreeNode parent);
void ConnectAsChild(TreeNode child, TreeNode parent, bool isRightChild);

void ResetInternalNodeToRootNode(TreeNode);

int TreeNodeGetDepth(TreeNode trn);

#endif 