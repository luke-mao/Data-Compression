#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "FGK_functions.h"


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