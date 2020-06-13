// first read the file and calculate the frequency table

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>


int main(int argc, char *argv[]){
    // first read the file from stdin and then printout the frequency table
    // right now, simply input one file name, such as t1.txt

    FILE *file_in;
    file_in = fopen(argv[1], "rb");
    if (file_in == NULL){
        fprintf(stderr, "Error opening the file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // now the file is successfully open, allocate the anscii  list
    int **anscii_freq_array;
    anscii_freq_array = (int **) malloc (256 * sizeof(int*));

    if (anscii_freq_array == NULL){
        fprintf(stderr, "error memory allocation, anscii_freq_array\n");
        exit(EXIT_FAILURE);
    }

    // initialize each pointer to NULL
    for (int i = 0; i < 256; i++){
        anscii_freq_array[i] = NULL;
    }

    char buffer = getc(file_in);

    while (buffer != EOF){
        if (anscii_freq_array[(int)buffer] == NULL){
            // if null, need to malloc
            anscii_freq_array[(int) buffer] = (int*) malloc (1 * sizeof(int));
            *(anscii_freq_array[(int) buffer]) = 1;     // start from 1
        }
        else{
            *(anscii_freq_array[(int) buffer]) += 1;
        }

        buffer = getc(file_in);
    }

    // close the file
    fclose(file_in);
    file_in = NULL;
    char pesudo_eof;

    // we use the first no appeared char as the pesudo eof
    // now print out the result
    bool pesudo_eof_flag = false;   // not found a char to represent the pesudo eof yet
    for (int i = 0; i < 256; i++){
        if ( anscii_freq_array[i] == NULL && (! pesudo_eof_flag)){
            fprintf(stdout, "pseudo_eof: 1\n");
            pesudo_eof = i;
            pesudo_eof_flag = true;     // the eof representative has found, so change to true
        }
        else if ( anscii_freq_array[i] != NULL){
            fprintf(stdout, "%c : %d\n", i, *(anscii_freq_array[i]));

            // here first free each pointer
            free(anscii_freq_array[i]);
            anscii_freq_array[i] = NULL;
        }
        else{
            continue;
        }
    }

    if (! pesudo_eof_flag){
        fprintf(stderr, "Not found a good representative of eof\n");
        exit(EXIT_FAILURE);
    }

    // free each pointer
    free(anscii_freq_array);

    return 0;
}