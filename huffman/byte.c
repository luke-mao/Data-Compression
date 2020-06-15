#include <stdio.h>
#include <stdlib.h>
#include "byte.h"

void print_byte(const Byte b, const int count){
    // print the byte, count = 8 if you want to print the whole bytes out
    for (int i = count-1; i >= 0; i--){
        Byte mask = 1 << i;
        Byte result = (b & mask) >> i;

        if (result == 1){
            fprintf(stdout, "1");
        }
        else{
            fprintf(stdout, "0");
        }
    } 
    return;
}