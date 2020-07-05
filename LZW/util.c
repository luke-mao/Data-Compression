#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


// concat prev + char(c)
// malloc and return a new pointer
char* string_concat(const char* prev, const int c){
    // concat s1 followed by s2
    // return a new pointer, so the inputs are not changed
    char* result;
    if (prev == NULL){
        result = (char*)malloc(2*sizeof(char));
        if (result == NULL){
            fprintf(stderr, "Memory error: string_concat\n");
            exit(EXIT_FAILURE);
        }

        result[0] = c;
        result[1] = '\0';
    }
    else{
        result = (char*)malloc((strlen(prev)+1+1)*sizeof(char));
        if (result == NULL){
            fprintf(stderr, "Memory error: string_concat\n");
            exit(EXIT_FAILURE);
        }

        strcpy(result, prev);
        result[strlen(prev)] = c;
        result[strlen(prev)+1] = '\0';
    }
    
    return result;
}