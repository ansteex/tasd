#include "list.h"

void initListStack(ListStack *s) {
    s->top = NULL;
    s->size = 0;
    s->freedCount = 0;
    for (int i = 0; i < MAX_STACK_SIZE; i++) {
        s->freedAddresses[i] = NULL;
    }
}

int pushListStack(ListStack *s, char *word) {
    ListNode *newNode = (ListNode*)malloc(sizeof(ListNode));
    if (!newNode) {
        printf("Ошибка выделения памяти\n");
        return 1;
    }
    strcpy(newNode->word, word);
    newNode->next = s->top;
    s->top = newNode;
    s->size++;
    return 0;
}

void popListStack(ListStack *s, char *word) {
    if (s->top == NULL) {
        printf("Стек пуст\n");
        return;
    }
    ListNode *temp = s->top;
    strcpy(word, temp->word);
    s->top = temp->next;
    
    if (s->freedCount < MAX_STACK_SIZE) {
        s->freedAddresses[s->freedCount++] = temp;
    }
    
    free(temp);
    s->size--;
}

void printListStack(ListStack *s) {
    if (s->top == NULL) {
        printf("Стек пуст.\n");
        return;
    }
    printf("Содержимое стека (список):\n");
    ListNode *current = s->top;
    while (current != NULL) {
        printf("[%p] %s\n", (void*)current, current->word);
        current = current->next;
    }
}

void printFreedAddresses(ListStack *s) {
    if(s->freedCount==0)
    {
        printf("Нет освобожденных адресов\n");
        return;
    }
    printf("Освобожденные адреса:\n");
    for (int i = 0; i < s->freedCount; i++) {
        printf("[%d] %p\n", i, (void*)s->freedAddresses[i]);
    }
}