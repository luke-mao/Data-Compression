#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "tree.h"
#include "FGK_functions.h"


const int power_of_two[] = {1, 2, 4, 8, 16, 32, 64, 128};

void decompress_file(FILE* fp, FILE* fp_out, Tree tr);


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

    fprintf(stdout, "Input file: %s\n", filename);
    fprintf(stdout, "Size %.2f KB\n", (float) original_size / 1024);

    fprintf(stdout, "Output compressed file: %s\n", filename_out);
    fprintf(stdout, "Size %.2f KB\n", (float) new_size / 1024);

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
    // first check if the filename is valid
    // valid name has .FGK at the end
    int len = strlen(filename);
    if(strcmp(".FGK", filename+len-4) != 0){
        fprintf(stdout, "Decompress Error: valid file should have .FGK suffix\n");
        exit(EXIT_FAILURE);
    }

    // for valid files, open the file
    FILE* fp = fopen(filename, "rb");
    assert(fp != NULL);

    // create output file, add "deFGK_" at the front, remove .FGK, +1 for \0
    char* filename_out = (char*) malloc((6+len-4+1) * sizeof(char));
    assert(filename_out != NULL);
    
    // form the name
    strcpy(filename_out, "deFGK_");
    strncat(filename_out, filename, len-4);

    // open the file
    FILE* fp_out = fopen(filename_out, "wb");
    assert(fp_out != NULL);

    // create the tree
    // for decompression, no need to use nodelist
    Tree tr = TreeCreate();

    // main function to decompress the file
    decompress_file(fp, fp_out, tr);


    // finish decompression, print out some statistics
    fprintf(stdout, "Input compressed file: %s\n", filename);
    fseek(fp, 0, SEEK_END);
    fprintf(stdout, "Size: %.2f KB\n", (float) ftell(fp) / 1024);

    fprintf(stdout, "Output decompressed file: %s\n", filename_out);
    fseek(fp_out, 0, SEEK_END);
    fprintf(stdout, "Size: %.2f KB\n", (float) ftell(fp_out) / 1024);


    // destroy the tree
    TreeDestroy(tr);

    // close the file
    fclose(fp);
    fclose(fp_out);

    fp = NULL;
    fp_out = NULL;

    // free the string
    free(filename_out);
    filename_out = NULL;

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

            // so if 3 numbers are padded, output 00000011
            // if four numbers are padded, output 00000100
            // easier for decompression
            putc(pad_num, fp_out);
        }
    }

    return;
}




