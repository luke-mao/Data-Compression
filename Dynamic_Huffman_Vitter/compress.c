#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "tree.h"
#include "list.h"
#include "dictionary.h"
#include "file.h"
#include "compress.h"


// in update function, find the parent node of this list node
ListNode FindParentListNode(ListNode);

void FindSlideBoundary(ListNode LN, ListNode* LN_start_p, ListNode* LN_final_p);

ListNode FindLeaderInTheBlock(ListNode);


// add .v suffix
char* compression_create_output_filename(char* filename_in){
    assert(filename_in != NULL);

    int len = strlen(filename_in);
    char* filename_out = (char*) malloc((len+2+1) * sizeof(char));

    strcpy(filename_out, filename_in);
    strcat(filename_out, ".v");

    return filename_out;
}


void compress_file_and_output(FILE* fp_in, FILE* fp_out){
    assert(fp_in != NULL && fp_out != NULL);

    // create the tree, dictionary and the list
    Tree tr = TreeCreate();
    List L = ListCreate();
    Dictionary d = DictionaryCreate(ASCII_SIZE);

    // buffer for print
    // when reach 8, print the byte
    int buffer = 0;
    int buffer_len = 0;

    // before reading the first char
    // print a byte to output file first, later used to record num of digits pad
    FilePrintEmptyByte(fp_out);

    // for the first char, build the tree and list naively
    int c = getc(fp_in);
    TreeUpdateForFirstChar(tr, c);
    ListUpdateForFirstChar(L, tr);
    DictionaryInsert(d, L->next->next);
    FilePrintByte(&buffer, &buffer_len, fp_out, c);

    // debug
    // printf("After first char = %c %d\n", c, c);
    // TreeShow(tr);
    // ListShow(L);
    // printf("\n");


    // read and compress
    while ((c=getc(fp_in)) != EOF){
        // debug
        // printf("Insert char = %c %d\n", c, c);
        UpdateAndPrint(tr, L, d, &buffer, &buffer_len, c, fp_out);
    }

    // at the end, pad the file
    int num_pad = FilePrintPad(&buffer, &buffer_len, fp_out);
    FileRePrintFirstByte(num_pad, fp_out);


    // clean everything
    TreeDestroy(tr);
    ListDestroy(L);
    DictionaryDestroy(d, ASCII_SIZE);

    return;
}


void compression_status(char* name_in, char* name_out, FILE* fp_in, FILE* fp_out){
    assert(fp_in != NULL && fp_out != NULL);
    assert(name_in != NULL && name_out != NULL);

    fseek(fp_in, 0, SEEK_END);
    fseek(fp_out, 0, SEEK_END);

    long size_in = ftell(fp_in);
    long size_out = ftell(fp_out);

    printf("Input file: %s\nSize: %.3f KB\n", name_in, (float) size_in / 1024);
    printf("Output file: %s\nSize: %.3f KB\n", name_out, (float) size_out / 1024);
    printf("Space saving: %.2f%%\n", ((float) 1 - (float)size_out / (float) size_in) * 100);

    return;
}


void TreeUpdateForFirstChar(Tree tr, int c){
    assert(tr != NULL);
    assert(c >= 0);
    
    // this is the first char
    // split the root into NYT on the left and new node on the right
    // the tree is created already

    // first create a TreeNode for the new char
    // so here the new symbol node is assigned with occ = 1 directly
    TreeNode newNode = TreeNodeCreate(c, 1, NULL, NULL, tr->root);
    TreeNode newNYT = TreeNodeCreate(NYT_C, 0, NULL, NULL, tr->root);

    tr->root->left = newNYT;
    tr->root->right = newNode;

    tr->root->occ += 1;
    tr->NYT = newNYT;

    return;
}


