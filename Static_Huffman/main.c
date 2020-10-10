#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "util.h"
#include "tree.h"
#include "file.h"
#include "priority_queue.h"
#include "codeword.h"
#include "frequency_table.h"
#include "compress.h"
#include "decompress.h"


void compress(char* filename);
void decompress(char* filename);


int main(int argc, char** argv){
    if (argc != 3){
        printf("Usage: %s <-c|-d> <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "-c") == 0){
        compress(argv[2]);
    }
    else if (strcmp(argv[1], "-d") == 0){
        decompress(argv[2]);
    }
    else{
        printf("Usage: %s <-c|-d> <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    return 0;
}


void compress(char* filename){
    assert(filename != NULL);

    char* filename_out = CreateCompressedFileName(filename);

    FILE* fp_in = OpenFileWithMode(filename, "rb");
    FILE* fp_out = OpenFileWithMode(filename_out, "wb");

    FreqTable fqtable = ReadFileCountFrequency(fp_in);
    // FreqTableShow(fqtable);
    
    PriorityQueue pq = UseFreqTableProducePriorityQueue(fqtable);
    // PriorityQueueShow(pq);
    
    Tree tr = UsePriorityQueueProduceTree(pq);
    // TreeShow(tr);

    CodeWord cw = UseTreeProduceCodeWord(tr);
    // CodeWordShow(cw);


    // the first byte records the number of 0 pad at the end
    PrintFirstByteEmpty(fp_out);
    PrintSecondByteCharCount(fp_out, fqtable);

    // // print the tree: this is the header
    PrintCompressionTree(fp_out, tr);
    
    // // body of compression
    int pad_num = ReadFilePrintCompression(fp_in, fp_out, cw);
    RePrintFirstByteWithPadNumber(fp_out, pad_num);

    compression_status(filename, filename_out, fp_in, fp_out);

    CloseFile(fp_in);
    CloseFile(fp_out);
    
    FreqTableDestroy(fqtable);
    TreeDestroy(tr);
    PriorityQueueDestroy(pq);
    CodeWordDestroy(cw);

    free(filename_out);
    filename_out = NULL;

    return;
}


void decompress(char* filename){
    assert(filename != NULL);

    char* filename_out = CreateDecompressedFileName(filename);

    FILE* fp_in = OpenFileWithMode(filename, "rb");
    FILE* fp_out = OpenFileWithMode(filename_out, "wb");

    int pad_num = ReadFileGetPadNumber(fp_in);
    int char_count = ReadFileGetCharCount(fp_in);

    Tree tr = ReadHeaderProduceTree(fp_in, char_count);
    // TreeShow(tr);

    ReadFilePrintDecompression(fp_in, fp_out, tr, pad_num);

    decompression_status(filename, filename_out, fp_in, fp_out);

    CloseFile(fp_in);
    CloseFile(fp_out);

    TreeDestroy(tr);

    free(filename_out);
    filename_out = NULL;

    return;
}





