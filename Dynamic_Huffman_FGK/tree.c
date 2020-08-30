#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "tree.h"

void TreeShowFunction(Node root);
void TreeUpdateFunction(Node root, Node n);
Node FindNodeNumberMaxInBlock(Node root, int occ);


Node NodeCreate(int c, int label, int occ, Node left, Node right, Node parent){
    // first create the node
    Node n = (Node) malloc(sizeof(struct _Node));
    assert(n != NULL);

    // fill information
    n->c = c;
    n->label = label;
    n->occ = occ;
    n->left = left;
    n->right = right;
    n->parent = parent;

    return n;
}


Node NodeDestroy(Node n){
    // recursive delete the children first, then delete this node
    if (n != NULL){
        NodeDestroy(n->left);
        NodeDestroy(n->right);
        
        // now free
        free(n);
        n = NULL;
    }

    return n;
}


Tree TreeCreate(void){
    Tree tr = (Tree) malloc(sizeof(struct _Tree));
    assert(tr != NULL);
    tr->root = NodeCreate(ROOT_C, LABEL_START, 0, NULL, NULL, NULL);
    return tr;
}


Tree TreeDestroy(Tree tr){
    assert(tr != NULL);
    NodeDestroy(tr->root);

    free(tr);
    tr = NULL;
    return tr;
}


void TreeShow(Tree tr){
    assert(tr != NULL);
    
    if (tr->root = NULL){
        fprintf(stdout, "Tree is empty\n");
        exit(EXIT_FAILURE);
    }
    else{
        TreeShowFunction(tr->root);
    }

    return;
}


// inorder traversal of the tree: left -> root -> right
void TreeShowFunction(Node n){
    if (n != NULL){
        // left 
        TreeShowFunction(n->left);

        // middle root
        if (n->c == ROOT_C){
            fprintf(stdout, "(Root,%d,%d) ", n->label, n->occ);
        }
        else if (n->c == INTERNAL_NODE_C){
            fprintf(stdout, "Internal,%d,%d) ", n->label, n->occ);
        }
        else{
            fprintf(stdout, "(%c,%d,%d) ", n->c, n->label, n->occ);
        }

        // right
        TreeShowFunction(n->right);
    }

    return;
}


void TreeUpdate(Tree tr, NodeList ndlist, Node NYT, int c){
    assert(tr != NULL);
    assert(ndlist != NULL);

    // first determine if the list contain the node or not
    Node n = NodeListFindNode(ndlist, c);
    if (n != NULL){
        // node has been created before
        TreeUpdateFunction(tr->root, n);
    }
    else{
        // n is empty, create the node first
        // n->label = current NYT label - 1
        // n->parent = current NYT
        n = NodeCreate(c, NYT->label-1, 1, NULL, NULL, NYT);
        NodeListAddNode(ndlist, n);

        Node newNYT = NodeCreate(NYT_C, NYT->label-2, 0, NULL, NULL, NYT);

        // rearrange the oldNYT connection parts
        NYT->left = newNYT;
        NYT->right = n;
        NYT->occ += 1;

        // loop to update the tree using sibling property
        // update from lowest level to the root level
        TreeUpdateFunction(tr->root, NYT);        

        // finally, re-assign the NYT
        NYT = newNYT;
    }


    return;
}


void TreeUpdateFunction(Node root, Node n){
    assert(n != NULL);
    Node target;
    
    Node tmp_left, tmp_right, tmp_parent;
    int tmp_c, tmp_label, tmp_occ;

    while (n->c != ROOT_C){
        // check if its node number is max in the block
        // traversal the whole tree
        target = FindNodeNumberMaxInBlock(root, n->occ);    
        // check if the target is current n's parent
        // if yes, ignore, if not do the swap
        if (target == n->parent){
            // increase weight
            n->occ += 1;
            // move to its parent level
            n = n->parent;
            continue;
        }

        // swap: three scenario
        //      swap internal node with internal
        //      swap internal with leaf
        //      swap leaf with leaf
        // first create an exact copy of the target
        tmp_left = target->left;
        tmp_right = target->right;
        tmp_parent = target->parent;
        tmp_c = target->c;
        tmp_label = target->label;
        tmp_occ = target->occ;

        // move node n to target, keep the label unchanged
        target->c = n->c;
        target->occ = n->occ;
        target->left = n->left;
        target->right = n->right;
        target->parent = n->parent;

        // move tmp to n, keep the label unchanged
        n->c = tmp_c;
        n->occ = tmp_occ;
        n->left = tmp_left;
        n->right = tmp_right;
        n->parent = tmp_parent;

        // now reassign
        n = target;
        // increase weight
        n->occ += 1;
        // move to parent
        n = n->parent;
    }

    // for root node, simply increase occ
    n->occ += 1;
    return;
}


Node FindNodeNumberMaxInBlock(Node root, int occ){
    Node result = NULL;

    if (root->occ == occ){
        // does not need to go deeper
        // since child's node number definitely smaller than root node number
        result = root;
    }
    else if (root->occ < occ){
        result = NULL;
    }
    else{
        // now root->occ > occ
        // go deeper
        Node left_max = FindNodeNumberMaxInBlock(root->left, occ);
        Node right_max = FindNodeNumberMaxInBlock(root->right, occ);

        // for the same occ, return the max label node
        // the label number must be different
        if (left_max->label > right_max->label){
            result = left_max;
        }
        else if (left_max->label < right_max->label){
            result = right_max;
        }
        else{
            fprintf(stderr, "label number same error\n");
            exit(EXIT_FAILURE);
        }
    }

    return result;
}



NodeList NodeListCreate(void){
    NodeList ndlist = (NodeList) malloc(ALPHABET_SIZE * sizeof(Node));
    assert(ndlist != NULL);

    // null every entry
    for (int i = 0; i < ALPHABET_SIZE; i++){
        ndlist[i] = NULL;
    }

    return ndlist;
}


NodeList NodeListDestroy(NodeList ndlist){
    assert(ndlist != NULL);

    // at the time to destroy ndlist, the tree should already be destroyed
    // so no need to call NodeDestroy to all entries
    free(ndlist);
    ndlist = NULL;
    return ndlist;
}


void NodeListAddNode(NodeList ndlist, Node n){
    assert(ndlist != NULL);
    assert(n != NULL && n->c != ROOT_C && n->c != INTERNAL_NODE_C);

    ndlist[n->c] = n;
    return;
}


Node NodeListFindNode(NodeList ndlist, int c){
    assert(ndlist != NULL);
    // if no node for that char, then return NULL
    return ndlist[c];
}