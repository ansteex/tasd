#ifndef ARR_H
#define ARR_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "consts.h"

typedef struct {
    char data[MAX_STACK_SIZE][MAX_WORD_LENGTH];
    int top;
} ArrayStack;

void initArrayStack(ArrayStack *s);
int isArrayStackEmpty(ArrayStack *s);
int isArrayStackFull(ArrayStack *s);
int pushArrayStack(ArrayStack *s, char *word);
void popArrayStack(ArrayStack *s, char *word);
void printArrayStack(ArrayStack *s);

#endif