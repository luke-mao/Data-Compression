/*
    Main function for compression.
    Input the file name as ./compress <file name>
*/


#include <stdio.h>
#include "file.h"
#include "freq_table.h"
#include "data_structure.h"
#include "compress_func.h"


int main(int argc, char *argv[]){

    if (argc == 0){
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    }

    // open file
    FILE* fp;
    fp = open_file(argv[1]);

    // // read file and create the freq table
    FreqTable* t = create_table();
    fill_table(fp, t);
    close_file(fp);         // close the file, this is first use of the file
    
    // print out the table
    // print_table_freq(t);
    // fprintf(stdout, "\n");

    // create the priority queue and the tree
    NodePQ* pq = create_pq(t);
    // pq_print(pq);

    Tree* tr = create_tree(t->counter);
    fill_tree(tr, pq);

    // print out the tree
    // post_order_traversal_test(tr);
    // fprintf(stdout, "\n");

    // now base on the tree, get the codeword and fill into freq_table
    // freq table has "codeword" in each bucket

    get_codeword(tr, t);
    print_table_codeword(t);
    fprintf(stdout, "\n");

    // compress
    char* out_file_name = compress(argv[1], t, tr);
    compress_stats(argv[1], out_file_name);         // second use of the file

    destroy_table(t);
    destroy_pq(pq);
    destroy_tree(tr);
    return 0;
}

