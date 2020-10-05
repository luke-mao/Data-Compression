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


// functions related to tree and tree node
TreeNode TreeNodeCreate(int c, int occ, TreeNode left, TreeNode right, TreeNode parent);
TreeNode TreeNodeDestroy(TreeNode);

Tree TreeCreate(void);
Tree TreeDestroy(Tree);

void TreeShow(Tree);

#endif 