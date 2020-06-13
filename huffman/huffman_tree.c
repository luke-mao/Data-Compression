#include "huffman_tree.h"

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

    pq->nodes = (Node**) malloc((pq->total)*sizeof(Node*));
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
            Node* n = create_tree_node(i, t->buckets[i]->freq);
            pq_insert(pq, n);
        }
    }
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


void pq_insert(NodePQ* pq, Node* n){
    // insert, always put the lowest freq to the end of the queue
    // during insert, first: the pq->current++
    // the smallest freq is located at the rightmost of the array
    if (pq->current == 0){
        pq->nodes[0] = n;
    }
    else{
        // linear scan all existing freq
        int idx = 0;
        while (idx < pq->current){
            if (pq->nodes[idx]->freq < n->freq){
                break;
            }
        }

        // then the node"n" replaces the idx position, 
        // the rest position are moved 1 space towards right
        for(int j = pq->current; j > idx; j--){
            pq->nodes[j] = pq->nodes[j-1];
        }
        pq->nodes[idx] = n;
    }
    return;
}

Node* pq_pop(NodePQ* pq){
    // pop the smallest freq, pop the end of the current items
    Node* returnNode = pq->nodes[pq->current - 1];
    pq->nodes[pq->current - 1] = NULL;
    pq->current--;
    return returnNode;
}


int pq_item_count(const NodePQ* pq){
    return pq->current;
}


// tree implementation
Tree* create_tree(void);
Node* create_tree_node(Byte, long freq);
void add_to_left(Node* root, Node* left);
void add_to_right(Node* root, Node* right);
Tree* delete_tree(Tree*);