void ListUpdateForFirstChar(List L, Tree tr){
    assert(L != NULL);
    assert(tr != NULL);
    // check root node
    assert(tr->root != NULL);
    // check it has a left NYT and right child not null
    assert(tr->root->right != NULL && tr->root->left == tr->NYT);
    // check the right child is a leaf node
    assert(tr->root->right->left == NULL && tr->root->right->right == NULL);
    // check the right child char >= 0 and occ = 1
    assert(tr->root->right->c >= 0 && tr->root->right->occ == 1);


    // create list node for: NYT, right child, and root
    // then link them together
    ListNode LN_NYT = ListNodeCreate(tr->NYT);
    ListNode LN_right = ListNodeCreate(tr->root->right);
    ListNode LN_root = ListNodeCreate(tr->root);

    // assign the list node
    L->next = LN_NYT;

    // forward link
    LN_NYT->next = LN_right;
    LN_right->next = LN_root;

    // backward link
    LN_root->prev = LN_right;
    LN_right->prev = LN_NYT;

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

        LN_p = ListGetFromTreeNode(L, tr->NYT);

        FilePrintNodePath(buffer_p, buffer_len_p, fp, LN_p->trn);
        
        FilePrintByte(buffer_p, buffer_len_p, fp, c);
    }
    else{
        // printf("existing symbol\n");
        // existing symbol, print the trace only
        FilePrintNodePath(buffer_p, buffer_len_p, fp, LN_p->trn);
    }

    // now the list node LN_c is either listnode NYT, or the listnode for the symbol

    ListNode LN_LeafToIncrement = NULL;
    TreeNode trn_p = LN_p->trn;


    if (trn_p->c == NYT_C){
        // change the current trn NYT to internal trn
        trn_p->c = INTERNAL_NODE_C;

        // create two new trn: NYT and the new symbol
        TreeNode trn_NYT = TreeNodeCreate(NYT_C, 0, NULL, NULL, NULL);
        // update the NYT in the tree
        tr->NYT = trn_NYT;


        TreeNode trn_c = TreeNodeCreate(c, 0, NULL, NULL, NULL);

        // reconstruct that tree
        // top to down
        trn_p->left = trn_NYT;
        trn_p->right = trn_c;
        // bottom to up
        trn_NYT->parent = trn_p;
        trn_c->parent = trn_p;

        // add to list
        ListNode LN_internal = LN_p;        // the NYT list node
        
        // create list node for the new NYT and new symbol node
        ListNode LN_NYT = ListNodeCreate(trn_NYT);

        // new symbol node
        ListNode LN_c = ListNodeCreate(trn_c);
        // for the new symbol, insert into dictionary
        DictionaryInsert(d, LN_c);
        
        // reconstruct the list, left to right
        L->next = LN_NYT;
        LN_NYT->next = LN_c;
        LN_c->next = LN_internal;

        // right to left
        LN_internal->prev = LN_c;
        LN_c->prev = LN_NYT;

        // p = parent of the symbol node, LN_p does not change
        // leaf to increment = the right child of trn_p
        LN_LeafToIncrement = LN_c;        
    }
    else{
        // swap trn_p in the tree with the leader of its block
        SwapWithLeader(L, LN_p);
        
        // if p is the sibling of the 0 node
        if (trn_p->parent->left->c == NYT_C){
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
    while (LN_p->trn->c != ROOT_C){
        SlideAndIncrement(L, &LN_p);
    }

    // increase root weight
    LN_p->trn->occ += 1;

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


void SlideAndIncrement(List L, ListNode* LN_p){
    // perform some checks, the inside treenode cannot be root or NYT
    assert(L != NULL);
    assert(LN_p != NULL && (*LN_p)->trn != NULL);  
    assert((*LN_p)->trn->c != ROOT_C && (*LN_p)->trn->c != NYT_C);

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

        bool prev_is_right_child = false;
        bool cur_is_right_child = false;

        if (prev_parent->right == (*LN_p)->trn){
            prev_is_right_child = true;
        }

        TreeNode this_parent;


        while (LN_this != LN_final){
            // check current is right child or not
            this_parent = LN_this->trn->parent;
            if (this_parent->right == LN_this->trn){
                cur_is_right_child = true;
            }

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
    if ((*LN_p)->trn->c == INTERNAL_NODE_C){
        (*LN_p) = ListGetFromTreeNode(L, trn_fp);
    }
    else{
        (*LN_p) = ListGetFromTreeNode(L, (*LN_p)->trn->parent);
    }

    return;
}


// range of slide: [start, final), note the right endpoint is not included
void FindSlideBoundary(ListNode LN, ListNode* LN_start_p, ListNode* LN_final_p){
    // check all pointers
    assert(LN != NULL && LN_start_p != NULL && LN_final_p != NULL);
    
    // also check the trn must be either a leaf node or an internal node
    assert(LN->trn->c >= 0 || LN->trn->c == INTERNAL_NODE_C);

    int target_occ;
    ListNode cur;

    if (LN->trn->c >= 0){
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


void SwapWithLeader(List L, ListNode LN){
    // check not null, and the list node must be a leaf node
    assert(L != NULL);
    assert(LN != NULL && LN->trn != NULL && LN->trn->c >= 0);

    ListNode LN_leader = FindLeaderInTheBlock(LN);

    // if the leader of the block exist 
    // (otherwise means the LN is already the leader of the block)
    if (LN_leader != NULL){
        // first reconnect at the tree level
        TreeNode trn_p = LN->trn->parent;
        
        bool trn_is_right_child = false;
        if (trn_p->right == LN->trn){
            trn_is_right_child = true;
        }

        TreeNode trn_leader_p = LN_leader->trn->parent;
        
        bool trn_leader_p_is_right_child = false;
        if (trn_leader_p->right == LN_leader->trn){
            trn_leader_p_is_right_child = true;
        }

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


// only look for leader of a leaf node
ListNode FindLeaderInTheBlock(ListNode LN){
    assert(LN != NULL && LN->trn != NULL && LN->trn->c >= 0);

    int occ = LN->trn->occ;
    ListNode result = LN->next;

    while (result != NULL && result->trn->c >= 0 && result->trn->occ == occ){
        result = result->next;
    }

    // move one step back
    result = result->prev;

    // if valid, return
    if (result->trn->c >= 0 && result->trn->occ == occ){
        return result;
    }
    else{
        return NULL;
    }
}
