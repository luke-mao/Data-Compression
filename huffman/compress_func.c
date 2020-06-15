#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "file.h"
#include "freq_table.h"
#include "data_structure.h"
#include "compress_func.h"


// additional functions
char* get_compressed_file_name(char*);
void write_header(FILE*, const Tree*);
void write_header_tree(FILE*, const Tree*);
void write_body(FILE*, FILE*, const FreqTable*);


char* compress(char* file_name, const FreqTable* t, const Tree* tr){
    // first get the output file name
    char* out_file_name = get_compressed_file_name(file_name);

    // now open both files
    FILE* old = open_file(file_name);
    FILE* new = open_file_write(out_file_name);

    // first output the header to the FILE* new
    write_header(new, tr);
    write_body(new, old, t);         

    // finish, close file
    close_file(old);
    close_file(new);

    fprintf(stdout, "Finish compression, output file \"%s\"\n", out_file_name);

    return out_file_name;
}


// check some statistics of the file, including name and 
void compress_stats(const char* original, const char* compressed){
    // print out the file size change, and calculate the compress ratio
    FILE* before = open_file(original);
    FILE* after = open_file(compressed);

    fseek(before, 0L, SEEK_END);
    fseek(after, 0L, SEEK_END);

    long before_size = ftell(before);
    long after_size = ftell(after);

    fprintf(stdout, "Compression statistics:\n");

    fprintf(
        stdout, 
        "Input filename=\"%s\". Output filename=\"%s\"\n", original, compressed
    );

    fprintf(
        stdout, 
        "Size from %ld => %ld (Bytes)\n", before_size, after_size
    );

    fprintf(
        stdout, 
        "Space saving: %.2f%%\n", ((float)1 - (float)after_size / before_size)*100
    );

    close_file(before);
    close_file(after);
    return;
}   


char* get_compressed_file_name(char* filename){
    // use the original file name, get the compressed name
    // add .huff at the end
    int length = strlen(filename);

    char* new = (char*)malloc((length+5)*sizeof(char));
    strcpy(new, filename);
    strcat(new, ".huff");

    return new;    
}


void write_header(FILE* fp, const Tree* tr){
    assert(fp != NULL);
    assert(tr != NULL);
    
    // given the new file, write the header
    // head includes three parts: 
    //      num of different char, pseudo_eof char, tree
    // output byte by byte

    putc((Byte) tr->leaf_count, fp);    // first byte, number of leaf nodes
    putc((Byte) 'a', fp);      // this is the second byte of the file, will be replaced with the number of pads later
    write_header_tree(fp, tr);
    return;
}


void write_header_tree(FILE* fp, const Tree* tr){
    // use post order traversal with no recursion to write the header
    // similar to the "post_order_traversal_test_no_recursion" in huffman.c 
    // include bit operation
    // for non-leaf node: add bit 0
    // for leaf node: add bit 1 + char

    assert(fp != NULL);
    assert(tr != NULL);

    NodeStack* s = create_stack(2 * tr->leaf_count); // amplify in case core dump
    TreeNode* root = tr->root;

    // this is the output byte b, and a counter to track number of digits
    // putc when counter >= 8
    Byte b = 0;
    int b_count = 0;    // value = 0 to 8

    do {
        // step 1: trace down to the null left root
        while (root != NULL){
            // push root right into the stack
            if (root->right != NULL){
                stack_push(s, root->right);
            }
            // push root into the stack
            stack_push(s, root);
            // go down to the root left
            root = root->left;
        }

        // pop one item out and set it as root
        root = stack_pop(s);
        if (root->right != NULL && root->right == stack_top(s)){
            // remove the top item
            stack_pop(s);
            // push the root back
            stack_push(s, root);
            // set root as root right node
            root = root->right;
        }
        else{
            // print the data, but consider leaf and non-leaf situations
            if (root->left != NULL || root->right != NULL){
                // non-leaf node, add bit 1
                b = b << 1;
                b |= 0;
                b_count++;
                if (b_count == 8){
                    // print to file
                    putc(b, fp);
                    // reset
                    b = 0;
                    b_count = 0;
                } 
            }
            else{ 
                // leaf node, add 1 and the char
                b = b << 1;
                b |= 1;
                b_count++;
                if (b_count == 8){
                    putc(b, fp);
                    putc(root->b, fp);
                    b = 0;
                    b_count = 0; 
                }
                else{
                    // b_count < 8, we need to add 8 bits
                    // first shift left and determine how many bits
                    b = b << (8 - b_count);
                    Byte add_to_b = (root->b) >> b_count;
                    b |= add_to_b;
                    putc(b, fp);
                    b = 0;
                    // now add the remaining bits
                    Byte mask = 255 >> (8 - b_count);
                    b = (root->b) & mask;
                    // b_count remain not change
                }
            }

            // after printing, set root as NULL
            root = NULL;
        }
    } while (! stack_is_empty(s));

    // when the loop finish, if b_count != 0, then need to pad
    if (b_count != 0){
        b = b << (8 - b_count);
        putc(b, fp);
    }

    destroy_stack(s);
    return;
}


void write_body(FILE* new, FILE* old, const FreqTable* t){
    assert(new != NULL && old != NULL);
    assert(t != NULL);

    Byte old_byte;      // as a buffer, read the old file
    // the following two is the actual output, shorter code
    Byte b = 0;
    int b_count = 0;
    Byte cw = 0;        // codeword
    int cw_count = 0;

    // does not need to rewind, fp is already at the beginning
    // byte is unsigned char
    // get the file size and then use the for loop
    long size = file_size(old);
    for (long i = 0; i < size; i++){
        old_byte = getc(old);           // get the old byte

        // get the cw and cw_count
        cw = t->buckets[(int) old_byte]->codeword;
        cw_count = t->buckets[(int) old_byte]->cw_count;

        if (b_count + cw_count <= 8){
            b <<= cw_count;
            b |= cw;
            b_count += cw_count;

            if (b_count == 8){
                putc(b, new);
                b = 0;
                b_count = 0;
            }
        }
        else{ // b_count + cw_count > 8
            b <<= (8 - b_count);
            Byte add_to_b = cw >> b_count;
            b |= add_to_b;
            putc(b, new);
            b = 0;
            // now add the remaining bits
            Byte mask = 255 >> (8 - (cw_count + b_count - 8));
            b = cw & mask;
            b_count = cw_count + b_count - 8;     
        }
    }

    // deal with the eof sign, check how many bits are needed to pad
    // and then modify the second byte of the file, 00000111 => padded three bits
    
    // pad the last byte
    if (b_count < 8){
        b <<= (8 - b_count);
    }
    putc(b, new);           // output the last byte of the file

    Byte padded = 255;      // 11111111
    padded >>= b_count;     // 00000111 => padded three bits
    fseek(new, 1L, SEEK_SET);
    putc(padded, new);

    return;
}
