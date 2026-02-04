#include "arr.h"
void initArrayStack(ArrayStack *s) {
    s->top = -1;
}

int isArrayStackEmpty(ArrayStack *s) {
    return s->top == -1;
}

int isArrayStackFull(ArrayStack *s) {
    return s->top == MAX_STACK_SIZE - 1;
}
int pushArrayStack(ArrayStack *s, char *word) {
    if (isArrayStackFull(s)) {
        printf("Стек переполнен\n");
        return 1;
    }
    strcpy(s->data[++s->top], word);
    return 0;
}

void popArrayStack(ArrayStack *s, char *word) {
    if (isArrayStackEmpty(s)) {
        printf("Стек пуст\n");
        return;
    }
    strcpy(word, s->data[s->top--]);
}

void printArrayStack(ArrayStack *s) {
    if (isArrayStackEmpty(s)) {
        printf("Стек пуст.\n");
        return;
    }
    printf("Содержимое стека (массив):\n");
    for (int i = s->top; i >= 0; i--) {
        printf("[%d] %s\n", i, s->data[i]);
    }
}