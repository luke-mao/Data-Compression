#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "tree.h"
#include "list.h"
#include "dictionary.h"
#include "file.h"
#include "update.h"


void FindSlideBoundary(ListNode LN, ListNode* LN_start_p, ListNode* LN_final_p);
ListNode FindLeaderInTheBlock(ListNode);


void TreeUpdateForFirstChar(Tree tr, int c){
    assert(tr != NULL);
    assert(c >= 0);
    
    // this is the first char
    // split the root into NYT on the left and new node on the right
    // the tree is created already

    // first create a TreeNode for the new char
    // so here the new symbol node is assigned with occ = 1 directly
    TreeNode newNode = TreeNodeCreate(c, 1, NULL, NULL, GetRoot(tr));
    TreeNode newNYT = TreeNodeCreate(NYT_C, 0, NULL, NULL, GetRoot(tr));

    ConnectAsLeftChild(newNYT, GetRoot(tr));
    ConnectAsRightChild(newNode, GetRoot(tr));

    IncreaseOcc(GetRoot(tr));

    UpdateNYT(tr, newNYT);

    return;
}


void ListUpdateForFirstChar(List L, Tree tr){
    assert(L != NULL);
    assert(tr != NULL);
    // check root node
    assert(GetRoot(tr) != NULL);
    // check it has a left NYT and right child not null
    assert(GetRight(GetRoot(tr)) != NULL && GetLeft(GetRoot(tr)) == GetNYT(tr));
    // check the left child of root is a NYT node
    assert(IsNYTNode(GetLeft(GetRoot(tr))));
    // check the right child is a symbol node
    assert(IsSymbolNode(GetRight(GetRoot(tr))));


    // create list node for: NYT, right child, and root
    // then link them together
    ListNode LN_NYT = ListNodeCreate(GetNYT(tr));
    ListNode LN_right = ListNodeCreate(GetRight(GetRoot(tr)));
    ListNode LN_root = ListNodeCreate(GetRoot(tr));

    // assign the list node
    // L->next = LN_NYT;
    AssignListHead(L, LN_NYT);

    // forward link
    ConnectAsNext(LN_NYT, LN_right);
    ConnectAsNext(LN_right, LN_root);

    // backward link
    ConnectAsPrev(LN_root, LN_right);
    ConnectAsPrev(LN_right, LN_NYT);

    return;
}


void UpdateAndPrint(Tree tr, List L, Dictionary d, int* buffer_p, int* buffer_len_p, int c, FILE* fp){
    assert(tr != NULL && L != NULL && d != NULL);
    assert(buffer_p != NULL && buffer_len_p != NULL && (*buffer_len_p) >= 0 && (*buffer_len_p)<8);
    assert(c >= 0);
    assert(fp != NULL);

    // find the listnode of the symbol
    // the symbol can be new, or existing
    ListNode LN_p = DictionarySearch(d, c);

    // for new symbol
    if (LN_p == NULL){
        // printf("new symbol\n");

        LN_p = GetListNodeFromTreeNode(L, GetNYT(tr));

        FilePrintNodePath(buffer_p, buffer_len_p, fp, GetTreeNodeFromListNode(LN_p));
        
        FilePrintByte(buffer_p, buffer_len_p, fp, c);
    }
    else{
        // printf("existing symbol\n");
        // existing symbol, print the trace only
        FilePrintNodePath(buffer_p, buffer_len_p, fp, GetTreeNodeFromListNode(LN_p));
    }

    // now the list node LN_c is either listnode NYT, or the listnode for the symbol

    ListNode LN_LeafToIncrement = NULL;
    TreeNode trn_p = GetTreeNodeFromListNode(LN_p);


    if (IsNYTNode(trn_p)){
        // change the current trn NYT to internal trn
        ResetToInternalNode(trn_p);

        // create two new trn: NYT and the new symbol
        TreeNode trn_NYT = TreeNodeCreate(NYT_C, 0, NULL, NULL, NULL);
        // update the NYT in the tree
        UpdateNYT(tr, trn_NYT);

        // create a node for this new symbol
        TreeNode trn_c = TreeNodeCreate(c, 0, NULL, NULL, NULL);

        // reconstruct that tree
        // top to down
        ConnectAsLeftChild(trn_NYT, trn_p);
        ConnectAsRightChild(trn_c, trn_p);
        // bottom to up
        ConnectToParent(trn_NYT, trn_p);
        ConnectToParent(trn_c, trn_p);

        // add to list
        ListNode LN_internal = LN_p;        // the NYT list node
        
        // create list node for the new NYT and new symbol node
        ListNode LN_NYT = ListNodeCreate(trn_NYT);

        // new symbol node
        ListNode LN_c = ListNodeCreate(trn_c);
        // for the new symbol, insert into dictionary
        DictionaryInsert(d, LN_c);
        
        // reconstruct the list, left to right
        AssignListHead(L, LN_NYT);
        ConnectAsNext(LN_NYT, LN_c);
        ConnectAsNext(LN_c, LN_internal);

        // right to left
        ConnectAsPrev(LN_internal, LN_c);
        ConnectAsPrev(LN_c, LN_NYT);

        // p = parent of the symbol node, LN_p does not change
        // leaf to increment = the right child of trn_p
        LN_LeafToIncrement = LN_c;        
    }
    else{
        // swap trn_p in the tree with the leader of its block
        SwapWithLeader(L, LN_p);
        
        // if p is the sibling of the 0 node
        if (IsNYTSubling(trn_p)){
            // leaf to increment = p, but assign with its outer structure list node
            LN_LeafToIncrement = LN_p;

            // p = parent of p, need to find the list node
            LN_p = FindParentListNode(LN_p);
        }
    }

    // debug
    // printf("before slide&incre:\n");
    // TreeShow(tr);
    // ListShow(L);

    // while p is not the root of the tree
    while (! IsRootNode(ListNodeGetTreeNode(LN_p))){
        SlideAndIncrement(L, &LN_p);
    }

    // increase root weight
    IncreaseOcc(ListNodeGetTreeNode(LN_p));

    if (LN_LeafToIncrement != NULL){
        SlideAndIncrement(L, &LN_LeafToIncrement);
    }

    // debug
    // printf("after slide&incre:\n");
    // TreeShow(tr);
    // ListShow(L);
    // printf("\n");

    return;
}


