#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data_structure.h"


// define the hash table capacity
// 4096 (12 bits) * capacity factor
// in order to reduce hash collision
#define CAPACITY (SIZE_LIMIT * CAPACITY_FACTOR)

// dictionary, create an empty node
Node node_create(const Key k, const CodeWord cw, Node next);

// dictionary, delete the node
Node node_delete(Node);


// calculate the hash value
Index calculate_index(const char* s){
    assert(s != NULL);

    Index val = 0;
    int ch;

    // iterate around the string, extract char one by one
    while (*s != '\0'){
        // extract this char, make sure it is positive
        ch = *s;
        if (ch < 0){
            ch += 256;
        }

        // left shift the last value and then add the new char
        val = (val << 3) + ch;
        // move to the next char
        s += 1;         
    }

    // extract the index, index must be >= 0 
    val = val % CAPACITY;
    
    while (val < 0){
        val += CAPACITY;
    }
    
    return val;
}


// create the dictionary, insert 256 chars + 1 for EOF and 1 for reflux
// so that the 256 and 257 index are reserved
// size = 4096, current_num = 258, so next index start at 258 during insert
Dictionary dictionary_create(void){
    Dictionary d = (struct _Dictionary*) malloc (sizeof(struct _Dictionary));
    assert(d != NULL);

    d->nodes = (Node*) malloc (CAPACITY * sizeof(Node));
    assert(d->nodes != NULL);

    for (Index i = 0; i < CAPACITY; i++){
        d->nodes[i] = NULL;
    }

    // fill the hash table with initial values 0 - 255
    d->current_num = 256 + 2; // initially 256 char + 1 for EOF + 1 for reflush   

    return d;
}


// release memory of the dictionary 
Dictionary dictionary_destroy(Dictionary d){
    assert(d != NULL);

    // use recursion to free all nodes
    for (Index i = 0; i < CAPACITY; i++){
        if (d->nodes[i] != NULL){
            node_delete(d->nodes[i]);
        }
    }

    free(d->nodes);
    d->nodes = NULL;
    free(d);
    d = NULL;
    return d;
}


// reset dictionary if it is full,
// to reflect more local characteristics. 
// reset = destroy + create
Dictionary dictionary_reset(Dictionary d){
    assert(d != NULL);
    d = dictionary_destroy(d);
    d = dictionary_create();
    return d;
}


// check if is full, if full, need reset
bool dictionary_is_full(Dictionary d){
    assert(d != NULL);

    // note that the capacity is a multiple of 4096
    // but when we reach 4096 items, we do the reflush !!!
    return d->current_num == SIZE_LIMIT;
}


// check if the key exist, 
// if exist return the index, if not return -1 
CodeWord dictionary_search(Dictionary d, const Key k){
    assert(d != NULL && k != NULL);

    CodeWord result;

    // if it is a single char, simply return its int
    if (strlen(k) == 1){
        int idx = k[0];
        if (idx < 0){
            idx += 256;
        }
        
        result = idx;
    }
    else{
        // for string of length >= 2
        Index hidx = calculate_index(k);
        if (d->nodes[hidx] == NULL){
            result = -1;
        }
        else{
            // if see the exact key, then return the index
            Node n = d->nodes[hidx];
            while (n != NULL && strcmp(n->k, k) != 0){
                n = n->next;
            }

            if (n == NULL){
                result = -1;
            }
            else{
                result = n->cw;   // find the key, return the codeword
            }
        }
    }

    return result;
}


// insert key-cw pair into the dictionary
// if hash index position is null, insert
// if not null, find the end of the linked table, then insert
CodeWord dictionary_insert(Dictionary d, const Key k){
    assert(d != NULL && k != NULL);

    // calculate the hash index
    Index hidx = calculate_index(k);
    assert(hidx >= 0);

    // assign the codeword
    CodeWord cw = d->current_num;       

    if (d->nodes[hidx] == NULL){
        d->nodes[hidx] = node_create(k, cw, NULL);
    }
    else{
        Node n = d->nodes[hidx];
        while (n->next != NULL){
            n = n->next;
        }
        n->next = node_create(k, cw, NULL);
    }

    // update the counter
    d->current_num += 1;
    return cw;
}


// debug use: print the dictionary
void dictionary_print(Dictionary d){
    assert(d != NULL);

    fprintf(stdout, "-----Dictionary print-----\n");
    fprintf(stdout, "Size = %d, current_num = %ld\n", SIZE_LIMIT, d->current_num);

    for (Index i = 0; i < CAPACITY; i++){

        if (i <= 255){
            // single char range
            fprintf(stdout, "[%ld] %c => %ld ", i, (int)i, i);
            if (d->nodes[i] != NULL){
                Node n = d->nodes[i];
                while (n != NULL){
                    fprintf(stdout, "%s => %ld ", n->k, n->cw);
                    n = n->next;
                }
            }
            fprintf(stdout, "\n");
        }
        else if (d->nodes[i] != NULL){
            Node n = d->nodes[i];
            fprintf(stdout, "[%ld]  ", i);
            while (n != NULL){
                fprintf(stdout, "%s => %ld ", n->k, n->cw);
                n = n->next;
            }
            fprintf(stdout, "\n");
        }
    }

    fprintf(stdout, "-----End-----\n");
    return;
}


