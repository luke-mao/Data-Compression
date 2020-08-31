#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "tree.h"
#include "FGK_functions.h"


const int power_of_two[] = {1, 2, 4, 8, 16, 32, 64, 128};


void FGK_compress(const char* filename){
    // first open the input file
    FILE* fp = fopen(filename, "rb");
    assert(fp != NULL);

    // create filename for output
    // original name + .FGK, also +1 for \0
    char* filename_out = (char*) malloc(strlen(filename+4+1) * sizeof(char));
    assert(filename_out != NULL);

    strcpy(filename_out, filename);
    strcat(filename_out, ".FGK");
    
    // create the file fp for output
    FILE* fp_out = fopen(filename_out, "wb");
    assert(fp_out != NULL);
    // output a char first
    // this char represent the number of digits padded
    // so initialize to zero.
    // and when all encoding finish, re-write the char if necessary
    putc(0, fp_out);

    // prepare the tree
    Tree tr = TreeCreate();
    NodeList ndlist = NodeListCreate();

    // initialize output buffer
    char out_c = 0;
    // number of digits in the output buffer, range 0 - 8
    // when equal to 8, call putc
    int out_c_num = 0;

    // input buffer
    int c;

    // read file and update the tree
    while ((c = getc(fp)) != EOF){
        TreeUpdate(tr, ndlist, c, fp_out, &out_c, &out_c_num);
    }

    // finish encoding, pad the last char and re-write the first char if necessary
    pad_last_bit(fp_out, &out_c, &out_c_num);

    // print some compression status
    fseek(fp, 0, SEEK_END);
    long original_size = ftell(fp);

    fseek(fp_out, 0, SEEK_END);
    long new_size = ftell(fp_out);

    fprintf(stdout, "Input file: %s \t Size %.2f KB\n", filename, (float) original_size / 1024);
    fprintf(stdout, "Output file: %s \t Size %.2f KB\n", filename_out, (float) new_size / 1024);
    fprintf(stdout, "Space saving %.2f%%\n", (1 - ((float) new_size / original_size)) * 100);

    // finish, destroy everything
    TreeDestroy(tr);
    NodeListDestroy(ndlist);

    // close files
    fclose(fp);
    fclose(fp_out);
    fp = NULL;
    fp_out = NULL;

    // free the char*
    free(filename_out);
    filename_out = NULL;

    return;
}



void FGK_decompress(const char* filename){
    puts("not finish");
    return;
}



void print_to_file(FILE* fp_out, char* out_c, int* out_c_num, int new_c, int new_c_num){

    int transfer_bit_num = 0; 
    int transfer_bit_buffer = 0;
    int trasnfer_bit = 0;
    int shift_num = 0;


    while ((*out_c_num) + new_c_num >= 8){
        transfer_bit_num = 8 - *out_c_num;
        transfer_bit_buffer = power_of_two[transfer_bit_num] - 1;
        shift_num = new_c_num - transfer_bit_num;
        trasnfer_bit = (((transfer_bit_buffer << shift_num) & new_c) >> shift_num) & transfer_bit_buffer; 

        // put these onto out_c
        *out_c <<= transfer_bit_num;
        *out_c |= trasnfer_bit;
        putc(*out_c, fp_out);
        
        // reset
        new_c_num -= transfer_bit_num;

        trasnfer_bit = 0;
        transfer_bit_buffer = 0;
        transfer_bit_num = 0;
        shift_num = 0;

        *out_c = 0;
        *out_c_num = 0;
    }

    // now append the rest
    if (new_c_num > 0){
        transfer_bit_num = new_c_num;
        // no need to shift the transfer_bit
        // the bits will only be the last few bits
        transfer_bit_buffer = power_of_two[transfer_bit_num] - 1;
        
        *out_c <<= transfer_bit_num;
        *out_c |= new_c & transfer_bit_buffer;

        *out_c_num += transfer_bit_num;
    }

    return;
}


void pad_last_bit(FILE* fp_out, char* out_c, int* out_c_num){
    // pad last bit and print out
    // and then move to the first byte of the FILE and redo the first byte

    if(*out_c_num != 0){
        if (*out_c_num == 8){
            putc(*out_c, fp_out);
            // the first byte is initially set to 0, so no need to change
        }
        else{
            // need to pad
            int pad_num = 8 - *out_c_num;
            *out_c <<= pad_num;
            putc(*out_c, fp_out);
            fseek(fp_out, 0, SEEK_SET);
            putc(power_of_two[pad_num]-1, fp_out);
        }
    }

    return;
}