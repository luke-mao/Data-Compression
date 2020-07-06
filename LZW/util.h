#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Append the char to the string. 
This function malloc and then strcpy the original string, 
and concat the char to the end. 
Return a new pointer of chars. So safer to use. 

It is also used to create a string from the single char, 
where char* prev = NULL */
char* string_concat(const char* prev, const int c);

#endif