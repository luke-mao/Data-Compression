#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "tree.h"
#include "list.h"
#include "dictionary.h"
#include "update.h"
#include "file.h"
#include "decompress.h"


// additional function for compression
// get next bit, or next 1 byte (8 bits)
int GetOneBit(int* c_p, int* unread_num_p, int* c_next_p, FILE* fp);
int GetOneByte(int* c_p, int* unread_num_p, int* c_next_p, FILE* fp);


// before create the output file name, check if the file is valid
// by check if it has .v suffix
bool IsValidFile(char* filename_in);


char* decompression_create_output_filename(char* filename_in){
    assert(filename_in != NULL);
    assert(IsValidFile(filename_in));

    // create output name: drop .v, add prefix deVitter_ at the front
    // the file name can be xx/xx/xx/test1.v
    // scan backwards to find the slash, or the first bit;
    long length = strlen(filename_in);

    // delete ".v", add "deVitter_", and add 1 for \0
    char* filename_out = (char*) malloc((length - 2 + 9 + 1) * sizeof(char));
    assert(filename_out != NULL);

    long idx = length-1;
    while (idx >= 0 && filename_in[idx] != '/'){
        idx -= 1;
    }


    if (idx == -1){
        // file in the same folder
        strcpy(filename_out, "deVitter_");
        strncat(filename_out, filename_in, length-2);   // do not copy ".v"
    }
    else{
        // file in different folder
        strncpy(filename_out, filename_in, idx+1);
        strcat(filename_out, "deVitter_");
        strncat(filename_out, filename_in+idx+1, length- (idx + 1) - 2);
    }

    return filename_out;
}


void decompress_file_and_output(FILE* fp_in, FILE* fp_out){
    assert(fp_in != NULL && fp_out != NULL);

    // create structure
    Tree tr = TreeCreate();
    List L = ListCreate();
    Dictionary d = DictionaryCreate(ASCII_SIZE);

    // the first byte records the number of zeros pad at the end
    int pad_number = getc(fp_in);

    // buffer for reading, c = current, c_next = next byte
    int c = getc(fp_in);
    int unread_num;
    int c_next;
    
    // first byte is output straight away
    putc(c, fp_out);
    
    // also update the tree for the first char
    TreeUpdateForFirstChar(tr, c);
    ListNode first_char_LN = ListUpdateForFirstChar(L, tr);
    DictionaryInsert(d, first_char_LN);

    c = getc(fp_in);
    unread_num = 8;
    c_next = getc(fp_in);

    int this_bit, this_byte;
    TreeNode trn = GetRoot(tr);
    ListNode LN_p = NULL;

    while (c_next != EOF){
        this_bit = GetOneBit(&c, &unread_num, &c_next, fp_in);

        if (this_bit == 0){
            trn = GetLeft(trn);
        }
        else{
            trn = GetRight(trn);
        }

        if (IsNYTNode(trn) || IsSymbolNode(trn)){
            if (IsNYTNode(trn)){
                // meet NYT, extract next 8 byte
                this_byte = GetOneByte(&c, &unread_num, &c_next, fp_in);
            }
            else if (IsSymbolNode(trn)){
                // meet an existing symbol
                this_byte = GetC(trn);
            }

            // print it out
            putc(this_byte, fp_out);

            // update
            LN_p = GetListNode(L, trn);
            UpdateTreeAndList(tr, L, d, LN_p, this_byte);

            // treenode back to the beginning
            trn = GetRoot(tr);
        }
    }


    // now c_next is EOF, so only read remaining parts in c
    // but read only the number of unread bits minus padded bits
    int mask = pow(2, unread_num - pad_number) - 1;
    c &= (mask << pad_number);
    c >>= pad_number;
    c &= mask;

    unread_num -= pad_number;


    while (unread_num >= 1){
        this_bit = GetOneBit(&c, &unread_num, &c_next, fp_in);

        if (this_bit == 0){
            trn = GetLeft(trn);
        }
        else{
            trn = GetRight(trn);
        }

        if (IsSymbolNode(trn)){
            this_byte = GetC(trn);

            // print it out
            putc(this_byte, fp_out);

            // update
            LN_p = GetListNode(L, trn);
            UpdateTreeAndList(tr, L, d, LN_p, this_byte);

            // treenode back to the beginning
            trn = GetRoot(tr);
        }
    }

    return;
}


void decompression_status(char* name_in, char* name_out, FILE* fp_in, FILE* fp_out){
    assert(name_in != NULL && name_out != NULL);
    assert(fp_in != NULL && fp_out != NULL);

    printf("Input compressed file: %s\nOutput decompressed file: %s\n", name_in, name_out);
    return;
}


bool IsValidFile(char* filename_in){
    assert(filename_in != NULL);
    
    // simply check the last two chars is ".v" or not
    long len = strlen(filename_in);
    if (filename_in[len-2] == '.' && filename_in[len-1] == 'v'){
        return true;
    }
    else{
        return false;
    }
}


int GetOneBit(int* c_p, int* unread_num_p, int* c_next_p, FILE* fp){
    assert(c_p != NULL && unread_num_p != NULL && c_next_p != NULL);
    assert((*unread_num_p) >= 0);
    assert(fp != NULL);

    // read the bit from left to right
    int mask = 1 << ((*unread_num_p) - 1);
    int this_bit = ((mask & (*c_p)) >> ((*unread_num_p) - 1)) & 1;
    (*unread_num_p) -= 1;

    if ((*unread_num_p) == 0 && (*c_next_p) != EOF){
        (*c_p) = (*c_next_p);
        (*c_next_p) = getc(fp);
        (*unread_num_p) = 8;
    }

    return this_bit;
}


int GetOneByte(int* c_p, int* unread_num_p, int* c_next_p, FILE* fp){
    assert(c_p != NULL && unread_num_p != NULL && c_next_p != NULL);
    assert((*unread_num_p) >= 0 && (*unread_num_p) <= 8);
    assert(fp != NULL);

    // need to extract 8 digits
    int result;

    if ((*unread_num_p) == 8){
        result = (*c_p);
        (*unread_num_p) = 0;

        (*c_p) = (*c_next_p);
        (*c_next_p) = getc(fp);
        (*unread_num_p) = 8;
    }
    else{
        // get part from *c_p
        int mask = pow(2, (*unread_num_p)) - 1;
        result = ((*c_p) & mask);
        result <<= 8 - (*unread_num_p);

        // and the rest from *c_next_p
        mask = pow(2, 8-(*unread_num_p)) - 1;
        int other_part = (*c_next_p) & (mask << (*unread_num_p));
        other_part >>= (*unread_num_p);
        other_part &= mask;
        result |= other_part;

        // unread num is still the same
        (*c_p) = (*c_next_p);
        (*c_next_p) = getc(fp);
    }

    return result;
}