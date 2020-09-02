#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tree.h"
#include "list.h"
#include "dictionary.h"
#include "compress.h"


// add .vitter suffix
char* compression_create_output_filename(char* filename_in){
    assert(filename_in != NULL);

    int len = strlen(filename_in);
    char* filename_out = (char*) malloc((len+7+1) * sizeof(char));

    strcpy(filename_out, filename_in);
    strcat(filename_out, ".vitter");

    return filename_out;
}


void compress_file_and_output(FILE* fp_in, FILE* fp_out){
    assert(fp_in != NULL && fp_out != NULL);

    // create the tree, dictionary and the list
    Tree tr = TreeCreate();
    List L = ListCreate();
    Dictionary d = DictionaryCreate(ASCII_SIZE);

    int c = getc(fp_in);
    TreeUpdateForFirstChar(tr, c);
    ListInitialUpdate(L, tr);
    DictionaryInsert(d, L->next->next);

    ListNode list_n_p, leafToIncrement;

    while ((c=getc(fp_in)) != NULL){
        update()
    }





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
    printf("Space saving: %.3f%%\n", (float) 1 - (float)size_out / (float) size_in);

    return;
}


void compress_update_everything(Tree tr, List L, Dictionary d, int c){
    
    leafToIncrement = NULL;
    list_n_p = DictionarySearch(d, c);

    if (list_n_p == NULL){
        // first occurrence
        // the old NYT node give birth to two new node

        list_n_p = ???;
        leafToIncrement = the right child of p;
    } 
    else{
        // swap p with the leader of the block
        // note that p->trn must be a leaf block !!!!!!

        // if p is the sibling of the 0-node
        if (p->parent->left == tr->NYT){
            leafToIncrement = p;
            p = parent of p; ///////////
        }
    }

    while (p->trn != tr->root){
        SlideAndIncrement(L, p);
    }

    if (leafToIncrement != NULL){
        SlideAndIncrement(L, p);
    }

    return;
}

