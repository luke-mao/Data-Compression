#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "data_structure.h"
#include "freq_table.h"
#include "byte.h"


// func for the tree deletion.
void destroy_tree_func(TreeNode*);
// func for tree debug  
void post_order_traversal_test_func(TreeNode* trnode);
// func for finding the max value
int max(const int a, const int b);
// func for get_codeword
void get_codeword_func(TreeNode* trnode, CodeWord** cw, int array[], int top);


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
    // pq->current: current number of nodes in the queue
    int idx = 0;
    while (idx < pq->current){
        // the freq in the pq decreases with increasing index
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


void pq_print(const NodePQ* pq){
    // debug function, check the priority queue
    assert(pq != NULL);

    fprintf(stdout, "Priority Queue (debug)\n");
    fprintf(stdout, "pq->current = %d\n", pq->current);
    fprintf(stdout, "pq->total = %d\n", pq->total);

    for(int i = 0; i < pq->current; i++){
        assert(pq->nodes[i] != NULL);
        fprintf(stdout, "%c (%d): %ld\n", i, i, pq->nodes[i]->freq);
    }

    fprintf(stdout, "\n");
    return;
}


Tree* create_tree(const int leaf_counter){
    // this function creates the empty tree
    // and malloc the root, and assign p_eof
    Tree* tr;
    tr = (Tree*) malloc (1*sizeof(Tree));
    if (tr == NULL){
        fprintf(stderr, "memory error create_tree\n");
        exit(EXIT_FAILURE);
    }
    
    tr->leaf_count = leaf_counter;
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

    assert(tr->leaf_count == pq->current);

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
        TreeNode* trnode = create_tree_node(1, (trn1->freq)+(trn2->freq), trn1, trn2);

        // insert back to the priority queue
        pq_insert(pq, trnode);
    }

    tr->root = pq_pop(pq);

    // pq should be empty now
    // pq_scan(pq);
    return;
}


void pq_scan(const NodePQ* pq){
    assert(pq != NULL);

    fprintf(stdout, "Priority Queue scan: large freq -> small freq\n");
    fprintf(stdout, "pq->total=%d, pq->current=%d\n", pq->total, pq->current);
    for (int i = 0; i < pq->total; i++){
        if (pq->nodes[i] != NULL){
            fprintf(stdout, 
                    "Node %d: char=(%c)(%d), freq=%ld\n", 
                    i, pq->nodes[i]->b, pq->nodes[i]->b, pq->nodes[i]->freq
            );
        }
    }

    fprintf(stdout, "Scan finish\n");
    return;
}


Tree* destroy_tree(Tree* tr){
    // free the memory, recursion method
    assert(tr != NULL);

    destroy_tree_func(tr->root);
    free(tr);
    tr = NULL;
    return tr;
}


void destroy_tree_func(TreeNode* trnode){
    if (trnode != NULL){
        destroy_tree_func(trnode->left);
        destroy_tree_func(trnode->right);
        free(trnode);
        trnode = NULL;
    }
    return;
}


void post_order_traversal_test(const Tree* tr){
    // test programme, use during debug to printout the post order travesal
    // caution about the pesudo eof byte
    fprintf(stdout, "Tree post order traversal\n");
    post_order_traversal_test_func(tr->root);
    fprintf(stdout, "\n");
    return;
}


void post_order_traversal_test_func(TreeNode* trnode){
    if (trnode != NULL){
        post_order_traversal_test_func(trnode->left);
        post_order_traversal_test_func(trnode->right);

        // now print trnode itself, consider two situations: non-leaf node and leaf node
        if (trnode->left != NULL || trnode->right != NULL){
            // non-leaf node
            fprintf(stdout, "(%ld) ", trnode->freq);
        }
        else{
            // leaf node
            fprintf(stdout, "(%c->%ld) ", trnode->b, trnode->freq);
        }
    }
    return;
}


