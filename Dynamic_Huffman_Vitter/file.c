#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"
#include "list.h"


FILE* open_the_file(char* filename, char* mode){
    assert(filename != NULL &&  mode != NULL);

    FILE* fp = fopen(filename, mode);
    assert(fp != NULL);
    return fp;
}


FILE* close_the_file(FILE* fp){
    assert(fp != NULL);
    fclose(fp);
    fp = NULL;
    return fp;
}



// single digit
void FilePrintDigit(int* buffer_p, int* buffer_len_p, FILE* fp, int new_digit){
    assert(buffer_p != NULL && buffer_len_p != NULL);
    assert(fp != NULL);
    assert(new_digit == 0 || new_digit == 1);

    (*buffer_p) <<= 1;
    (*buffer_p) |= (new_digit & 1);
    (*buffer_len_p) += 1;

    // print when 8
    if (*buffer_len_p == 8){
        fputc(*buffer_p, fp);
        
        // reset
        (*buffer_p) = 0;
        (*buffer_len_p) = 0;
    }

    return;
}


// for new node, print the byte
void FilePrintByte(int* buffer_p, int* buffer_len_p, FILE* fp, int new_byte){
    assert(buffer_p != NULL && buffer_len_p != NULL);
    assert(fp != NULL);
    assert(new_byte >= 0);

    if (*buffer_len_p == 0){
        fputc(new_byte, fp);
    }
    else{
        int this_digit;
        int mask;
        // input into FilePrintDigit one by one
        for (int i = 7; i >= 0; i--){
            mask = 1 << i;
            this_digit = ((new_byte & mask) >> i) & 1; 
            FilePrintDigit(buffer_p, buffer_len_p, fp, this_digit);
        }
    }

    return;
}


// print the node path from the root node
// left = 0, right = 1
void FilePrintNodePath(int* buffer_p, int* buffer_len_p, FILE* fp, TreeNode trn){
    assert(buffer_p != NULL && buffer_len_p != NULL);
    assert(fp != NULL);
    assert(trn != NULL);
    
    // recursion, go upwards from the tree first
    if (trn->parent->c != ROOT_C){
        FilePrintNodePath(buffer_p, buffer_len_p, fp, trn->parent);
    }

    // now trn's parent is the root node
    if (trn == trn->parent->left){
        FilePrintDigit(buffer_p, buffer_len_p, fp, 0);
    }
    else{
        FilePrintDigit(buffer_p, buffer_len_p, fp, 1);
    }

    return;
}


void FilePrintEmptyByte(FILE* fp){
    assert(fp != NULL);

    // put 0 for now, will change at the end of compression
    fputc(0, fp);
    return;
}


void FileRePrintFirstByte(int num, FILE* fp){
    assert(num >= 0 && num <= 7);
    assert(fp != NULL);

    fseek(fp, 0, SEEK_SET);
    fputc(num, fp);

    return;
}


// for the last byte, need to pad and print to the front
// return the number of bits pad, should be 0 to 7
int FilePrintPad(int* buffer_p, int* buffer_len_p, FILE* fp){
    assert(buffer_p != NULL && buffer_len_p != NULL);
    assert((*buffer_len_p) >= 0 && (*buffer_len_p) <= 7);
    assert(fp != NULL);

    int pad_num = 0;

    if ((*buffer_len_p) != 0){
        pad_num = 8 - (*buffer_len_p);
        (*buffer_p) <<= pad_num;
        fputc(*buffer_p, fp);
    }

    return pad_num;
}
