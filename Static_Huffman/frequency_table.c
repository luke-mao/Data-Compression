#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "frequency_table.h"
#include "util.h"


FreqTable FreqTableCreate(int size){
    assert(size > 0);

    FreqTable fqtable = (FreqTable) malloc(sizeof(struct _FreqTable));
    assert(fqtable != NULL);

    fqtable->size = size;
    fqtable->char_count = 0;

    fqtable->table = (int*) malloc(size * sizeof(int));
    assert(fqtable->table != NULL);

    for (int i = 0; i < size; i++){
        fqtable->table[i] = 0;
    }    

    return fqtable;
}


FreqTable FreqTableDestroy(FreqTable fqtable){
    assert(IsFreqTableValid(fqtable));
    
    free(fqtable->table);
    fqtable->table = NULL;

    free(fqtable);
    fqtable = NULL;

    return fqtable;
}


void FreqTableInsert(FreqTable fqtable, int c){
    assert(IsFreqTableValid(fqtable));
    assert(c >= 0 && c < fqtable->size);
    assert(fqtable->table[c] >= 0);

    if (fqtable->table[c] == 0){
        fqtable->char_count += 1;
    }

    fqtable->table[c] += 1;
    return;
}


int FreqTableGetCount(FreqTable fqtable, int c){
    assert(IsFreqTableValid(fqtable));
    assert(c >= 0 && c < fqtable->size);
    assert(fqtable->table[c] >= 0);

    return fqtable->table[c];
}


void FreqTableShow(FreqTable fqtable){
    assert(IsFreqTableValid(fqtable));

    printf("Frequency Table Print:\n");
    for (int i = 0; i < fqtable->size; i++){
        if (fqtable->table[i] > 0){
            printf("idx = %d, char = %c, count = %d\n", i, i, fqtable->table[i]);
        }
    }

    printf("\n");
    return;
}


int FreqTableGetCharCount(FreqTable fqtable){
    assert(IsFreqTableValid(fqtable));
    return fqtable->char_count;
}


bool IsFreqTableValid(FreqTable fqtable){
    return fqtable != NULL && fqtable->size > 0 && fqtable->char_count >= 0 && fqtable->table != NULL;
}