void decompress_file(FILE* fp, FILE* fp_out, Tree tr){
    assert(fp != NULL && fp_out != NULL);
    assert(tr != NULL);


    // the first byte of file tells how many numbers are padded
    // example: 00000100 means 4 numbers are padded
    //          00000011 means three numbers are padded 
    int pad_num = getc(fp);

    // the second byte is the first letter
    int buffer1 = getc(fp);
    // the first letter needs to be printed out, and insert into the tree
    putc(buffer1, fp_out);

    // insert into the tree
    Node newNYT = NodeCreate(NYT_C, tr->NYT->label-2, 0, NULL, NULL, tr->NYT);
    Node newNode = NodeCreate(buffer1, tr->NYT->label-1, 1, NULL, NULL, tr->NYT);
    // rearrange the link from top-down
    tr->NYT->left = newNYT;
    tr->NYT->right = newNode;
    // reset the NYT
    tr->NYT = newNYT;
    // update the tree
    // since the old NYT is the root node, so just add 1
    tr->root->occ += 1;

    // read the next char
    buffer1 = getc(fp);
    int buffer1_num_not_read = 8;
    int mask;
    int this_bit;

    // read a further char
    int buffer2 = getc(fp);

    // node
    Node n = tr->root;


    while (buffer2 != EOF){
        // extract one bit
        mask = 1 << (buffer1_num_not_read - 1);
        this_bit = ((mask & buffer1) >> (buffer1_num_not_read - 1)) & 1;

        if (this_bit == 0){            
            n = n->left;
        }
        else{
            n = n->right;
        }

        // reduce the number un-readed
        buffer1_num_not_read -= 1;

        // move forward if buffer 1 all readed
        if (buffer1_num_not_read == 0){
            buffer1 = buffer2;
            buffer1_num_not_read = 8;

            buffer2 = getc(fp);
            // here does not need to worry about buffer2 = EOF
        }


        // if reach NYT node, read the next 8 bits
        if (n == tr->NYT){
            // read the next 8 bits
            // first shift buffer1
            buffer1 <<= 8 - buffer1_num_not_read;

            // then extract some bits from buffer2
            // need "8 - buffer_1_num_not_read" bits
            mask = power_of_two[8 - buffer1_num_not_read] - 1;
            // but this mask needs to be shifted to the leftmost position
            // and put it to the end of buffer1, in the rightmost position
            buffer1 |= (((mask << buffer1_num_not_read) & buffer2) >> buffer1_num_not_read) & mask;

            // now buffer1 store the whole 8 bits
            putc(buffer1, fp_out);

            // insert into the tree
            Node newNYT = NodeCreate(NYT_C, tr->NYT->label-2, 0, NULL, NULL, tr->NYT);
            Node newNode = NodeCreate(buffer1, tr->NYT->label-1, 1, NULL, NULL, tr->NYT);
            
            // rearrange the oldNYT connection parts
            tr->NYT->left = newNYT;
            tr->NYT->right = newNode;
            tr->NYT->occ += 1;

            // so right now, the separation of old NYT to two new nodes have been finished
            // also the old NYT weight has been increased
            // reassign the NYT
            tr->NYT = newNYT;

            // check if it is the root node
            // if the old NYT is the root node, do nothing, do not need to update tree any further
            if(tr->NYT->parent->c != ROOT_C){
                tr->NYT->parent->c = INTERNAL_NODE_C;
                // go to its parent node for further update
                TreeUpdateFunction(tr, NULL, tr->NYT->parent->parent); 
            }          

            // once finish, move n to the root node
            n = tr->root;

            // move the remaining bit of buffer2 to buffer1
            mask = power_of_two[buffer1_num_not_read] - 1;
            buffer1 = 0;
            buffer1 |= mask & buffer2;
            // and buffer1_num_not_read remains the same

            // read next byte
            buffer2 = getc(fp);        
        }
        else if (n->c >= 0){
            // reach a leaf node, not NYT
            // print the byte
            putc(n->c, fp_out);

            // also update the tree: first swap and then increment
            TreeUpdateFunction(tr, NULL, n);
            
            // move n to the root
            n = tr->root;
        }

        // if node n is an internal node, then keep going on
    }


    // now buffer2 = EOF, and buffer1 is the last byte, need to use pad_num
    // read the last few byte
    int stop_at_this_num;

    if (pad_num == 0){
        stop_at_this_num = 0;
    }
    else{
        stop_at_this_num = pad_num;
    }


    // loop to scan the last few bits
    while (buffer1_num_not_read > stop_at_this_num){
        // read one bit
        mask = 1 << (buffer1_num_not_read - 1);
        this_bit = ((mask & buffer1) >> (buffer1_num_not_read - 1)) & 1;

        if (this_bit == 0){
            n = n->left;
        }
        else{
            n = n->right;
        }

        buffer1_num_not_read -= 1;

        // so only max 8 bits left, don't need to worry about NYT issue
        // for internal node, do nothing
        if (n->c >= 0){
            // reach a leaf node, not NYT
            // print the byte
            putc(n->c, fp_out);

            // also update the tree: first swap and then increment
            TreeUpdateFunction(tr, NULL, n);
            
            // move n to the root
            n = tr->root;
        }
    }

    return;
}

