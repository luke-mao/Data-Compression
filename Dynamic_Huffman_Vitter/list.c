#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "tree.h"
#include "list.h"


// for simplicity, set all next pointer to NULL
ListNode ListNodeCreate(TreeNode trn){
    assert(trn != NULL);
    
    ListNode listn = (ListNode) malloc(sizeof(struct _ListNode));
    assert(listn != NULL);

    listn->trn = trn;
    listn->prev = NULL;
    listn->next = NULL;

    return listn;
}   


// recursion, delete all "next" first, then delete current one
ListNode ListNodeDestroy(ListNode listn){
    if (listn != NULL){
        ListNodeDestroy(listn->next);
        free(listn);
        listn = NULL;
    }

    return listn;
}


// create the linked list, use a dummy head
List ListCreate(void){
    List L = (List) malloc(sizeof(struct _ListNode));
    assert(L != NULL);

    L->trn = NULL;
    L->next = NULL; // so the first actual node is stored in L->next

    return L;
}


List ListDestroy(List L){
    assert(L != NULL);

    // since it has a dummy head, destroy the second node downards
    L->next = ListNodeDestroy(L->next);
    free(L);
    L = NULL;
    return L;
}


void ListInsert(List L, ListNode newNode){
    assert(L != NULL);
    assert(newNode != NULL);

    // the list is in increasing order of occ
    // for same occ, leaf nodes are before internal nodes
    // for same occ and same leaf/internal, new node are before old node (implicit numbering)

    // since the first node is a dummy head, 
    // so start from the second node
    ListNode curr = L->next;

    // first locate the starting place for the same occ
    // if newNode->trn is a leaf node, then simply stop at there
    // and form the link: curr->prev then newNode then curr
    while (curr != NULL && curr->trn->occ < newNode->trn->occ){
        curr = curr->next;
    }

    // but if the newNode->trn is an internal node,
    // then need to skip the leaf nodes with the same occ
    // so still form the link: curr->prev then newNode then curr
    if (newNode->trn->c >= 0){
        while (curr != NULL && curr->trn->occ < 0){
            curr = curr->next;
        }
    }

    ListNode prev = curr->prev;

    // forward link
    prev->next = newNode;
    newNode->next = curr;

    // backward link
    curr->prev = newNode;
    newNode->prev = prev;


    return;
}            


void ListShow(List L){
    assert(L != NULL);
    printf("Linked list: ");
    
    ListNode listn = L->next;
    while (listn != NULL){
        if (listn->trn->c >= 0){
            printf("(%c-%d, ", listn->trn->c);
        }
        else if(listn->trn->c == ROOT_C){
            printf("(Root, ");
        }
        else{
            printf("(Internal, ");
        }

        printf("%d) ", listn->trn->occ);

        // move on to next one
        listn = listn->next;
    }

    printf("\n");
}          


void ListInitialUpdate(List L, Tree tr){
    assert(tr != NULL);
    assert(L != NULL);

    // pay attention to the insert order
    // first is root, second is root->right, third is root->left
    ListNode listn = ListNodeCreate(tr->root);
    ListInsert(L, listn);

    listn = ListNodeCreate(tr->root->right);
    ListInsert(L, listn);

    listn = ListNodeCreate(tr->root->left);
    ListInsert(L, listn); 

    return;
}


// Slide and increment "listn->trn"
// Vitter's paper slides the tree nodes only.
// However, in order to maintain the implicit numbering,
// I need to slide and shift the list nodes 
// which contain the corresponding treenodes.
// So the shift needs to be done 
// both in the "linked list" level and the "tree" level

void SlidAndIncrement(List L, ListNode p){
    assert(L != NULL);
    assert(p != NULL);


    TreeNode old_parent_p_trn = p->trn->parent;


    // consider two cases: p->trn is leaf / internal node
    if (p->trn->c >= 0){
        // the node is a leaf node
        // (already swap to be the leader of the leaf node)
        // so the internal node should be next to it
        ListNode start = p->next;
        ListNode final = NULL;

        // check if it is the same occ we want
        if (start->trn->occ == p->trn->occ){
            // determine the final
            final = start->next;
            while (final != NULL && final->trn->occ == p->trn->occ){
                final = final->next;
            }

            // so shift the range [start, final), note does not include final !!
            // first rearrange the tree node parent connection
            // the connection is established both upwards and downwards
            // Node sequence: p->trn, start->trn, XX, XX, XX, final
            ListNode prev = p;
            ListNode curr = start;
            ListNode curr_trn_old_parent;

            while (curr != final){
                curr_trn_old_parent = curr->trn->parent;

                curr->trn->parent = prev->trn->parent;

                if (prev->trn->left == prev->trn){
                    prev->trn->left = curr->trn;
                }
                else{
                    prev->trn->right = curr->trn;
                }

                // move forwards
                prev = curr;
                curr = curr->next;
            }

            // now curr = "final", so prev is the last node
            p->trn->parent = curr_trn_old_parent;
            if (curr_trn_old_parent->trn->left == prev->trn){
                prev->trn->left = curr->trn;
            }
            else{
                prev->trn->right = curr->trn;
            }


            // and finally, reform the connection at the linked list level
            // new connection: p->prev => start => xx => xx => final->prev => p => final
            p->prev->next = start;
            start->prev = p->prev;

            final->prev->next = p;
            p->prev = final->prev;

            p->next = final;
            final->prev = p;
        }

        // if the start is not the same occ, then nothing to slide & increment
        // so stop 

    }
    else{
        // internal node
        // at upper level of implementation, stop when p is a root node
        // so no need to worry about root node here

        // for internal nodes, shift leaf node with p->trn->occ + 1
        // first determine the boundary [start, final)
        ListNode start = p->next;
        ListNode final = NULL;
        if (start->trn->occ == (p->trn->occ + 1) && start->trn->c >= 0){
            // find the end boundary
            final = start;
            while (final != NULL && 
                final->trn->occ == (p->trn->occ+1) && final->trn->c >= 0){
                // move forward
                final = final->next;               
            }

            // so shift the range [start, final), note does not include final !!
            // first rearrange the tree node parent connection
            // the connection is established both upwards and downwards
            // Node sequence: p->trn, start->trn, XX, XX, XX, final
            ListNode prev = p;
            ListNode curr = start;
            ListNode curr_trn_old_parent;

            while (curr != final){
                curr_trn_old_parent = curr->trn->parent;

                curr->trn->parent = prev->trn->parent;

                if (prev->trn->left == prev->trn){
                    prev->trn->left = curr->trn;
                }
                else{
                    prev->trn->right = curr->trn;
                }

                // move forwards
                prev = curr;
                curr = curr->next;
            }

            // now curr = "final", so prev is the last node
            p->trn->parent = curr_trn_old_parent;
            if (curr_trn_old_parent->trn->left == prev->trn){
                prev->trn->left = curr->trn;
            }
            else{
                prev->trn->right = curr->trn;
            }


            // and finally, reform the connection at the linked list level
            // new connection: p->prev => start => xx => xx => final->prev => p => final
            p->prev->next = start;
            start->prev = p->prev;

            final->prev->next = p;
            p->prev = final->prev;

            p->next = final;
            final->prev = p;
        }
    }


    // increase p weight
    p->trn->occ += 1;
    
    // move upwards
    if (p->trn->c >= 0){
        p = p->trn->parent;
    }
    else{
        p = old_parent_p_trn;
    }

    return;
}

