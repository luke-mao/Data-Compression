#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "util.h"
#include "codeword.h"
#include "file.h"



FILE* OpenFileWithMode(char* filename, char* mode){
    assert(filename != NULL && mode != NULL);

    FILE* fp = fopen(filename, mode);
    assert(fp != NULL);
    
    return fp;
}


FILE* CloseFile(FILE* fp){
    assert(fp != NULL);
    fclose(fp);
    fp = NULL;
    return fp;
}


int GetOneBit(FILE* fp, int* buffer_p, int* unread_num_p, int* buffer_next_p){
    assert(fp != NULL);
    assert(buffer_p != NULL && buffer_next_p != NULL);
    assert(unread_num_p != NULL && (*unread_num_p) >= 0 && (*unread_num_p) <= 8);

    if ((*unread_num_p) == 0){
        (*buffer_p) = (*buffer_next_p);
        (*buffer_next_p) = getc(fp);
        (*unread_num_p) = 8;
    }

    int mask = 1 << ((*unread_num_p) - 1);
    int this_bit = mask & (*buffer_p);
    this_bit >>= ((*unread_num_p) - 1);
    this_bit &= 1;

    (*unread_num_p) -= 1;

    return this_bit;
}


int GetOneByte(FILE* fp, int* buffer_p, int* unread_num_p, int* buffer_next_p){
    assert(fp != NULL);
    assert(buffer_p != NULL && buffer_next_p != NULL);
    assert(unread_num_p != NULL && (*unread_num_p) >= 0 && (*unread_num_p) <= 8);

    int result = 0;

    if ((*unread_num_p) == 0){
        result = (*buffer_next_p);
        (*buffer_p) = getc(fp);
        (*unread_num_p) = 8;
        (*buffer_next_p) = getc(fp);
    }
    else if ((*unread_num_p) == 8){
        result = (*buffer_p);
        (*buffer_p) = (*buffer_next_p);     // unread_num still be 8
        (*buffer_next_p) = getc(fp);
    }
    else{
        // extract one part from buffer_p, and another half from the next byte
        int mask = power_of_2[*unread_num_p] - 1;
        result = (*buffer_p) & mask;
        result <<= 8 - (*unread_num_p);

        mask = power_of_2[8 - (*unread_num_p)] - 1;        
        int other_part = (*buffer_next_p) & (mask <<= (*unread_num_p));
        other_part >>= (*unread_num_p);
        other_part &= mask;

        result |= other_part;

        (*buffer_p) = (*buffer_next_p);     // unread num is still the same
        (*buffer_next_p) = getc(fp);
    }

    return result;
}


int GetOneByteSimple(FILE* fp){
    assert(fp != NULL);
    return getc(fp);
}


void PrintOneBit(FILE* fp, int* buffer_p, int* buffer_len_p, int this_bit){
    assert(fp != NULL);
    assert(buffer_p != NULL);
    assert(buffer_len_p != NULL && (*buffer_len_p) >= 0 && (*buffer_len_p) <= 8);
    assert(this_bit == 0 || this_bit == 1);

    if ((*buffer_len_p) == 8){
        putc(*buffer_p, fp);
        (*buffer_p) = 0;
        (*buffer_len_p) = 0;
    }

    (*buffer_p) <<= 1;
    (*buffer_p) |= this_bit;
    (*buffer_len_p) += 1;

    return;
}


void PrintOneByte(FILE* fp, int* buffer_p, int* buffer_len_p, int this_byte){
    assert(fp != NULL);
    assert(buffer_p != NULL);
    assert(buffer_len_p != NULL && (*buffer_len_p) >= 0 && (*buffer_len_p) <= 8);
    assert(this_byte >= 0 && this_byte < 256);

    // append the whole byte to the buffer
    // then print
    (*buffer_p) <<= 8;
    (*buffer_p) |= this_byte;
    (*buffer_len_p) += 8;

    int mask;
    int c;

    while ((*buffer_len_p) >= 8){
        mask = power_of_2[8] - 1;
        c = (*buffer_p) & (mask << ((*buffer_len_p) - 8));
        c >>= (*buffer_len_p) - 8;
        c &= mask;
        putc(c, fp);

        (*buffer_len_p) -= 8;
    }

    return;
}


// pad the bit, and return the number of 0 padded
int PadByte(FILE* fp, int* buffer_p, int* buffer_len_p){
    assert(fp != NULL);
    assert(buffer_p != NULL);
    assert(buffer_len_p != NULL && (*buffer_len_p) >= 0 && (*buffer_len_p) <= 8);

    if ((*buffer_len_p) == 0){
        return 0;
    }
    else{
        int pad_num = 8 - (*buffer_len_p);
        (*buffer_p) <<= pad_num;
        putc(*buffer_p, fp);

        // after padding, reset the buffer and its length to 0
        (*buffer_p) = 0;
        (*buffer_len_p) = 0;

        return pad_num;
    }
}


void PrintFirstByteEmpty(FILE* fp){
    assert(fp != NULL);
    fseek(fp, 0, SEEK_SET);
    putc(0, fp);
    return;
}


void RePrintFirstByteWithPadNumber(FILE* fp, int pad_num){
    assert(fp != NULL);
    assert(pad_num >= 0 && pad_num <= 7);

    fseek(fp, 0, SEEK_SET);
    putc(pad_num, fp);
    return;
}


int ReadFirstByteGetPadNumber(FILE* fp){
    assert(fp != NULL);
    
    // the pad number is at the first byte
    fseek(fp, 0, SEEK_SET);
    return getc(fp);
}
