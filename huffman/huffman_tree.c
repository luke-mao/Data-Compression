#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "huffman_tree.h"
#include "freq_table.h"
#include "main.h"


// additional func for the tree deletion.
void destroy_tree_func(TreeNode*);
// additional func for tree debug  
void post_order_traversal_test_func(TreeNode* trnode, Byte p_eof);
// additonal func for get_codeword
void get_codeword_func(TreeNode*, FreqTable*, Byte, int);


// priority queue implementation
NodePQ* create_pq(const FreqTable* t){
    assert(t != NULL);

    NodePQ* pq = (NodePQ*) malloc (1*sizeof(NodePQ));
    if (pq == NULL){
        fprintf(stderr, "memory error: create_pq pq\n");
        exit(EXIT_FAILURE);
    }

    pq->total = t->counter;
    pq->current = 0;

    pq->nodes = (TreeNode**) malloc((pq->total)*sizeof(TreeNode*));
    if (pq->nodes == NULL){
        fprintf(stderr, "memory error: create_pq, pq->nodes\n");
        exit(EXIT_FAILURE);
    }

    // scan all positions, make null
    for (int i = 0; i < pq->total; i++){
        pq->nodes[i] = NULL;
    }

    for (int i = 0; i < ASCII_NUMBER; i++){
        if (t->buckets[i] != NULL){
            TreeNode* n = create_tree_node((Byte)i, t->buckets[i]->freq, NULL, NULL);
            pq_insert(pq, n);
        }
    }

    return pq;
}


NodePQ* destroy_pq(NodePQ* pq){
    for (int i = 0; i < pq->total; i++){
        if (pq->nodes[i] != NULL){
            free(pq->nodes[i]);
            pq->nodes[i] = NULL;
        }
    }

    free(pq);
    pq = NULL;
    return pq;
}


void pq_insert(NodePQ* pq, TreeNode* n){
    // insert, always put the lowest freq to the end of the queue
    // during insert, first: the pq->current++
    // the smallest freq is located at the rightmost of the array
    // linear scan all existing freq
    int idx = 0;
    while (idx < pq->current){
        if (pq->nodes[idx]->freq < n->freq){
            break;
        }
        idx++;
    }

    // then the node"n" replaces the idx position, 
    // the rest position are moved 1 space towards right
    for(int j = pq->current; j > idx; j--){
        pq->nodes[j] = pq->nodes[j-1];
    }
    pq->nodes[idx] = n;
    pq->current++;      // update the current counter
    
    return;
}

TreeNode* pq_pop(NodePQ* pq){
    // there must be some nodes available, check "current" > 0
    assert (pq->current > 0);

    // pop the smallest freq, pop the end of the current items
    TreeNode* returnNode = pq->nodes[pq->current - 1];
    pq->nodes[pq->current - 1] = NULL;
    pq->current--;
    return returnNode;
}


int pq_item_count(const NodePQ* pq){
    return pq->current;
}


// tree implementation

Tree* create_tree(const FreqTable* t){
    // this function creates the empty tree
    // and malloc the root, and assign p_eof
    Tree* tr;
    tr = (Tree*) malloc (1*sizeof(Tree));
    if (tr == NULL){
        fprintf(stderr, "memory error create_tree\n");
        exit(EXIT_FAILURE);
    }

    tr->p_eof = t->p_eof;
    tr->root = NULL;

    return tr;
}

TreeNode* create_tree_node(Byte b, long freq, TreeNode* left, TreeNode* right){
    // create the tree node
    // this function is used in the pq creation
    TreeNode* trnode = (TreeNode*) malloc (1*sizeof(TreeNode));
    if (trnode == NULL){
        fprintf(stderr, "memory error, create_tree_node\n");
        exit(EXIT_FAILURE);
    }

    trnode->freq = freq;
    trnode->b = b;

    // the char node should be the leaf node, so left = right = null
    trnode->left = left;
    trnode->right = right;
    return trnode;
}


void fill_tree(Tree* tr, NodePQ* pq){
    // given the tree and the node priority queue, fill the tree
    // the priority queue is filled with all nodes 

    // when the loop ends, there should be only 1 thing, which is the root node
    while (pq_item_count(pq) != 1){
        // follow the huffman tree construction
        // first pop two smallest freq nodes out
        TreeNode* trn1 = pq_pop(pq);
        TreeNode* trn2 = pq_pop(pq);

        // create the Treenode, with 1 as left and 2 as right
        // the byte just input 1, since it is not relevant
        TreeNode* trnode = create_tree_node(1, (trn1->freq)+(trn2->freq), trn1, trn2
        );

        // insert back to the priority queue
        pq_insert(pq, trnode);
    }

    tr->root = pq_pop(pq);
    return;
}


Tree* destroy_tree(Tree* tr){
    // free the memory
    // recursion method
    destroy_tree_func(tr->root);
    free(tr);
    tr = NULL;
    return tr;
}

void destroy_tree_func(TreeNode* trnode){
    if (trnode == NULL)   return;
    if (trnode->left != NULL)   destroy_tree_func(trnode->left);
    if (trnode->right != NULL)  destroy_tree_func(trnode->right);
    
    free(trnode);
    trnode = NULL;

    return;
}


void post_order_traversal_test(const Tree* tr){
    // test programme, use during debug to printout the post order travesal
    // caution about the pesudo eof byte
    fprintf(stdout, "Tree post order traversal\n");
    post_order_traversal_test_func(tr->root, tr->p_eof);
    fprintf(stdout, "\n");
    return;
}

void post_order_traversal_test_func(TreeNode* trnode, Byte p_eof){
    if (trnode != NULL){
        post_order_traversal_test_func(trnode->left, p_eof);
        post_order_traversal_test_func(trnode->right, p_eof);

        // now print trnode itself, consider two situations: non-leaf node and leaf node
        if (trnode->left != NULL || trnode->right != NULL){
            // non-leaf node
            fprintf(stdout, "(%ld) ", trnode->freq);
        }
        else{
            // leaf node
            if (trnode->b == p_eof){
                fprintf(stdout, "(p_eof->%ld) ", trnode->freq);
            }
            else{
                fprintf(stdout, "(%c->%ld) ", trnode->b, trnode->freq);
            }
        }
    }
    return;
}


void get_codeword(Tree* tr, FreqTable* t){
    // after the finalize of the tree, get the codeword and store in the freqtable
    get_codeword_func(tr->root, t, (Byte) 0, 0);
    return;
}


void get_codeword_func(TreeNode* trnode, FreqTable* t, Byte codeword, int count){
    if (trnode != NULL){
        if (trnode->left == NULL && trnode->right == NULL){
            // a leaf node
            t->buckets[(int)trnode->b]->codeword = codeword;
            t->buckets[(int) trnode->b]->cw_count = count;
        }
        else{
            count++;
            codeword = codeword << 1;  // shift 1 position, added position is 0
            get_codeword_func(trnode->left, t, codeword, count);
            codeword |= 1;  // added position change to 1
            get_codeword_func(trnode->right, t, codeword, count);
        }
    }
    return;
}