void post_order_traversal_test_no_recursion(const Tree* tr){
    // no recursion method to do the post order traversal
    assert(tr != NULL);
    NodeStack* s = create_stack(2 * tr->leaf_count);    // double the number in case core dump

    fprintf(stdout, "Tree post order traversal (no recursion)\n");
     
    TreeNode* root = tr->root;
    do {
        // step 1: trace down to the null left root
        while (root != NULL){
            // push root right into the stack
            if (root->right != NULL){
                stack_push(s, root->right);
            }
            // push root into the stack
            stack_push(s, root);
            // go down to the root left
            root = root->left;
        }

        // pop one item out and set it as root
        root = stack_pop(s);
        if (root->right != NULL && root->right == stack_top(s)){
            // remove the top item
            stack_pop(s);
            // push the root back
            stack_push(s, root);
            // set root as root right node
            root = root->right;
        }
        else{
            // print the data, but consider leaf and non-leaf situations
            if (root->left != NULL || root->right != NULL){
                // non-leaf node
                fprintf(stdout, "(%ld) ", root->freq);
            }
            else{
                // leaf node
                fprintf(stdout, "(%c->%ld) ", root->b, root->freq);
            }

            // after printing, set root as NULL
            root = NULL;
        }
    } while (! stack_is_empty(s));

    fprintf(stdout, "\n");
    destroy_stack(s);
    return;
}


void get_codeword(Tree* tr, CodeWord** cw){
    // after the finalize of the tree, get the codeword and store in the freqtable
    int array[get_tree_height(tr->root)];
    get_codeword_func(tr->root, cw, array, 0);
    return;
}


void get_codeword_func(TreeNode* trnode, CodeWord** cw, int array[], int top){
    if (trnode != NULL){
        if (trnode->left == NULL && trnode->right == NULL){
            // leaf node
            create_cw_with_index(cw, (int)trnode->b, top);
            // copy the cw
            for (int i = 0; i < top; i++){
                (cw[(int)trnode->b]->array)[i] = array[i];
            }
        }
        else{
            array[top] = 0;
            get_codeword_func(trnode->left, cw, array, top+1);
            array[top] = 1;
            get_codeword_func(trnode->right, cw, array, top+1);
        }
    }

    return;
}


// return the max height of the tree
int get_tree_height(const TreeNode* root){
    if (root == NULL){
        return 0;
    }
    else{
        return 1 + max(get_tree_height(root->left), get_tree_height(root->right));
    }
}


int max(const int a, const int b){
    if (a>b){
        return a;
    }
    else{
        return b;
    }
}


NodeStack* create_stack(int size){
    // input the number to set the size of the stack
    NodeStack* s = (NodeStack*) malloc (1*sizeof(NodeStack));
    if (s == NULL){
        fprintf(stderr, "memory error, create_stack stack\n");
        exit(EXIT_FAILURE);
    }

    s->total_num = size;
    s->current_num = 0;

    s->nodes = (TreeNode**) malloc(size*sizeof(TreeNode*));
    if (s->nodes == NULL){
        fprintf(stderr, "memory error, create_stack, nodes\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < size; i++){
        s->nodes[i] = NULL;
    }

    return s;
}


NodeStack* destroy_stack(NodeStack* s){
    // free everything
    for(int i = 0; i < s->current_num; i++){
        free(s->nodes[i]);
        s->nodes[i] = NULL;
    }

    free(s->nodes);
    s->nodes = NULL;

    free(s);
    s = NULL;

    return s;
}


void stack_push(NodeStack* s, TreeNode* trnode){
    assert(s != NULL && trnode != NULL);
    assert(! stack_is_full(s));

    s->nodes[s->current_num] = trnode;  // no need to malloc
    s->current_num++;

    return;
}


TreeNode* stack_pop(NodeStack* s){
    assert(s != NULL);
    assert(! stack_is_empty(s));

    s->current_num--;
    return s->nodes[s->current_num];
}


TreeNode* stack_top(const NodeStack* s){
    assert(s != NULL);
    if (stack_is_empty(s)){
        return NULL;
    }
    else{
        return s->nodes[s->current_num - 1];
    }
}


int stack_current_size(const NodeStack* s){
    assert(s != NULL);
    return s->current_num;
}

bool stack_is_full(const NodeStack* s){
    assert(s!=NULL);
    return s->current_num == s->total_num;
}


bool stack_is_empty(const NodeStack* s){
    assert(s!=NULL);
    return s->current_num == 0;
}
