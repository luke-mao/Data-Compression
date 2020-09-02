#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"


TreeNode TreeNodeCreate(int c, int occ, TreeNode* left, TreeNode* right, TreeNode* parent){
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


void TreeUpdateForFirstChar(Tree tr, int c){
    // this is the first char
    // split the root into NYT on the left and new node on the right

}


void TreeUpdate(Tree tr, int c);