void SlideAndIncrement(List L, ListNode* LN_p){
    // perform some checks, the inside treenode cannot be root or NYT
    assert(L != NULL);
    assert(LN_p != NULL && (*LN_p)->trn != NULL);  
    assert( (! IsRootNode(ListNodeGetTreeNode(*LN_p))) && (! IsNYTNode(ListNodeGetTreeNode(*LN_p))) );


    // record its original parent tree node
    TreeNode trn_fp = (*LN_p)->trn->parent;

    // find the boundary of the slide
    ListNode LN_start = NULL;
    ListNode LN_final = NULL;
    FindSlideBoundary(*LN_p, &LN_start, &LN_final);


    // if nothing to slide, skip to bottom to increase the weight
    if (LN_start != NULL && LN_final != NULL){
        ListNode LN_this = LN_start;
        TreeNode prev_parent = (*LN_p)->trn->parent;

        bool prev_is_right_child = IsRightChild(ListNodeGetTreeNode(*LN_p), prev_parent);
        bool cur_is_right_child = false;

        TreeNode this_parent;


        while (LN_this != LN_final){
            // check current is right child or not
            this_parent = LN_this->trn->parent;
            cur_is_right_child = IsRightChild(ListNodeGetTreeNode(LN_this), this_parent);

            // connect LN_this->trn as a child of prev_parent
            // connect from bottom up
            LN_this->trn->parent = prev_parent;
            // connect from top to bottom
            if (prev_is_right_child){
                prev_parent->right = LN_this->trn;
            }
            else{
                prev_parent->left = LN_this->trn;
            }

            // prepare for next list node
            LN_this = LN_this->next;
            prev_parent = this_parent;
            prev_is_right_child = cur_is_right_child;
            cur_is_right_child = false;
        }

        // now connect LN_p->trn with the prev_paarent
        (*LN_p)->trn->parent = prev_parent;
        if (prev_is_right_child){
            prev_parent->right = (*LN_p)->trn;
        }
        else{
            prev_parent->left = (*LN_p)->trn;
        }

        // so far, re-link at the tree level is completed
        // move the LN_p to the position before LN_final
        ListNode LN_left_most = (*LN_p)->prev;
        ListNode LN_right_before_final = LN_final->prev;

        // the original sequence is: LN_left_most, LN_p, LN_start, xx, xx, LN_right_before_final, LN_final
        // the new sequence is: LN_left_most, LN_start, xx, xx, LN_right_before_final, LN_p, LN_final

        // forward link
        LN_left_most->next = LN_start;
        LN_right_before_final->next = (*LN_p);
        (*LN_p)->next = LN_final;

        // backward link
        LN_final->prev = (*LN_p);
        (*LN_p)->prev = LN_right_before_final;
        LN_start->prev = LN_left_most;
    }


    // increase p weight
    (*LN_p)->trn->occ += 1;
    
    // move upwards
    // if p is an internal node, p = original parent of p
    // if p is a leaf node, p = new parent of p
    if (IsInternalNode(ListNodeGetTreeNode(*LN_p))){
        (*LN_p) = ListGetFromTreeNode(L, trn_fp);
    }
    else{
        (*LN_p) = ListGetFromTreeNode(L, (*LN_p)->trn->parent);
    }

    return;
}


