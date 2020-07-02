#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


char* string_concat(const char* s1, const char* s2){
    // concat s1 followed by s2
    // return a new pointer, so the inputs are not changed
    char* result;

    if (s1 == NULL){
        // simply copy s2
        result = (char*)malloc((strlen(s2)+1) * sizeof(char));
        if (result == NULL){
            fprintf(stderr, "Memory error: string_concat\n");
            exit(EXIT_FAILURE);
        }

        strcpy(result, s2);
    }
    else{
        result = (char*)malloc((strlen(s1)+strlen(s2)+1) * sizeof(char));

        result[0] = '\0';

        strcpy(result, s1);
        strcat(result, s2);
    }

    return result;
}