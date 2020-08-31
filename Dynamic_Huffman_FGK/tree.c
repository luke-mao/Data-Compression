#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "tree.h"
#include "FGK_functions.h"

void TreeShowFunction(Node root);
void TreeUpdateFunction(Tree tr, NodeList ndlist, Node n);
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


// recursion, upward trace until reach root
void NodePrintCode(Node n, FILE* fp_out, char* out_c, int* out_c_num){
    assert(n != NULL);

    // stop when reach the root
    if (n->c != ROOT_C){
        // if not root node yet, continue upwards
        NodePrintCode(n->parent, fp_out, out_c, out_c_num);
        
        // check left or right child
        if (n == n->parent->left){
            // left = 0, only print 1 bit
            print_to_file(fp_out, out_c, out_c_num, 0, 1);
        }
        else{
            // right = 1, only print 1 bit
            print_to_file(fp_out, out_c, out_c_num, 1, 1);
        }
        
    }

    return;
}



Tree TreeCreate(void){
    Tree tr = (Tree) malloc(sizeof(struct _Tree));
    assert(tr != NULL);
    
    // create the root node, and initially this node is also the NYT
    tr->root = NodeCreate(ROOT_C, LABEL_START, 0, NULL, NULL, NULL);
    tr->NYT = tr->root;

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
    
    TreeShowFunction(tr->root);
    fprintf(stdout, "\n");

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
            fprintf(stdout, "(Internal,%d,%d) ", n->label, n->occ);
        }
        else if(n->c == NYT_C){
            fprintf(stdout, "(NYT,%d,%d) ", n->label, n->occ);
        }
        else{
            fprintf(stdout, "(%c,%d,%d) ", n->c, n->label, n->occ);
        }

        // right
        TreeShowFunction(n->right);
    }

    return;
}


void TreeUpdate(Tree tr, NodeList ndlist, int c, FILE* fp_out, char *out_c, int* out_c_num){
    assert(tr != NULL);
    assert(ndlist != NULL);

    // first determine if the list contain the node or not
    Node n = NodeListFindNode(ndlist, c);
    if (n != NULL){
        // not first occurrence for this letter
        NodePrintCode(n, fp_out, out_c, out_c_num);

        // node has been created before
        TreeUpdateFunction(tr, ndlist, n);
    }
    else{
        // first occurrence for this letter
        // print the code for NYT
        NodePrintCode(tr->NYT, fp_out, out_c, out_c_num);
        // and also print the new char, the new char has 8 bit
        print_to_file(fp_out, out_c, out_c_num, c, 8);

        // n is empty, create the node first
        // n->label = current NYT label - 1
        // n->parent = current NYT
        n = NodeCreate(c, tr->NYT->label-1, 1, NULL, NULL, tr->NYT);
        NodeListAddNode(ndlist, n);

        Node newNYT = NodeCreate(NYT_C, tr->NYT->label-2, 0, NULL, NULL, tr->NYT);

        // rearrange the oldNYT connection parts
        tr->NYT->left = newNYT;
        tr->NYT->right = n;
        tr->NYT->occ += 1;

        // so right now, the separation of old NYT to two new nodes have been finished
        // also the old NYT weight has been increased
        // reassign the NYT
        tr->NYT = newNYT;

        // check if it is the root node
        // if the old NYT is the root node, do nothing, do not need to update tree any further
        if(tr->NYT->parent->c != ROOT_C){
            tr->NYT->parent->c = INTERNAL_NODE_C;
            // go to its parent node for further update
            TreeUpdateFunction(tr, ndlist, tr->NYT->parent->parent); 
        }
    }

    return;
}


void TreeUpdateFunction(Tree tr, NodeList ndlist, Node n){
    assert(tr != NULL && ndlist != NULL && n != NULL);
    Node target;

    while (n->c != ROOT_C){     
        // check if its node number is max in the block
        // traversal the whole tree
        target = FindNodeNumberMaxInBlock(tr->root, n->occ);    

        // there are 3 possible results: 
        //  1. target = n->parent, 2. target is null, 3. target is purely different
        
        // if yes, ignore, if not do the swap
        if (target == n->parent || target == NULL){
            // increase weight
            n->occ += 1;
            // move to its parent level
            n = n->parent;
        }
        else{
            // swap: three scenario
            //      swap internal node with internal
            //      swap internal with leaf
            //      swap leaf with leaf

            // so we need to notice that
            // if simply break and re-construct from bottom up, is not enough
            // also need to reconstrct from top to down
            Node n_old_parent = n->parent;
            Node target_old_parent = target->parent;

            // first break from top to down
            // the label may be not continuous, so need to use pointer comparison
            if(n == n_old_parent->right){
                // n is the right child
                n_old_parent->right = target;
            }
            else{
                // n is the left child
                n_old_parent->left = target;
            }

            // same as the target side
            if(target == target_old_parent->right){
                target_old_parent->right = n;
            }
            else{
                target_old_parent->left = n;
            }

            // the above are top-down approach, now finish the bottom up
            n->parent = target_old_parent;
            target->parent = n_old_parent;

            // finally, swap the label
            int tmp_label;
            tmp_label = n->label;
            n->label = target->label;
            target->label = tmp_label;


            // and remember to update the node list
            if (n->c >= 0){
                ndlist[n->c] = n;
            }

            if (target->c >= 0){
                ndlist[target->c] = target;
            }

            // at last, increase the occ of n, and move to its parent
            n->occ += 1;
            n = n->parent;
        }
    }

    // for the root node, only need to update the counter
    n->occ += 1;

    return;
}


Node FindNodeNumberMaxInBlock(Node root, int occ){
    Node result = NULL;

    // use the idea that: occ decreases when going deeper, same as the label
    if (root != NULL){
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

            // if one solution is null or both is null
            if(left_max == NULL && right_max == NULL){
                result = NULL;
            }
            else if (left_max == NULL){
                result = right_max;
            }
            else if (right_max == NULL){
                result = left_max;
            }
            else{
                // both are not null
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