// create a node, initialize with NULL next pointer
Node node_create(const Key k, const CodeWord cw, Node next){
    Node n = (Node) malloc (sizeof(struct _Node));
    assert(n != NULL);

    // allocate memory
    n->k = (char*)malloc((strlen(k)+1)*sizeof(char)); // add 1 for \0
    assert(n->k != NULL);
    // copy the key
    strcpy(n->k, k);

    // create the index and next pointer
    n->cw = cw;
    n->next = next;

    return n;
}


// recursive to delete all nodes of the dictionary
Node node_delete(Node n){
    if (n == NULL){
        return n;
    }
    else{
        node_delete(n->next);
        free(n->k);
        free(n);
        n = NULL;
        return n;
    }
}


// array functions
// create
// 0-255 leaves empty, also reserve 256 for EOF and 257 for Reflush
Array array_create(void){

    Array a = (struct _Array*) malloc (sizeof(struct _Array));
    assert(a != NULL);

    // occupy the first 258+2 positions, but do not set anything
    // so that the whole ram usage can be smaller
    a->current_num = 256 + 2;

    // here we only need 4096 items
    // since here we do not perform hash, so no hash collision
    a->nodes = (char**) malloc (SIZE_LIMIT * sizeof(char*));
    assert(a->nodes != NULL);

    for (Index i = 0; i < SIZE_LIMIT; i++){
        a->nodes[i] = NULL;
    }

    return a;
}


// clean the array
Array array_destroy(Array a){
    assert(a != NULL);

    for (Index i = 0; i < SIZE_LIMIT; i++){
        if (a->nodes[i] != NULL){
            free(a->nodes[i]);
            a->nodes[i] = NULL;
        }
    }

    free(a->nodes); 
    free(a);
    a = NULL;
    return a;
}


// reset = destroy + create
Array array_reset(Array a){
    assert(a != NULL);

    a = array_destroy(a);
    a = array_create();
    return a;
}


// check if full
bool array_is_full(const Array a){
    assert(a != NULL);
    return a->current_num == SIZE_LIMIT;
}


// search using index, return the string
// Key prev is included to avoid the unseen mistake
// that during compression, the key is created then used straight away
// without any time gap.
// Also need to take care with the return key
// if simply print it, it is fine
// but if need to modify, make sure use strcpy first
// do not change on the returned pointer directly
//
// malloc and copy for all answers.
Key array_search(Array a, const Index idx){
    assert(a != NULL && idx >= 0);

    Key result;

    // if the index <= 255, create the key and return
    // since the bucket is actually NULL
    if (idx <= 255){
        result = (Key) malloc (2 * sizeof(char));
        assert(result != NULL);

        result[0] = idx;
        result[1] = '\0';
    }
    else{
        assert(a->nodes[idx] != NULL);

        // malloc and then strcmp
        result = (char*)malloc((strlen(a->nodes[idx])+1)*sizeof(char));
        assert(result != NULL);
        strcpy(result, a->nodes[idx]);
    }

    return result;
}


// insert, no codeword required, simply insert in first in order
Index array_insert(Array a, const Key k){
    assert(a != NULL && k != NULL);
    assert(! array_is_full(a));

    a->nodes[a->current_num] = (char*) malloc ((strlen(k)+1)*sizeof(char));
    assert(a->nodes[a->current_num] != NULL);

    strcpy(a->nodes[a->current_num], k);
    a->current_num += 1;    // update the counter
    return a->current_num - 1;
}


// check if the index is in the array or not
// i.e. the array has corresponding codeword or not
bool array_has_this_codeword(Array a, const Index idx){
    assert(a != NULL && idx >= 0);
    
    if (idx < 256){
        return true;
    }
    else{
        return a->nodes[idx] != NULL;
    }
}


// debug use
void array_print(const Array a){
    assert(a != NULL);

    fprintf(stdout, "-----Array print-----\n");
    fprintf(stdout, "Size = %d, current_num = %ld\n", SIZE_LIMIT, a->current_num);

    for (Index i = 0; i < SIZE_LIMIT; i++){
        if (i <= 255){
            fprintf(stdout, "%ld => %c\n", i, (int) i);
        }
        else if (a->nodes[i] != NULL){
            fprintf(stdout, "%ld => %s\n", i, a->nodes[i]);
        }
    }

    fprintf(stdout, "-----End-----\n");
    return;
}