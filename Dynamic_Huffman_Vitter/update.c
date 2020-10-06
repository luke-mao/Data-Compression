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


void UpdateTreeAndList(Tree tr, List L, Dictionary d, ListNode LN_p, int c){
    assert(tr != NULL);
    assert(L != NULL);
    assert(d != NULL);
    assert(LN_p != NULL);
    assert(c >= 0);

    // now the list node LN_c is either listnode NYT, or the listnode for the symbol

    ListNode LN_LeafToIncrement = NULL;
    TreeNode trn_p = GetTreeNode(LN_p);

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
        ConnectAsParent(trn_NYT, trn_p);
        ConnectAsParent(trn_c, trn_p);

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
    while (! IsRootNode(GetTreeNode(LN_p))){
        SlideAndIncrement(L, &LN_p);
    }

    // increase root weight
    IncreaseOcc(GetTreeNode(LN_p));

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
    assert( (! IsRootNode(GetTreeNode(*LN_p))) && (! IsNYTNode(GetTreeNode(*LN_p))) );


    // record its original parent tree node
    TreeNode trn_fp = GetParent(GetTreeNode(*LN_p));

    // find the boundary of the slide
    ListNode LN_start = NULL;
    ListNode LN_final = NULL;
    FindSlideBoundary(*LN_p, &LN_start, &LN_final);


    // if nothing to slide, skip to bottom to increase the weight
    if (LN_start != NULL && LN_final != NULL){
        ListNode LN_this = LN_start;
        TreeNode prev_parent = GetParent(GetTreeNode(*LN_p));

        bool prev_is_right_child = IsRightChild(GetTreeNode(*LN_p), prev_parent);
        bool cur_is_right_child = false;

        TreeNode this_parent;


        while (LN_this != LN_final){
            // check current is right child or not
            this_parent = LN_this->trn->parent;
            cur_is_right_child = IsRightChild(GetTreeNode(LN_this), this_parent);

            // connect LN_this->trn as a child of prev_parent
            // connect from bottom up
            ConnectAsParent(GetTreeNode(LN_this), prev_parent);
            // connect from top to bottom
            ConnectAsChild(GetTreeNode(LN_this), prev_parent, prev_is_right_child);

            // prepare for next list node
            LN_this = GetNext(LN_this);
            prev_parent = this_parent;
            prev_is_right_child = cur_is_right_child;
            cur_is_right_child = false;
        }


        // now connect LN_p->trn with the prev_paarent
        ConnectAsParent(GetTreeNode(*LN_p), prev_parent);
        ConnectAsChild(GetTreeNode(*LN_p), prev_parent, prev_is_right_child);


        // so far, re-link at the tree level is completed
        // move the LN_p to the position before LN_final
        ListNode LN_left_most = GetPrev(*LN_p);
        ListNode LN_right_before_final = GetPrev(LN_final);

        // the original sequence is: LN_left_most, LN_p, LN_start, xx, xx, LN_right_before_final, LN_final
        // the new sequence is: LN_left_most, LN_start, xx, xx, LN_right_before_final, LN_p, LN_final
        // forward link
        ConnectAsNext(LN_left_most, LN_start);
        ConnectAsNext(LN_right_before_final, *LN_p);
        ConnectAsNext(*LN_p, LN_final);
        // backward link
        ConnectAsPrev(LN_final, *LN_p);
        ConnectAsPrev(*LN_p, LN_right_before_final);
        ConnectAsPrev(LN_start, LN_left_most);
    }


    // increase p weight
    IncreaseOcc(GetTreeNode(*LN_p));
    
    // move upwards
    // if p is an internal node, p = original parent of p
    // if p is a leaf node, p = new parent of p
    if (IsInternalNode(GetTreeNode(*LN_p))){
        (*LN_p) = GetListNode(L, trn_fp);
    }
    else{
        (*LN_p) = GetListNode(L, GetParent(GetTreeNode(*LN_p)));
    }

    return;
}


