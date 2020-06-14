#ifndef HUFFMAN_TREE_H_
#define HUFFMAN_TREE_H_

#include "file.h"
#include "freq_table.h"

// define a tree node
typedef struct tree_node{
    Byte b;         // this contain the byte
    long freq;      // this contain freq: set to 0 for non-leaf nodes
    struct tree_node* left;
    struct tree_node* right;
} TreeNode;

// define the tree
typedef struct tree{
    TreeNode* root;         // the root node
    Byte p_eof;         // contain the eof value
} Tree;


// priority queue
// use it to store nodes during tree construction
// easier to use in the array
// arrange the nodes based on the freq
// so index=0 => freq max, index=count-1 => freq min
typedef struct node_priority_queue{
    TreeNode** nodes;           // number of nodes initially = count
    int total;              // total positions available
    int current;            // current number of positions filled
} NodePQ;


// tree implementation
Tree* create_tree(const FreqTable*);    // create the empty tree and assign p_eof
TreeNode* create_tree_node(Byte, long, TreeNode*, TreeNode*);
void fill_tree(Tree*, NodePQ*);
Tree* destroy_tree(Tree*);
void post_order_traversal_test(const Tree*); // test to examine the tree in post-order-traversal
void get_codeword(Tree*, FreqTable*);       // get the codeword

// priority queue implementation
NodePQ* create_pq(const FreqTable*);    // create and fill the pq
NodePQ* destroy_pq(NodePQ*);
void pq_insert(NodePQ*, TreeNode*);
TreeNode* pq_pop(NodePQ*);
int pq_item_count(const NodePQ*);


#endif