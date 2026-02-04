#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "consts.h"
typedef struct ListNode ListNode;
typedef struct ListStack ListStack;
struct ListNode {
    char word[MAX_WORD_LENGTH];
    ListNode *next;
};

struct ListStack {
    ListNode *top;
    int size;
    ListNode *freedAddresses[MAX_STACK_SIZE];  
    int freedCount;                           
};
void initListStack(ListStack *s);
int pushListStack(ListStack *s, char *word);
void popListStack(ListStack *s, char *word);
void printListStack(ListStack *s);
void printFreedAddresses(ListStack *s);

#endif