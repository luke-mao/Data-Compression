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


ListNode GetPrev(ListNode LN){
    assert(LN != NULL);
    return LN->prev;
}


ListNode GetNext(ListNode LN){
    assert(LN != NULL);
    return LN->next;
}


void ConnectAsPrev(ListNode cur, ListNode prev){
    assert(cur != NULL && prev != NULL);
    cur->prev = prev;
    return;
}


void ConnectAsNext(ListNode cur, ListNode next){
    assert(cur != NULL && next != NULL);
    cur->next = next;
    return;
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
        else if (listn->trn->c == NYT_C){
            printf("(NYT, ");
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


TreeNode GetTreeNode(ListNode LN){
    assert(LN != NULL && LN->trn != NULL);
    return LN->trn;
}


ListNode GetListNode(List L, TreeNode trn){
    assert(L != NULL && trn != NULL);

    // linear scan the list
    ListNode result = L->next;
    while (result != NULL && result->trn != trn){
        result = result->next;
    }

    assert(result != NULL);
    return result;
}


ListNode FindParentListNode(ListNode LN){
    assert(LN != NULL);

    // simply go down LN->next, and compare the trn
    ListNode current = LN->next;
    while (current != NULL && current->trn != LN->trn->parent){
        current = current->next;
    }

    // the return must not be null, otherwise error
    assert(current != NULL);
    return current;
}


ListNode GetListHead(List L){
    assert(L != NULL);
    return L->next;
}


void AssignListHead(List L, ListNode LN){
    assert(L != NULL && LN != NULL);
    L->next = LN;
    return;
}
