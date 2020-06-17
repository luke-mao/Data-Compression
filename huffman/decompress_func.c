#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "byte.h"
#include "file.h"
#include "data_structure.h"
#include "decompress_func.h"


bool check_valid_file_name(const char *filename){
    int len = strlen(filename);
    if (len >= 6 && strcmp(".huff", filename + len - 5) == 0){
        return true;
    }
    return false;
}


char* create_output_file_name(const char* filename){
    // add a prefix "dehuff_" to the file name
    // and remove the last suffix ".huff"

    int old_name_len = strlen(filename);
    int new_name_len = 7 + old_name_len - 5; // 7=dehuff_, 5=.huff
    char* new_file_name = (char*) malloc (new_name_len * sizeof(char));
    strcpy(new_file_name, "dehuff_");
    strncat(new_file_name, filename, old_name_len - 5);

    return new_file_name;
}


Tree* read_header(FILE* fp){
    Byte buffer, bit, mask;
    
    // read the first byte: indicate the total number of different characters
    buffer = getc(fp);
    int leaf_num = (int) buffer;

    // skip the second byte, no use at this moment
    buffer = getc(fp);

    // prepare the stack, enlarge the size in case core dump
    NodeStack* s = create_stack(3 * leaf_num);

    // while we have not found all the leaf nodes
    bool tree_finish = false;
    int bit_pos = -1;           // bit index, when one byte finish in whole, set to -1
    Tree* tr;

    while (! tree_finish){

        if (bit_pos == -1){
            buffer = getc(fp);
            // read every bit from the left towards right
            bit_pos = 7;
        }

        while (bit_pos >= 0){
            mask = 1 << bit_pos;              // locate the bit
            bit = (buffer & mask) >> bit_pos; // get the single bit

            // if the bit = 1 we read the next whole byte
            if ((int) bit == 1){
                break;      // break the loop first
            }
            else{   // bit = 0
                if (stack_current_size(s) == 1){
                    // the tree if finalized
                    tr = create_tree(leaf_num);
                    tr->root = stack_pop(s);
                    tree_finish = true;
                    break;
                }
                else{
                    // there must be >= 2 nodes in the stack, the stack should not be empty
                    assert(stack_current_size(s) >= 2);

                    // pop first to be the right child, second to be the left child
                    TreeNode* right = stack_pop(s);
                    TreeNode* left = stack_pop(s);
                    // create a new node to include the both two childs
                    TreeNode* new_node = create_tree_node(0, 0, left, right);
                    // push the new node into the stack
                    stack_push(s, new_node);
                }
            }

            bit_pos--;    // update bit index bit_pos
        }

        if (tree_finish){
            break;
        }

        if ((int) bit == 1){
            bit_pos--;          // first minus the bit (1)

            if (bit_pos == -1){
                // lucky, so read the entire next byte is enough
                buffer = getc(fp);
                // create a node and insert this into the stack
                TreeNode* new_node = create_tree_node(buffer, 0, NULL, NULL);
                stack_push(s, new_node);

                // bit_pos remain -1, so when reach the start of the loop,
                // the program will read a new byte
            }
            else{
                // some bits locate in the buffer, some in the next byte
                int num_bits_need_in_old = bit_pos + 1;
                // extract that number of bits
                mask = 255 >> (8 - num_bits_need_in_old);
                // make the bits locate at the leftmost position
                Byte this_char = (buffer & mask) << (8 - num_bits_need_in_old);

                // get the next byte
                buffer = getc(fp);
                int num_bits_need_in_new = 8 - num_bits_need_in_old;
                // the bits are located at the left end
                mask = 255 << (8 - num_bits_need_in_new);
                // after extract, push them towards left， use OR!!!!!!
                this_char |= ( (buffer & mask) >> (8 - num_bits_need_in_new) );
                
                // create new node
                TreeNode* new_node = create_tree_node(this_char, 0, NULL, NULL);
                stack_push(s, new_node);

                // here since we get the new byte, need to deal with the index bit_pos
                bit_pos = 8 - num_bits_need_in_new - 1;
            }
        }
    }

    destroy_stack(s);
    return tr;
}


void decompress_body(FILE* fp_in, FILE* fp_out, const Tree* tr){
    // For every bit, meet 0 then go down left child, meet 1 then go down right child
    // If reach leaf node, then print the byte
    // Then return to the root to begin
    // Use two buffer to check the EOF sign
    // fp_in is already at the body paragraph position

    // !! Byte is unsigned char
    // if buffer2 = unsigned char, then no way to detect EOF (-1) !!!
    Byte buffer1, bit, mask;
    int buffer2;            
    buffer1 = getc(fp_in);
    buffer2 = getc(fp_in);

    TreeNode* root = tr->root;
    
    while (buffer2 != EOF){
        // bit operation on buffer 1

        for (int bit_pos = 7; bit_pos >= 0; bit_pos--){
            // extract the bit
            mask = 1 << bit_pos;
            bit = ( (buffer1 & mask) >> bit_pos );
            
            // go deeper
            if ((int) bit == 0){
                root = root->left;
            }
            else{
                root = root->right;
            }

            if (root->left == NULL && root->right == NULL){
                putc(root->b, fp_out);
                root = tr->root;
            }
        }

        // update two buffer
        buffer1 = (Byte) buffer2;
        buffer2 = getc(fp_in);
    }

    // now buffer1 is the last byte of the file
    // move the fp to the second byte
    fseek(fp_in, 1L, SEEK_SET);
    buffer2 = getc(fp_in);      // use buffer2 to extract the padding symbol
    // 00000111 means the last three are padded

    int bit_pos_start;


    // calculate how many bits are padded
    if ((int) buffer2 == 0){
        // no padding, then simply follow everything above again
        bit_pos_start = 7; 
    }
    else{
        // padded zero at the end of last byte
        // count how many bits are padded
        int bit_pos = 0;
        while (bit_pos <= 7){
            if ( ((1<<bit_pos) & buffer2) == 1 ){
                bit_pos++;
            }
            else{
                break;
            }
            
            bit_pos++;
        }

        buffer1 >>= bit_pos;
        bit_pos_start = 7 - bit_pos;

    }

    // do bit operation on buffer1 for the last few iterations
    for (int bit_pos = bit_pos_start; bit_pos >= 0; bit_pos--){
        // extract the bit
        mask = 1 << bit_pos;
        bit = (buffer1 & mask) >> bit_pos;
        
        // go deeper
        if ((int) bit == 0){
            root = root->left;
        }
        else{
            root = root->right;
        }

        if (root->left == NULL && root->right == NULL){
            putc(root->b, fp_out);
            root = tr->root;
        }
    }

    // finish the whole file
    return;
}