void SwapWithLeader(List L, ListNode LN){
    // check not null, and the list node must be a leaf node
    assert(L != NULL);
    assert(LN != NULL && GetTreeNode(LN) != NULL && IsLeafNode(GetTreeNode(LN)) );
    

    // find the leader of the block, could be null
    ListNode LN_leader = FindLeaderInTheBlock(LN);

    // if the leader of the block exist 
    // (otherwise means the LN is already the leader of the block)
    if (LN_leader != NULL){
        // first reconnect at the tree level
        TreeNode trn_p = GetParent(GetTreeNode(LN));;        
        bool trn_is_right_child = IsRightChild(GetTreeNode(LN), trn_p);

        TreeNode trn_leader_p = GetParent(GetTreeNode(LN_leader));
        bool trn_leader_p_is_right_child = IsRightChild(GetTreeNode(LN_leader), trn_leader_p);

        // after get all information, reconnect at the tree level
        
        // bottom up
        ConnectAsParent(GetTreeNode(LN), trn_leader_p);
        ConnectAsParent(GetTreeNode(LN_leader), trn_p);

        // top to bottom
        ConnectAsChild(GetTreeNode(LN_leader), trn_p, trn_is_right_child);
        ConnectAsChild(GetTreeNode(LN), trn_leader_p, trn_leader_p_is_right_child);


        // then reconnect at the list level
        // take caution when LN and LN_leader are next to each other
        if (LN->next == LN_leader){
            // if next to each other
            ListNode left = GetPrev(LN);
            ListNode right = GetNext(LN_leader);

            // the new order is: left, LN_leader, LN, right
            // forward
            ConnectAsNext(left, LN_leader);
            ConnectAsNext(LN_leader, LN);
            ConnectAsNext(LN, right);
            // backward
            ConnectAsPrev(right, LN);
            ConnectAsPrev(LN, LN_leader);
            ConnectAsPrev(LN_leader, left);
        }
        else{
            // if they are not next to each other
            // old order: left, LN, mid1, ..., mid2, LN_leader, right
            ListNode left = GetPrev(LN);
            ListNode mid1 = GetNext(LN);
            ListNode mid2 = GetPrev(LN_leader);
            ListNode right = GetNext(LN_leader);

            // new order: left, LN_leader, mid1, ...., mid2, LN, right
            // forward 
            ConnectAsNext(left, LN_leader);
            ConnectAsNext(LN_leader, mid1);
            ConnectAsNext(mid2, LN);
            ConnectAsNext(LN, right);
            // backwaard
            ConnectAsPrev(right, LN);
            ConnectAsPrev(LN, mid2);
            ConnectAsPrev(mid1, LN_leader);
            ConnectAsPrev(LN_leader, left);
        }
    }

    return;
}


// range of slide: [start, final), note the right endpoint is not included
void FindSlideBoundary(ListNode LN, ListNode* LN_start_p, ListNode* LN_final_p){
    // check all pointers
    assert(LN != NULL && LN_start_p != NULL && LN_final_p != NULL);
    // also, the trn must be either a symbol node or an internal node
    assert( IsSymbolNode(GetTreeNode(LN)) || IsInternalNode(GetTreeNode(LN)) );

    int target_occ;
    ListNode cur;

    if (IsLeafNode(GetTreeNode(LN))){
        // for a leaf node, find the same weight internal node range
        target_occ = GetOcc(GetTreeNode(LN));

        if (GetNext(LN) != NULL && IsInternalNode(GetTreeNode(GetNext(LN))) && GetOcc(GetTreeNode(GetNext(LN))) == target_occ){
            // find the start
            (*LN_start_p) = GetNext(LN);

            cur = GetNext(GetNext(LN));
            
            while (cur != NULL && IsInternalNode(GetTreeNode(cur)) && GetOcc(GetTreeNode(cur)) == target_occ){
                cur = GetNext(cur);
            }

            // find the final
            (*LN_final_p) = cur;
        }
    }
    else{
        // internal node: find range of leaf nodes with occ = target_occ + 1
        target_occ = GetOcc(GetTreeNode(LN)) + 1;

        if (LN->next != NULL && IsSymbolNode(GetTreeNode(GetNext(LN))) && GetOcc(GetTreeNode(GetNext(LN))) == target_occ){
            (*LN_start_p) = GetNext(LN);

            cur = GetNext(GetNext(LN));   
              
            while (cur != NULL && IsSymbolNode(GetTreeNode(cur)) && GetOcc(GetTreeNode(cur)) == target_occ){
                cur = GetNext(cur);
            }

            (*LN_final_p) = cur;
        }
    }

    return;
}


// only look for leader of a leaf node
ListNode FindLeaderInTheBlock(ListNode LN){
    assert(LN != NULL && GetTreeNode(LN) != NULL && IsLeafNode(GetTreeNode(LN)));

    int occ = GetOcc(GetTreeNode(LN));
    ListNode result = GetNext(LN);

    while (result != NULL && IsLeafNode(GetTreeNode(result)) && GetOcc(GetTreeNode(result)) == occ){
        result = GetNext(result);
    }

    // move one step back
    result = GetPrev(result);

    // if valid, return
    if (IsLeafNode(GetTreeNode(result)) && GetOcc(GetTreeNode(result)) == occ){
        return result;
    }
    else{
        return NULL;
    }
}