void SwapWithLeader(List L, ListNode LN){
    // check not null, and the list node must be a leaf node
    assert(L != NULL);
    assert(LN != NULL && ListNodeGetTreeNode(LN) != NULL && IsLeafNode(ListNodeGetTreeNode(LN)) );

    // find the leader of the block, could be null
    ListNode LN_leader = FindLeaderInTheBlock(LN);

    // if the leader of the block exist 
    // (otherwise means the LN is already the leader of the block)
    if (LN_leader != NULL){
        // first reconnect at the tree level
        TreeNode trn_p = LN->trn->parent;        
        bool trn_is_right_child = IsRightChild(ListNodeGetTreeNode(LN), trn_p);

        TreeNode trn_leader_p = LN_leader->trn->parent;
        bool trn_leader_p_is_right_child = IsRightChild(ListNodeGetTreeNode(LN_leader), trn_leader_p);

        // after get all information, reconnect at the tree level
        
        // bottom up
        LN->trn->parent = trn_leader_p;
        LN_leader->trn->parent = trn_p;

        // top to bottom
        if (trn_is_right_child){
            trn_p->right = LN_leader->trn;
        }
        else{
            trn_p->left = LN_leader->trn;
        }

        if (trn_leader_p_is_right_child){
            trn_leader_p->right = LN->trn;
        }
        else{
            trn_leader_p->left = LN->trn;
        }

        // then reconnect at the list level
        // take caution when LN and LN_leader are next to each other
        if (LN->next == LN_leader){
            // if next to each other
            ListNode left = LN->prev;
            ListNode right = LN_leader->next;

            // the new order is: left, LN_leader, LN, right
            // forward
            left->next = LN_leader;
            LN_leader->next = LN;
            LN->next = right;
            // backward
            right->prev = LN;
            LN->prev = LN_leader;
            LN_leader->prev = left;
        }
        else{
            // if they are not next to each other
            // old order: left, LN, mid1, ..., mid2, LN_leader, right
            ListNode left = LN->prev;
            ListNode mid1 = LN->next;
            ListNode mid2 = LN_leader->prev;
            ListNode right = LN_leader->next;

            // new order: left, LN_leader, mid1, ...., mid2, LN, right
            // forward 
            left->next = LN_leader;
            LN_leader->next = mid1;
            mid2->next = LN;
            LN->next = right;
            // backwaard
            right->prev = LN;
            LN->prev = mid2;
            mid1->prev = LN_leader;
            LN_leader->prev = left;
        }
    }

    return;
}


// range of slide: [start, final), note the right endpoint is not included
void FindSlideBoundary(ListNode LN, ListNode* LN_start_p, ListNode* LN_final_p){
    // check all pointers
    assert(LN != NULL && LN_start_p != NULL && LN_final_p != NULL);
    
    // also check the trn must be either a leaf node or an internal node
    assert(IsLeafNode(ListNodeGetTreeNode(LN)) || IsInternalNode(ListNodeGetTreeNode(LN)));

    int target_occ;
    ListNode cur;

    if (IsLeafNode(ListNodeGetTreeNode(LN))){
        // for a leaf node, find the same weight internal node range
        target_occ = LN->trn->occ;

        if (LN->next != NULL && LN->next->trn->c == INTERNAL_NODE_C && LN->next->trn->occ == target_occ){
            // find the start
            (*LN_start_p) = LN->next;

            cur = LN->next->next;
            while (cur != NULL && cur->trn->c == INTERNAL_NODE_C && cur->trn->occ == target_occ){
                cur = cur->next;
            }

            // find the final
            (*LN_final_p) = cur;
        }
    }
    else{
        // internal node: find range of leaf nodes with occ = target_occ + 1
        target_occ = LN->trn->occ + 1;

        if (LN->next != NULL && LN->next->trn->c >= 0 && LN->next->trn->occ == target_occ){
            (*LN_start_p) = LN->next;

            cur = LN->next->next;
            while (cur != NULL && cur->trn->c >= 0 && cur->trn->occ == target_occ){
                cur = cur->next;
            }

            (*LN_final_p) = cur;
        }
    }

    return;
}


// only look for leader of a leaf node
ListNode FindLeaderInTheBlock(ListNode LN){
    assert(LN != NULL && ListNodeGetTreeNode(LN) != NULL && IsLeafNode(ListNodeGetTreeNode(LN)));

    int occ = LN->trn->occ;
    ListNode result = LN->next;

    while (result != NULL && IsLeafNode(ListNodeGetTreeNode(result)) && result->trn->occ == occ){
        result = result->next;
    }

    // move one step back
    result = result->prev;

    // if valid, return
    if (IsLeafNode(ListNodeGetTreeNode(result)) && result->trn->occ == occ){
        return result;
    }
    else{
        return NULL;
    }
}