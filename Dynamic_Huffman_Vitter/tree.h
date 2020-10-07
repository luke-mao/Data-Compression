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


// so the leaf node has char c >= 0
// and internal node, as well as root and NYT, has char c < 0
#define ROOT_C -1
#define INTERNAL_NODE_C -2
#define NYT_C -3


// ASCII table size = 256
#define ASCII_SIZE 256


// tree node
// due to implicit numbering, we do not need to use "label" as in the FGK algorithm
struct _TreeNode{
    int c;
    int occ;
    struct _TreeNode *left;
    struct _TreeNode *right;
    struct _TreeNode *parent;
};


// define the pointer
typedef struct _TreeNode *TreeNode;


// define the tree
struct _Tree{
    TreeNode root;
    TreeNode NYT;
};


// define the pointer
typedef struct _Tree *Tree;


// create and destroy the treenode
TreeNode TreeNodeCreate(int c, int occ, TreeNode left, TreeNode right, TreeNode parent);
TreeNode TreeNodeDestroy(TreeNode);


// create and destroy the tree
Tree TreeCreate(void);
Tree TreeDestroy(Tree);


// some check functions for the tree
bool IsRightChild(TreeNode child, TreeNode parent);
bool IsRootNode(TreeNode trn);
bool IsInternalNode(TreeNode trn);
bool IsLeafNode(TreeNode trn);
bool IsNYTNode(TreeNode trn);
bool IsSymbolNode(TreeNode trn);
bool IsNYTSubling(TreeNode trn);


// increase occ by 1 for the treenode
void IncreaseOcc(TreeNode trn);


// construct the tree: connect to parent, right, left,
// or input the right/left boolean variable and connect
void ConnectAsParent(TreeNode child, TreeNode parent);
void ConnectAsRightChild(TreeNode child, TreeNode parent);
void ConnectAsLeftChild(TreeNode child, TreeNode parent);
void ConnectAsChild(TreeNode child, TreeNode parent, bool isRightChild);


// Get the root node or NYT node
TreeNode GetRoot(Tree tr);
TreeNode GetNYT(Tree tr);


// Get the information from the treenode
int GetC(TreeNode trn);
int GetOcc(TreeNode trn);
TreeNode GetRight(TreeNode trn);
TreeNode GetLeft(TreeNode trn);
TreeNode GetParent(TreeNode trn);


// when a new symbol is introduced,
// reset the previous NYT node to internal node,
// and reset the NYT pointer of the tree
void ResetToInternalNode(TreeNode trn);
void UpdateNYT(Tree tr, TreeNode NYT);


// debug use: in-order traversal of the tree
void TreeShow(Tree);


#endif 