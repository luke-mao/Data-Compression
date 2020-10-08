#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "frequency_table.h"
#include "file.h"
#include "util.h"


FreqTable FreqTableCreate(int size){
    assert(size > 0);

    FreqTable fqtable = (FreqTable) malloc(sizeof(struct _FreqTable));
    assert(fqtable != NULL);

    fqtable->size = size;

    fqtable->table = (int*) malloc(size * sizeof(int));
    assert(fqtable->table != NULL);

    for (int i = 0; i < size; i++){
        fqtable->table[i] = 0;
    }    

    return fqtable;
}


FreqTable FreqTableDestroy(FreqTable fqtable){
    assert(fqtable != NULL);
    
    free(fqtable->table);
    fqtable->table = NULL;

    free(fqtable);
    fqtable = NULL;

    return fqtable;
}


void FreqTableInsert(FreqTable fqtable, int c){
    assert(fqtable != NULL && fqtable->table != NULL);
    assert(c >= 0 && c < fqtable->size);
    assert(fqtable->table[c] >= 0);

    fqtable->table[c] += 1;
    return;
}


void FreqTableGetCount(FreqTable fqtable, int c){
    assert(fqtable != NULL && fqtable->table != NULL);
    assert(c >= 0 && c < fqtable->size);
    assert(fqtable->table[c] >= 0);

    return fqtable->table[c]
}


void FreqTableShow(FreqTable fqtable){
    assert(fqtable != NULL && fqtable->table != NULL);

    printf("Frequency Table Print:\n");
    for (int i = 0; i < fqtable->size; i++){
        if (fqtable->table[i] > 0){
            printf("idx = %d, char = %c, count = %d\n", i, i, fqtable->table[i]);
        }
    }

    printf("\n");
    return;
}

