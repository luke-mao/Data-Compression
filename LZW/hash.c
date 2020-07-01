#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hash.h"


// create an empty node
Node node_create(const char* key, Index idx, Node next);
// delete the node
Node node_delete(Node);


// create the dictionary, insert 256 chars + 1 for EOF and 1 for reflux
// so that the 256 and 257 index are reserved
// size = 4096, current_num = 258, so next index start at 258 during insert
Dictionary dictionary_create(void){
    Dictionary d = (struct _Dictionary*) malloc (sizeof(struct _Dictionary));
    if (d == NULL){
        fprintf(stderr, "Memory error: malloc, dictionary");
        exit(EXIT_FAILURE);
    }

    d->current_num = 256 + 2; // initially 256 char + 1 for EOF + 1 for reflush   
    d->size = SIZE_LIMIT; 
    d->nodes = (Node*) malloc (d->size * sizeof(Node));
    if (d->nodes == NULL){
        fprintf(stderr, "Memory error: malloc nodes\n");
        exit(EXIT_FAILURE);
    }

    for (Index i = 0; i < SIZE_LIMIT; i++){
        d->nodes[i] = NULL;
    }

    // fill the hash table with initial values 0 - 255
    for (Index i = 0; i < 255; i++){
        char ch = (char) i;
        dictionary_insert(d, &ch, i);
    }

    return d;
}


Dictionary dictionary_destroy(Dictionary d){
    assert(d != NULL);

    // use recursion to free all nodes
    for (Index i = 0; i < SIZE_LIMIT; i++){
        if (d->nodes[i] != NULL){
            node_delete(d->nodes[i]);
        }
    }

    free(d->nodes);
    d->nodes = NULL;
    free(d);
    return d;
}


// reset,  = destroy + create
Dictionary dictionary_reset(Dictionary d){
    assert(d != NULL);
    dictionary_destroy(d);
    d = dictionary_create(d);
    return d;
}


// check if is full, if full, need reset
bool dictionary_is_full(Dictionary d){
    assert(d != NULL);
    return d->current_num == d->size;
}

// check if the key exist, 
// if exist return the index, if not return -1 
Index dictionary_search(Dictionary d, const char* key){
    assert(d != NULL && key != NULL);

    Index hidx = calculate_index(key);
    if (d->nodes[hidx] == NULL){
        return -1;
    }
    else{
        // if see the exact key, then return the index
        Node n = d->nodes[hidx];
        while (n != NULL && strcmp(n->key, key) != 0){
            n = n->next;
        }

        if (n == NULL){
            return -1;
        }
        else{
            return hidx;
        }
    }
}


// insert
Index dictionary_insert(Dictionary d, const char* key, Index idx){
    assert(d != NULL && key != NULL);

    Index hidx = calculate_index(key);
    if (d->nodes[hidx] == NULL){
        d->nodes[hidx] = node_create(key, idx, NULL);
    }
    else{
        Node n = d->nodes[hidx];
        while (n->next != NULL){
            n = n->next;
        }
        n->next = node_create(key, idx, NULL);
    }

    return hidx;
}




Node node_create(const char* key, Index idx, Node next){
    Node n = (Node) malloc (sizeof(struct _Node));
    if (n == NULL){
        fprintf(stderr, "Memory error: malloc, node\n");
        exit(EXIT_FAILURE);
    }

    // allocate memory
    n->key = (char*)malloc(strlen(key)+1); // add 1 for \0
    if (n->key == NULL){
        fprintf(stderr, "Memory error: node copy key\n");
        exit(EXIT_FAILURE);
    }
    // copy the key
    strcpy(n->key, key);

    // create the index and next pointer
    n->idx = idx;
    n->next = next;

    return n;
}


Node node_delete(Node n){
    if (n == NULL){
        return n;
    }
    else{
        node_delete(n->next);
        free(n->key);
        free(n);
        n = NULL;
        return n;
    }
}