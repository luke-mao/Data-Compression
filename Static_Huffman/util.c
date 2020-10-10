#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "util.h"

const int power_of_2[9] = {1, 2, 4, 8, 16, 32, 64, 128, 256};


void PrintByteInBits(int c, int len){
    // max 8 bits, max value < 256
    assert(len > 0 && len <= 8);
    assert(c >= 0 && c < power_of_2[len]);

    int mask, this_bit;

    for (int i = len-1; i >= 0; i--){
        mask = 1 << i;
        this_bit = mask & c;
        this_bit >>= i;
        this_bit &= 1;

        if (this_bit == 0){
            printf("0");
        }
        else{
            printf("1");
        }
    }

    return;
}