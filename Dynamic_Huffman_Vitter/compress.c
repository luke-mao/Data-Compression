#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "tree.h"
#include "list.h"
#include "dictionary.h"
#include "file.h"
#include "update.h"
#include "compress.h"


// add .v suffix
char* compression_create_output_filename(char* filename_in){
    assert(filename_in != NULL);

    int len = strlen(filename_in);
    char* filename_out = (char*) malloc((len+2+1) * sizeof(char));

    strcpy(filename_out, filename_in);
    strcat(filename_out, ".v");

    return filename_out;
}


void compress_file_and_output(FILE* fp_in, FILE* fp_out){
    assert(fp_in != NULL && fp_out != NULL);

    // create the tree, dictionary and the list
    Tree tr = TreeCreate();
    List L = ListCreate();
    Dictionary d = DictionaryCreate(ASCII_SIZE);

    // buffer for print
    // when reach 8, print the byte
    int buffer = 0;
    int buffer_len = 0;

    // before reading the first char
    // print a byte to output file first, later used to record num of digits pad
    FilePrintEmptyByte(fp_out);

    // for the first char, build the tree and list naively
    int c = getc(fp_in);
    TreeUpdateForFirstChar(tr, c);
    ListUpdateForFirstChar(L, tr);
    DictionaryInsert(d, L->next->next);
    FilePrintByte(&buffer, &buffer_len, fp_out, c);

    // debug
    printf("After first char = %c %d\n", c, c);
    // TreeShow(tr);
    // ListShow(L);
    // printf("\n");


    // read and compress
    while ((c=getc(fp_in)) != EOF){
        // debug
        printf("Insert char = %c %d\n", c, c);
        UpdateAndPrint(tr, L, d, &buffer, &buffer_len, c, fp_out);
    }

    // at the end, pad the file
    int num_pad = FilePrintPad(&buffer, &buffer_len, fp_out);
    FileRePrintFirstByte(num_pad, fp_out);


    // clean everything
    TreeDestroy(tr);
    ListDestroy(L);
    DictionaryDestroy(d, ASCII_SIZE);

    return;
}


void compression_status(char* name_in, char* name_out, FILE* fp_in, FILE* fp_out){
    assert(fp_in != NULL && fp_out != NULL);
    assert(name_in != NULL && name_out != NULL);

    fseek(fp_in, 0, SEEK_END);
    fseek(fp_out, 0, SEEK_END);

    long size_in = ftell(fp_in);
    long size_out = ftell(fp_out);

    printf("Input file: %s\nSize: %.3f KB\n", name_in, (float) size_in / 1024);
    printf("Output file: %s\nSize: %.3f KB\n", name_out, (float) size_out / 1024);
    printf("Space saving: %.2f%%\n", ((float) 1 - (float)size_out / (float) size_in) * 100);

    return;
}
