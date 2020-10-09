#include <stdio.h>
#include <stdlib.h>
#include "util.h"


void PrintByteInBits(int c, int len){
    // should only be 8 bits
    assert(len > 0 && len <= 8);
    assert(c >= 0 && c < power_of_2[len]);

    int mask, this_bit;

    for (int i = len; i >= 0; i--){
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