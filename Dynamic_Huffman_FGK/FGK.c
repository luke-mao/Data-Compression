#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "tree.h"


void FGK_compress(const char* filename);
void FGK_decompress(const char* filename);


int main(int argc, const char** argv){
    // Usage: ./FGK -compress/-decompress file
    if (argc != 3){
        fprintf(stderr, "Usage: %s <-compress|-decompress> <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "-compress") == 0){
        FGK_compress(argv[2]);
    }
    else if(strcmp(argv[1], "-decompress") == 0){
        FGK_decompress(argv[2]);
    }
    else{
        fprintf(stderr, "Usage: %s <-compress|-decompress> <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    return 0;
}


void FGK_compress(const char* filename){
    FILE* fp = fopen(filename, "rb");
    assert(fp != NULL);

    // prepare the tree
    Tree tr = TreeCreate();
    NodeList ndlist = NodeListCreate();

    fprintf(stdout, "Initially: ");
    TreeShow(tr);

    int c;
    int i = 0;
    while ((c = getc(fp)) != EOF){
        i += 1;
        fprintf(stdout, "Number (%d): insert %c %d: \n", i, c, c);
        TreeUpdate(tr, ndlist, c);

        // debug use
        fprintf(stdout, "Final tree is: ");
        TreeShow(tr);
        printf("\n");
    }

    return;
}



void FGK_decompress(const char* filename){
    puts("not finish");
    return;
}