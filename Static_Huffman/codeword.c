#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "file.h"
#include "tree.h"
#include "priority_queue.h"
#include "frequency_table.h"
#include "util.h"
#include "codeword.h"


CodeWordNode CodeWordNodeCreate(TreeNode trn){
    assert(IsLeafNode(trn));        // only leaf node has a codeword

    int bit_num = TreeNodeGetDepth(trn);
    int byte_num = (bit_num + 8 - 1) / 8;  // find ceil

    CodeWordNode cwn = (CodeWordNode) malloc(sizeof(struct _CodeWordNode));
    assert(cwn != NULL);

    cwn->c = GetC(trn);
    cwn->bit_num = bit_num;
    cwn->byte_num = byte_num;
    
    cwn->s = (int*) malloc(byte_num * sizeof(int));
    assert(cwn != NULL);

    // initialize to all zero
    for (int i = 0; i < byte_num; i++){
        cwn->s[i] = 0;
    }

    int buffer = 0;
    int buffer_num = 0;
    int s_idx = byte_num - 1;
    int this_bit;

    TreeNode current = trn;

    // allocate the bits backwards
    // so cwn->s[0] may not be exact 8 bits
    // but all other are exact 8 bits

    while (! IsRootNode(current)){
        this_bit = 0;

        if (! IsLeftChild(current, current->parent)){
            // right child, add 1
            this_bit = 1 << buffer_num;
        }

        buffer_num += 1;
        buffer |= this_bit;

        if (buffer_num == 8){
            cwn->s[s_idx] = buffer & 255;   // remove any extra bits longer than 8 bits, & 11111111
            s_idx -= 1;

            buffer = 0;
            buffer_num = 0;
        }

        current = current->parent;
    }

    // if depth = 24, then finish, s_idx = -1
    // if not exact multiple of 8, assign the last buffer
    assert(s_idx == 0 || s_idx == -1);

    if (s_idx == 0){
        cwn->s[s_idx] = buffer; 
    }

    return cwn;
}


CodeWordNode CodeWordNodeDestroy(CodeWordNode cwn){
    assert(IsCodeWordNodeValid(cwn));

    free(cwn->s);
    cwn->s = NULL;

    free(cwn);
    cwn = NULL;

    return cwn;
}


// print in the form: xxx xxxxxxxx xxxxxxxx ...
void CodeWordNodeShow(CodeWordNode cwn){
    assert(IsCodeWordNodeValid(cwn));

    // cwn->s[0] may not be exact 8 bits
    PrintByteInBits(cwn->s[0], cwn->bit_num - (cwn->byte_num - 1) * 8);

    // for full bytes, print 8 bits
    if (cwn->byte_num >= 2){
        for (int i = 1; i < cwn->byte_num; i++){
            printf(" ");
            PrintByteInBits(cwn->s[i], 8); 
        }
    }

    return;
}


CodeWord CodeWordCreate(int size){
    assert(size > 0);

    CodeWord cw = (CodeWord) malloc(sizeof(struct _CodeWord));
    assert(cw != NULL);

    cw->size = size;
    
    cw->list = (CodeWordNode*) malloc(size * sizeof(CodeWordNode));
    assert(cw != NULL);

    for (int i = 0; i < size; i++){
        cw->list[i] = NULL;
    }

    return cw;
}


CodeWord CodeWordDestroy(CodeWord cw){
    assert(cw != NULL);
    assert(cw->size > 0 && cw->list != NULL);

    for (int i = 0; i < cw->size; i++){
        if (cw->list[i] != NULL){
            CodeWordNodeDestroy(cw->list[i]);
            cw->list[i] = NULL;
        }
    }

    free(cw->list);
    cw->list = NULL;

    free(cw);
    cw = NULL;
    return cw;
}


void CodeWordInsertNode(CodeWord cw, CodeWordNode cwn){
    assert(cw != NULL && cw->list != NULL);
    assert(cwn != NULL && cwn->c >= 0);
    assert(cw->size > cwn->c);

    cw->list[cwn->c] = cwn;
    return;
}


CodeWordNode CodeWordGetNode(CodeWord cw, int c){
    assert(cw != NULL && cw->list != NULL);
    assert(c >= 0 && c < cw->size);
    assert(IsCodeWordNodeValid(cw->list[c]));

    return cw->list[c];
}


void CodeWordShow(CodeWord cw){
    assert(cw != NULL);
    assert(cw->size > 0 && cw->list != NULL);

    for (int i = 0; i < cw->size; i++){
        if (cw->list[i] != NULL){
            printf("idx = %d, char = %c, cw = ", i, i);
            CodeWordNodeShow(cw->list[i]);
            printf("\n");
        }
    }

    printf("\n");
    return;
}


void PrintCodeWordToFile(FILE* fp, int* buffer_p, int* buffer_len_p, CodeWordNode cwn){
    assert(fp != NULL);
    assert(buffer_p != NULL);
    assert(buffer_len_p != NULL && (*buffer_len_p) >= 0 && (*buffer_len_p) <= 8);
    assert(cwn != NULL && cwn->s != NULL);

    if ((*buffer_len_p) == 8){
        putc(*buffer_p, fp);
        (*buffer_p) = 0;
        (*buffer_len_p) = 0;
    }    

    // cwn->s[0] may not be exactly 8 bits, but all other are eight bits
    // separate into two cases: exact, and not exact
    if (cwn->bit_num == cwn->byte_num * 8){
        for (int i = 0; i < cwn->byte_num; i++){
            PrintOneByte(fp, buffer_p, buffer_len_p, cwn->s[i]);
        }
    }
    else{
        // cwn->s[0] has total len - (num-1) * 8 bits
        int extra_num = cwn->bit_num - (cwn->byte_num - 1) * 8;
        int mask;
        int this_bit;

        for (int i = extra_num - 1; i >= 0; i--){
            mask = 1 << i;
            this_bit = mask & cwn->s[0];
            this_bit >>= i;
            this_bit &= 1;
            PrintOneBit(fp, buffer_p, buffer_len_p, this_bit);  
        }

        // for the remaining, they are exactly 8 bits
        if (cwn->byte_num >= 2){
            for (int i = 1; i < cwn->byte_num; i++){
                PrintOneByte(fp, buffer_p, buffer_len_p, cwn->s[i]);
            }
        }
    }

    return;
}


bool IsCodeWordNodeValid(CodeWordNode cwn){
    return cwn != NULL && cwn->bit_num > 0 && cwn->byte_num > 0 && cwn->s != NULL;
}