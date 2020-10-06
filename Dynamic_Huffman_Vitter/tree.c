#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "tree.h"


void TreeShowFunction(TreeNode);



TreeNode TreeNodeCreate(int c, int occ, TreeNode left, TreeNode right, TreeNode parent){
    TreeNode trn = (TreeNode) malloc(sizeof(struct _TreeNode));
    assert(trn != NULL);

    trn->c = c;
    trn->occ = occ;
    trn->left = left;
    trn->right = right;
    trn->parent = parent;

    return trn;
}


// recursive delete children first, then trace upwards
TreeNode TreeNodeDestroy(TreeNode trn){
    if (trn != NULL){
        TreeNodeDestroy(trn->left);
        TreeNodeDestroy(trn->right);

        free(trn);
        trn = NULL;
    }

    return trn;
}


Tree TreeCreate(void){
    Tree tr = (Tree) malloc(sizeof(struct _Tree));
    assert(tr != NULL);

    // create the initial root and NYT = root
    tr->root = TreeNodeCreate(ROOT_C, 0, NULL, NULL, NULL);
    tr->NYT = tr->root;

    return tr;
}


Tree TreeDestroy(Tree tr){
    assert(tr != NULL);

    TreeNodeDestroy(tr->root);
    free(tr);
    tr = NULL;
    return tr;
}


// in-order traversal
void TreeShow(Tree tr){
    assert(tr != NULL);

    printf("Tree: ");
    TreeShowFunction(tr->root);
    printf("\n");
    return;
}


void TreeShowFunction(TreeNode trn){
    if (trn != NULL){
        // in-order traversal
        // show left first
        TreeShowFunction(trn->left);

        // show myself
        if (trn->c >= 0){
            printf("(%c-%d, %d) ", trn->c, trn->c, trn->occ);
        }
        else if (trn->c == ROOT_C){
            printf("(Root, %d) ", trn->occ);
        }
        else if (trn->c == NYT_C){
            printf("(NYT, %d) ", trn->occ);
        }
        else{
            // internal node
            printf("(Internal, %d) ", trn->occ);
        }

        // show right
        TreeShowFunction(trn->right);
    }

    return;
}

bool IsRightChild(TreeNode child, TreeNode parent){
    assert(child != NULL && parent != NULL);
    assert(child->parent == parent);

    if (parent->right == child){
        return true;
    }
    else{
        return false;
    }
}


// just root tag there is fine
bool IsRootNode(TreeNode trn){
    assert(trn != NULL);
    return trn->c == ROOT_C;
}


bool IsInternalNode(TreeNode trn){
    assert(trn != NULL);
    return trn->c == INTERNAL_NODE_C && (trn->left != NULL || trn->right != NULL);
}


// no child node is enough, cause NYT, root can also be leaf node
bool IsLeafNode(TreeNode trn){
    assert(trn != NULL);
    return trn->left == NULL && trn->right == NULL;
}


bool IsNYTNode(TreeNode trn){
    assert(trn != NULL);
    return trn->c == NYT_C && trn->left == NULL && trn->right == NULL;
}


bool IsSymbolNode(TreeNode trn){
    assert(trn != NULL);
    return IsLeafNode(trn) && trn->c >= 0;
}


bool IsNYTSubling(TreeNode trn){
    assert(trn != NULL);
    // must be the right child
    return IsRightChild(trn, trn->parent) && IsNYTNode(trn->parent->left);
}

void IncreaseOcc(TreeNode trn){
    assert(trn != NULL);
    assert(! IsNYTNode(trn));
    
    trn->occ += 1;
    return;
}


int GetOcc(TreeNode trn){
    assert(trn != NULL && trn->occ >= 0);
    return trn->occ;
}


void ConnectAsParent(TreeNode child, TreeNode parent){
    assert(child != NULL && parent != NULL);
    child->parent = parent;
    return;
}


void ConnectAsRightChild(TreeNode child, TreeNode parent){
    assert(child != NULL && parent != NULL);
    parent->right = child;
    return;
}


void ConnectAsLeftChild(TreeNode child, TreeNode parent){
    assert(child != NULL && parent != NULL);
    parent->left = child;
    return;
}


TreeNode GetRoot(Tree tr){
    assert(tr != NULL);
    return tr->root;
}

TreeNode GetNYT(Tree tr){
    assert(tr != NULL);
    return tr->NYT;
}


TreeNode GetRight(TreeNode trn){
    assert(trn != NULL);
    return trn->right;
}


TreeNode GetLeft(TreeNode trn){
    assert(trn != NULL);
    return trn->left;
}


TreeNode GetParent(TreeNode trn){
    assert(trn != NULL);
    return trn->parent;
}


void UpdateNYT(Tree tr, TreeNode NYT){
    assert(tr != NULL && NYT != NULL);
    assert(IsNYTNode(NYT));
    
    tr->NYT = NYT;
    return;
}


void ResetToInternalNode(TreeNode trn){
    assert(trn != NULL);
    assert(! IsInternalNode(trn));
    trn->c = INTERNAL_NODE_C;
    return;
}
