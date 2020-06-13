#ifndef HUFFMAN_TREE_H_
#define HUFFMAN_TREE_H_

#include "main.h"

// define a tree node
typedef struct node{
    Byte b;         // this contain the byte
    long freq;      // this contain freq: set to 0 for non-leaf nodes
    struct node* left;
    struct node* right;
} Node;

// define the tree
typedef struct tree{
    Node* root;         // the root node
    Byte p_eof;         // contain the eof value
} Tree;

// functions related to the tree
Tree* create_tree(void);
Node* create_tree_node(Byte, long freq);
void add_to_left(Node* root, Node* left);
void add_to_right(Node* root, Node* right);
Tree* delete_tree(Tree*);


// priority queue
// use it to store nodes during tree construction
// easier to use in the array
// arrange the nodes based on the freq
// so index=0 => freq max, index=count-1 => freq min
typedef struct node_priority_queue{
    Node** nodes;           // number of nodes initially = count
    int total;              // total positions available
    int current;            // current number of positions filled
} NodePQ;

// some functions
NodePQ* create_pq(const FreqTable*);    // create and fill the pq
NodePQ* destroy_pq(NodePQ*);
void pq_insert(NodePQ*, Node*);
Node* pq_pop(NodePQ*);
int pq_item_count(const NodePQ*);



#endif