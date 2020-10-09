#ifndef _FREQUENCY_TABLE_H_
#define _FREQUENCY_TABLE_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "util.h"


// read the file and count the frequency of each char
struct _FreqTable{
    int size;
    int *table;
};

typedef struct _FreqTable *FreqTable;


FreqTable FreqTableCreate(int size);
FreqTable FreqTableDestroy(FreqTable);

void FreqTableInsert(FreqTable, int c);
int FreqTableGetCount(FreqTable, int c);

void FreqTableShow(FreqTable);


#endif 