#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "tree.h"
#include "list.h"


// just create the node that pack the treenode
// assign the prev and next pointers later
ListNode ListNodeCreate(TreeNode trn){
    assert(trn != NULL);
    
    ListNode listn = (ListNode) malloc(sizeof(struct _ListNode));
    assert(listn != NULL);

    listn->trn = trn;
    listn->prev = NULL;
    listn->next = NULL;

    return listn;
}   


// during free: the tree is freed first, then the list
// so here only need to free the outermost listnode structure
// recursion, delete all "next" first, then delete current one
ListNode ListNodeDestroy(ListNode listn){
    if (listn != NULL){
        ListNodeDestroy(listn->next);
        free(listn);
        listn = NULL;
    }

    return listn;
}


// create the list: dummy head
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


// only new leaf node can be inserted into the list
void ListInsertNewSymbol(List L, ListNode newLeafNode){
    assert(L != NULL);
    assert(newLeafNode != NULL);
    assert(newLeafNode->trn != NULL);

    // trn must be a leaf node, and trn must have occ == 1 as a new leaf node
    assert(newLeafNode->trn->c >= 0);     
    assert(newLeafNode->trn->occ == 1);

    // so the new symbol will simply be placed on the right of NYT
    // dummy head -> NYT -> this position -> other nodes
    ListNode left = L->next;
    ListNode right = L->next->next;

    // forward link
    left->next = newLeafNode;
    newLeafNode->next = right;

    // backward link
    right->prev = newLeafNode;
    newLeafNode->prev = left;
    
    return;
}            


void ListShow(List L){
    assert(L != NULL);
    printf("Linked list: ");
    
    ListNode listn = L->next;
    while (listn != NULL){
        if (listn->trn->c >= 0){
            printf("(%c-%d, ", listn->trn->c, listn->trn->c);
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


ListNode ListGetNYT(List L){
    assert(L != NULL);
    assert(L->next->trn->c == NYT_C);
    return L->next;
}