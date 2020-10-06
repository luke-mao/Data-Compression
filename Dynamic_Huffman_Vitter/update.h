#ifndef _UPDATE_H_
#define _UPDATE_H_


#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "list.h"
#include "dictionary.h"


void TreeUpdateForFirstChar(Tree tr, int c);
void ListUpdateForFirstChar(List L, Tree tr);
void UpdateTreeAndList(Tree tr, List L, Dictionary d, ListNode LN_p, int c);
void SlideAndIncrement(List, ListNode*);
void SwapWithLeader(List, ListNode);


#endif 
