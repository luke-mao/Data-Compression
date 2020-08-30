#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "file.h"
#include "data_structure.h"
#include "decompress_func.h"


int main(int argc, char* argv[]){
    // read the command line input of the file, one file at each time
    if (argc != 2){
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // examine the file name 
    if (! check_valid_file_name(argv[1])){
        fprintf(stderr, "Usage: %s <file.huff>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* file_in = argv[1];
    char* file_out = create_output_file_name(file_in);

    FILE* fp_in = open_file(file_in);
    FILE* fp_out = open_file_write(file_out);

    // now read the original file and create the tree
    Tree* tr = read_header(fp_in);

    // print the tree, the tree should be the same as in compress
    // post_order_traversal_test(tr);

    // use the tree, to decompress the file
    decompress_body(fp_in, fp_out, tr);
    decompress_stats(file_in, file_out);

    close_file(fp_in);
    close_file(fp_out);
    destroy_tree(tr);
    return 0;
}