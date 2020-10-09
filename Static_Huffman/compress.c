#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "tree.h"
#include "priority_queue.h"
#include "frequency_table.h"
#include "file.h"
#include "util.h"
#include "codeword.h"


void UseTreeProduceCodeWordFunction(CodeWord cw, TreeNode trn);

void PrintCompressionTreeFunction(FILE* fp, int* buffer_p, int* buffer_len_p, TreeNode trn);


// add .huff suffix 
char* CreateCompressedFileName(char* filename){
    assert(filename != NULL);

    char* filename_out = (char*) malloc((strlen(filename)+5+1) * sizeof(char));
    assert(filename_out != NULL);

    strcpy(filename_out, filename);
    strcat(filename_out, ".huff");

    return filename_out;
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


FreqTable ReadFileCountFrequency(FILE* fp){
    assert(fp != NULL);
    fseek(fp, 0, SEEK_SET);

    FreqTable fqtable = FreqTableCreate(ASCII_SIZE);

    // read file
    int c;

    while ((c = getc(fp)) != EOF){
        FreqTableInsert(fqtable, c);
    }

    return fqtable;
}


PriorityQueue UseFreqTableProducePriorityQueue(FreqTable fqtable){
    assert(fqtable != NULL && fqtable->table != NULL);
    assert(fqtable->size > 0);

    PriorityQueue pq = PriorityQueueCreate();

    for (int i = 0; i < fqtable->size; i++){
        if (fqtable->table[i] > 0){
            // create a tree node and insert into the table
            TreeNode trn = TreeNodeCreate(i, fqtable->table[i]);
            PriorityQueueInsertTreeNode(pq, trn);
        }
    }

    return pq;
}


Tree UsePriorityQueueProduceTree(PriorityQueue pq){
    assert(IsPriorityQueueValid(pq));

    // pick two trn nodes from the queue
    // combine them, smaller occ be the right child
    // and insert back to the queue
    // until only one trn left
    TreeNode trn1, trn2, trn_parent;    
    int trn_parent_occ;
    
    while (GetCount(pq) != 1){
        trn1 = PriorityQueueGetTreeNode(pq);
        trn2 = PriorityQueueGetTreeNode(pq);

        trn_parent_occ = SumTwoOcc(trn1, trn2);
        trn_parent = TreeNodeCreate(INTERNAL_NODE_C, trn_parent_occ);

        ConnectAsLeftChild(trn2, trn_parent);
        ConnectAsRightChild(trn1, trn_parent);

        PriorityQueueInsertTreeNode(pq, trn_parent);
    }

    TreeNode root = PriorityQueueGetTreeNode(pq);
    ResetInternalNodeToRootNode(root);

    Tree tr = TreeCreate(root);
    return tr;
}


CodeWord UseTreeProduceCodeWord(Tree tr){
    assert(tr != NULL);

    CodeWord cw = CodeWordCreate(ASCII_SIZE);

    // traversal the tree
    // insert each leaf into the codeword
    UseTreeProduceCodeWordFunction(cw, tr->root);

    return cw;
}


void UseTreeProduceCodeWordFunction(CodeWord cw, TreeNode trn){
    if (trn != NULL){
        if (IsLeafNode(trn)){
            CodeWordNode cwn = CodeWordNodeCreate(trn);
            CodeWordInsertNode(cw, cwn);
        }
        else{
            UseTreeProduceCodeWordFunction(cw, trn->left);
            UseTreeProduceCodeWordFunction(cw, trn->right);
        }
    }

    return;
}


int ReadFilePrintCompression(FILE* fp_in, FILE* fp_out, CodeWord cw){
    assert(fp_in != NULL && fp_out != NULL);
    assert(cw != NULL);
    assert(cw->size > 0 && cw->list != NULL);

    // move the file descriptor to the start of the file
    // read again
    fseek(fp_in, 0, SEEK_SET);

    int c;
    int buffer = 0;
    int buffer_len = 0;

    CodeWordNode cwn;

    while ((c = getc(fp_in)) != EOF){
        cwn = CodeWordGetNode(cw, c);
        PrintCodeWord(fp_out, &buffer, &buffer_len, cwn);
    }

    // after finish, pad the last byte if necessary
    int pad_num = PadByte(fp_out, &buffer, &buffer_len);
    return pad_num;
}


void PrintCompressionTree(FILE* fp, Tree tr){
    assert(fp != NULL);
    assert(tr != NULL && tr->root != NULL);

    // output post order traversal of the tree
    // during decompression, use stack to rebuild the tree
    int buffer = 0;
    int buffer_len = 0;
    OutputCompressionTreeFunction(fp, &buffer, &buffer_len, tr->root);
    PadByte(fp, &buffer, &buffer_len);

    return;
}


void PrintCompressionTreeFunction(FILE* fp, int* buffer_p, int* buffer_len_p, TreeNode trn){
    assert(fp != NULL);
    assert(buffer_p != NULL && buffer_len_p != NULL);

    if (trn != NULL){
        // post order traversal: left, right, middle
        // check this node first
        if (IsLeafNode(trn)){
            // for leaf node: print 1 + byte
            PrintOneBit(fp, buffer_p, buffer_len_p, 1);
            PrintOneByte(fp, buffer_p, buffer_len_p, GetC(trn));
        }
        else{
            // internal node
            // go down left and right first
            OutputCompressionTreeFunction(fp, buffer_p, buffer_len_p, trn->left);
            OutputCompressionTreeFunction(fp, buffer_p, buffer_len_p, trn->right);

            // then print the bit 0
            PrintOneBit(fp, buffer_p, buffer_len_p, 0);
        }
    }

    return;
}
