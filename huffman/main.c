// this is still the draft
// later on i will separate the compress and decompress

#include "main.h"

int main(int argc, char *argv[]){

    if (argc == 0){
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    }

    FILE* fp;
    fp = open_file(argv[1]);

    FreqTable* t = create_table();
    fill_table(fp, t);
    print_table(t);

    // now having the freq table, 
    // write a priority queue with smallest freq on the top
    // but before that, write the tree first




    fp = close_file(fp);
    t = destroy_table(t);
    return 0;
}