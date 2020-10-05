#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tree.h"
#include "list.h"
#include "dictionary.h"
#include "compress.h"


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

    // for the first char, build the tree and list naively
    int c = getc(fp_in);
    TreeUpdateForFirstChar(tr, c);
    ListUpdateForFirstChar(L, tr);
    DictionaryInsert(d, L->next->next);
    FilePrintDigit(&buffer, &buffer_len, fp_out, 0);

    // need to have a buffer to print 
    // and for the first char, print 0


    while ((c=getc(fp_in)) != EOF){
        UpdateAndPrint(tr, L, d, c, &buffer, &buffer_len, fp_out);
    }


    // clean everything
    TreeDestroy(tr);
    ListDestroy(L);
    DictionaryDestroy(d);

    return;
}


// single digit
void FilePrintDigit(int* buffer_p, int* buffer_len_p, FILE* fp, int new_digit){
    assert(buffer_p != NULL && buffer_len_p != NULL);
    assert(fp != NULL);
    assert(new_digit == 0 || new_digit == 1);

    (*buffer_p) <<= 1;
    (*buffer_p) |= (new_digit & 1);
    (*buffer_len_p) += 1;

    // print when 8
    if (*buffer_len_p == 8){
        fputc(*buffer_p, fp);
        
        // reset
        (*buffer_p) = 0;
        (*buffer_len_p) = 0
    }

    return;
}


// for new node, print the byte
void FilePrintByte()


// for NYT, or 
void FilePrintNode(int* buffer_p, int* buffer_len_p, FILE* fp, TreeNode trn){
    assert(buffer_p != NULL && buffer_len_p != NULL);
    assert(fp != NULL);
    assert(trn != NULL);
    
    // recursion: go to the root node first, then print digit one by one
    if (trn->c != ROOT_C){
        FilePrintNode(trn->parent);
    }

    // now reach the root


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
    printf("Space saving: %.3f%%\n", (float) 1 - (float)size_out / (float) size_in);

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

    // for a symbol c, first look for its treenode from the dictionary
    ListNode LN_c = DictionarySearch(d, c);
    if (LN_c == NULL){
        LN_c = ListGetNYT(L);
    }

    // print the upwards trace of the inside treenode
    FILE





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
            TreeNode curr_trn_old_parent;


            // This part needs to be reviewed!!!
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


void SwapWithLeader(List L, ListNode L_n){
    assert(L != NULL);
    assert(L_n != NULL);

    // Since this function is only called during the early stage
    // of update the tree, L_n->trn must be a leaf node
    // look for the leader of the leaf block, with same occ
    ListNode L_n_leader = L_n->next;

    while (L_n_leader->trn->occ == L_n->trn->occ && L_n_leader->trn->c >= 0){
        L_n_leader = L_n_leader->next;
    }

    // move one position backwards
    L_n_leader = L_n_leader->prev;

    // if it is same as the input node, no need to swap
    if (L_n_leader != L_n){
        // do the swap on both the tree level and the linked list level
        
        // first do the swap on the tree level
        TreeNode trn_leader_old_p = L_n_leader->trn->parent;
        TreeNode trn_old_p = L_n->trn->parent;
        
        // from child to parent
        L_n_leader->trn->parent = trn_old_p;
        L_n->trn->parent = trn_leader_old_p;

        // from parent to child for leader
        if (trn_leader_old_p->left == L_n_leader->trn){
            trn_leader_old_p->left = L_n->trn;
        }
        else{
            trn_leader_old_p->right = L_n->trn;
        }
        
        // from parent to child to the node
        if (trn_old_p->left == L_n->trn){
            trn_old_p->left = L_n_leader->trn;
        }
        else{
            trn_old_p->right = L_n_leader->trn;
        }

        // now do the swap on the linked list level
        // now the link should be:
        // L_n->prev => L_n_leader => L_n->next => 
        //          => xx => L_n_leader->prev => L_n => L_n_leader->next 
        // Name the following vairables:
        // first => L_n_leader => second => ... => third => L_n => fourth
        ListNode first = L_n->prev;
        ListNode second = L_n->next;
        ListNode third = L_n_leader->prev;
        ListNode fourth = L_n_leader->next;

        // first do the forward link
        first->next = L_n_leader;
        L_n_leader->next = second;
        third->next = L_n;
        L_n->next = fourth;

        // then do the backward link
        fourth->prev = L_n;
        L_n->prev = third;
        second->prev = L_n_leader;
        L_n_leader->prev = first;
    }

    return;
}
