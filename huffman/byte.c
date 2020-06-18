#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "byte.h"

void print_byte(const Byte b, const int count){
    // print the byte, count = 8 if you want to print the whole bytes out
    for (int i = count-1; i >= 0; i--){
        Byte mask = 1 << i;
        Byte result = ((b & mask) >> i);

        if (result == 1){
            fprintf(stdout, "1");
        }
        else{
            fprintf(stdout, "0");
        }
    } 
    return;
}


CodeWord** create_cw(void){
    // create for all 256 ANSCII chars
    CodeWord** cw = (CodeWord**) malloc (256 * sizeof(CodeWord*));
    if (cw == NULL){
        fprintf(stdout, "memory error, create_cw\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 256; i++){
        cw[i] = NULL;
    }

    return cw;
}


void create_cw_with_index(CodeWord** cw, const int i, const int maxlen){
    // create for this codeword
    assert(cw != NULL);
    cw[i] = (CodeWord*) malloc (sizeof(int) +maxlen*sizeof(int));
    if (cw[i] == NULL){
        fprintf(stdout, "memory error, create_cw_with_index\n");
        exit(EXIT_FAILURE);
    }

    cw[i]->num = maxlen;
    return;
}


CodeWord** destroy_cw(CodeWord** cw){
    // free each component
    for (int i = 0; i < 256; i++){
        if (cw[i] != NULL){
            free(cw[i]);
            cw[i] = NULL;
        }
    }

    free(cw);
    cw = NULL;
    return cw;
}


void print_cw(CodeWord** cw){
    // print the whole table, according to ANSCII order
    for (int i = 0; i < 256; i++){
        if (cw[i] != NULL){
            fprintf(stdout, "(%c)(%d): ", i, i);
            for (int j = 0; j < cw[i]->num; j++){
                fprintf( stdout, "%d", (cw[i]->array)[j] );
            }
            fprintf(stdout, "\n");
        }
    }

    return;
}