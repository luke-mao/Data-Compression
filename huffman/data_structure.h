#ifndef DATA_STRUCTURE_H_
#define DATA_STRUCTURE_H_

#include <stdbool.h>
#include "file.h"
#include "freq_table.h"
#include "byte.h"


/*
    This header file includes three data structures used in the project.
    1. Huffman tree: obtain the codeword of each char.
    2. Priority queue: use it to construct the tree, 
        pop the node with smallest freq every time.
    3. Stack: use it during post-order-traversal of the tree, 
        to write the header for the compressed file
*/

/*Tree node and tree*/
typedef struct tree_node{
    Byte b;         // this contain the byte
    long freq;      // this contain freq: set to 0 for non-leaf nodes
    struct tree_node* left;
    struct tree_node* right;
} TreeNode;

typedef struct tree{
    TreeNode* root;         // the root node
    int leaf_count;          // count how many leaf nodes
} Tree;


/*  
    Priority queue
    use it to store nodes during tree construction
    easier to use in the array
    arrange the nodes based on the freq
    so index=0 => freq max, index=count-1 => freq min
*/
typedef struct node_priority_queue{
    TreeNode** nodes;           // number of nodes initially = count
    int total;              // total positions available
    int current;            // current number of positions filled
} NodePQ;


/*  
    Stack, used in the compression for post order traversal
    also will be used in decompression to construct the tree
*/
typedef struct node_stack{
    TreeNode** nodes;
    int total_num;
    int current_num;
} NodeStack;


// tree
Tree* create_tree(const FreqTable*);    // create the empty tree and assign p_eof
TreeNode* create_tree_node(Byte, long, TreeNode*, TreeNode*);
void fill_tree(Tree*, NodePQ*);
Tree* destroy_tree(Tree*);
void post_order_traversal_test(const Tree*); // test to examine the tree in post-order-traversal
void post_order_traversal_test_no_recursion(const Tree*);   // no recursion method, use stack
void get_codeword(Tree*, FreqTable*);       // get the codeword


// priority queue
NodePQ* create_pq(const FreqTable*);    // create and fill the pq
NodePQ* destroy_pq(NodePQ*);
void pq_insert(NodePQ*, TreeNode*);
TreeNode* pq_pop(NodePQ*);
int pq_item_count(const NodePQ*);
void pq_print(const NodePQ*);


// stack 
NodeStack* create_stack(int size);
NodeStack* destroy_stack(NodeStack*);
void stack_push(NodeStack*, TreeNode*);
TreeNode* stack_pop(NodeStack*);
TreeNode* stack_top(const NodeStack*);
int stack_current_size(const NodeStack*);
bool stack_is_full(const NodeStack*);
bool stack_is_empty(const NodeStack*);


#endif