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
    listn->next = NULL;

    return listn;
}   


// recursion, delete all "next" first, then delete current one
ListNode ListNodeDestroy(ListNode listn){
    if (listn != NULL){
        listn->next = ListNodeDestroy(listn->next);
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


void ListInsert(List L, ListNode listn){
    assert(L != NULL);
    assert(listn != NULL);

    // the list is in increasing order of occ
    // for same occ, leaf nodes are before internal nodes
    // for same occ and same leaf/internal, new node are before old node (implicit numbering)

    ListNode prev = NULL;
    ListNode curr = L->next;

    // first find the list position with occ >= new node
    while (curr != NULL && curr->trn->occ < listn->trn->occ){
        prev = curr;
        curr = curr->next;
    }

    // but for internal nodes, need to skip leaf nodes with the same occ
    while (curr != NULL && curr->trn->occ == listn->trn->occ && curr->trn->c >= 0){
        prev = curr;
        curr = curr->next;
    }

    // now the list n should be placed between prev - curr
    // form prev -> listn ->curr

    prev->next = listn;
    listn->next = curr;

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

