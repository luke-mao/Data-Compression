#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "tree.h"


TreeNode TreeNodeCreate(int c, int occ){
    assert(c >= INTERNAL_NODE_C);
    assert(occ >= 0);

    TreeNode trn = (TreeNode) malloc(sizeof(struct _TreeNode));
    assert(trn != NULL);

    trn->c = c;
    trn->occ = occ;
    trn->left = NULL;
    trn->right = NULL;
    trn->parent = NULL;

    return trn;
}


// recursion
TreeNode TreeNodeDelete(TreeNode trn){
    if (trn != NULL){
        trn->left = TreeNodeDelete(trn->left);
        trn->right = TreeNodeDelete(trn->right);

        free(trn);
        trn = NULL;
    }

    return trn;
}


// for now, set the root to null
Tree TreeCreate(TreeNode root){
    assert(IsTreeNodeValid(root));
    assert(IsRootNode(root));

    Tree tr = (Tree) malloc(sizeof(struct _Tree));
    assert(tr != NULL);

    tr->root = root;
    return tr;
}


Tree TreeDestroy(Tree tr){
    assert(tr != NULL);

    tr->root = TreeNodeDelete(tr->root);
    
    free(tr);
    tr = NULL;
    
    return tr;
}


bool IsTreeNodeValid(TreeNode trn){
    return trn != NULL && trn->c >= INTERNAL_NODE_C && trn->occ >= 0;
}


bool IsRootNode(TreeNode trn){
    assert(IsTreeNodeValid(trn));
    return trn->c == ROOT_NODE_C;
}


bool IsInternalNode(TreeNode trn){
    assert(IsTreeNodeValid(trn));
    return trn->c == INTERNAL_NODE_C;
}


bool IsLeafNode(TreeNode trn){
    assert(IsTreeNodeValid(trn));
    return trn->c >= 0 && trn->left == NULL && trn->right == NULL;
}


// true if trn1 has smaller occ than trn2
bool IsOccSmaller(TreeNode trn1, TreeNode trn2){
    assert(IsTreeNodeValid(trn1));
    assert(IsTreeNodeValid(trn2));
    return trn1->occ < trn2->occ;
}


bool IsLeftChild(TreeNode child, TreeNode parent){
    assert(IsTreeNodeValid(child) && IsTreeNodeValid(parent));
    assert(child->parent == parent);

    return parent->left == child;
}


int GetC(TreeNode trn){
    assert(IsTreeNodeValid(trn));
    return trn->c;    
}


int GetOcc(TreeNode trn){
    assert(IsTreeNodeValid(trn));
    return trn->occ;
}


void SetOcc(TreeNode trn, int occ){
    assert(IsTreeNodeValid(trn));
    trn->occ = occ;
    return;
}


int SumTwoOcc(TreeNode trn1, TreeNode trn2){
    assert(IsTreeNodeValid(trn1));
    assert(IsTreeNodeValid(trn2));

    return trn1->occ + trn2->occ;
}


void ConnectAsLeftChild(TreeNode child, TreeNode parent){
    assert(IsTreeNodeValid(child) && IsTreeNodeValid(parent));
    parent->left = child;
    child->parent = parent;
    return;
}


void ConnectAsRightChild(TreeNode child, TreeNode parent){
    assert(IsTreeNodeValid(child) && IsTreeNodeValid(parent));
    parent->right = child;
    child->parent = parent;
    return;
}


void ConnectAsChild(TreeNode child, TreeNode parent, bool isRightChild){
    assert(IsTreeNodeValid(child) && IsTreeNodeValid(parent));
    
    if (isRightChild){
        ConnectAsRightChild(child, parent);
    }
    else{
        ConnectAsLeftChild(child, parent);
    }

    return;
}


void ResetInternalNodeToRootNode(TreeNode trn){
    assert(IsTreeNodeValid(trn));
    assert(IsInternalNode(trn));

    trn->c = ROOT_NODE_C;
    return;
}


int TreeNodeGetDepth(TreeNode trn){
    assert(IsTreeNodeValid(trn));

    // while loop to trace upwards
    int depth = 0;
    TreeNode current = trn;
    while (! IsRootNode(current)){
        current = current->parent;
        depth += 1;
    }

    return depth;
}