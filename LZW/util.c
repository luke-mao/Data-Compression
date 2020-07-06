#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


// concat prev + char(c)
// malloc and return a new pointer
char* string_concat(const char* prev, const int c){
    // concat prev followed by the new char
    // return a new pointer, so the inputs are not changed
    char* result;
    if (prev == NULL){
        // create space for the single char
        result = (char*)malloc(2*sizeof(char));
        assert(result != NULL);

        // create the string
        result[0] = c;
        result[1] = '\0';
    }
    else{
        // concat the two things
        result = (char*)malloc((strlen(prev)+1+1)*sizeof(char));
        assert(result != NULL);

        // copy the first part, and then the char
        strcpy(result, prev);
        result[strlen(prev)] = c;
        result[strlen(prev)+1] = '\0';
    }
    
    return result;
}