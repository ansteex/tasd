#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "arr.h"
#include "list.h"
#include "ops.h"
int is_space(char c)
{
    return c == ' ' || c == '\t' || c == '\n';
}
int is_empty(const char *str)
{
    if (str == NULL)
        return 1;
    for (const char *p = str; *p; p++)
    {
        if (!is_space(*p))
            return 0;
    }
    return 1;
}
int process_name(char *name)
{
    size_t len = strlen(name);
    if (len > 0 && name[len - 1] == '\n')
        name[len - 1] = '\0';
    return is_empty(name);
}
void printMenu() {
    printf("Меню\n");
    printf("1 - Инициализация массива\n");
    printf("2 - Инициализация списка\n");
    printf("3 - Добавление элемента в стек\n");
    printf("4 - Удаление элемента из стека\n");
    printf("5 - Просмотр текущего состояния стека\n");
    printf("6 - Печать слов в обратном порядке, каждое слово в перевернутом виде.\n");
    printf("7 - Просмотр освобожденных адресов (только для списка)\n");
    printf("8 - Анализ эффективности\n");
    printf("9 - Демонстрация фрагментации памяти\n");
    printf("0 - Завершение работы\n");
    printf("Выберите действие: ");
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int getIntegerInput(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &value) == 1) {
                if (value >= min && value <= max) {
                    return value;
                }
            }
        }
        printf("Ошибка: введите целое число от %d до %d\n", min, max);
    }
}

void addElementToStack(ArrayStack *arrStack, ListStack *listStack, int useList) {
    char line[MAX_WORD_LENGTH];
    printf("Введите слова для добавления: ");
    if(fgets(line, sizeof(line), stdin) == NULL || is_empty(line))
    {
        printf("Вы ввели пустую строку или строку состоящую только из пробельных символов\n");
        return;
    }
    char *token = strtok(line, " \t\n");
    while(token != NULL) {
        if(strlen(token) >= MAX_WORD_LENGTH) {
            printf("Слово '%s' слишком длинное и будет обрезано\n", token);
            token[MAX_WORD_LENGTH - 1] = '\0';
        }
        
        if(useList) {
            if(pushListStack(listStack, token)) {
                printf("Не удалось добавить слово '%s' (переполнение)\n", token);
                break;
            } else {
                printf("Слово '%s' добавлено в список\n", token);
            }
        } else {
            if(pushArrayStack(arrStack, token)) {
                printf("Не удалось добавить слово '%s' (переполнение)\n", token);
                break;
            } else {
                printf("Слово '%s' добавлено в массив\n", token);
            }
        }
        token = strtok(NULL, " \t\n");
    }
}

void removeElementFromStack(ArrayStack *arrStack, ListStack *listStack, int useList) {
    char word[MAX_WORD_LENGTH];
    
    if (useList) {
        if (listStack->top == NULL) {
            printf("Ошибка: стек пуст\n");
            return;
        }
        popListStack(listStack, word);
        printf("Извлечено из стека: %s\n", word);
    } else {
        if (isArrayStackEmpty(arrStack)) {
            printf("Ошибка: стек пуст\n");
            return;
        }
        popArrayStack(arrStack, word);
        printf("Извлечено из стека: %s\n", word);
    }
}

void viewStack(ArrayStack *arrStack, ListStack *listStack, int useList) {
    if (useList) {
        printListStack(listStack);
    } else {
        printArrayStack(arrStack);
    }
}

void processStackMenu(ArrayStack *arrStack, ListStack *listStack, int useList) {
    char word[MAX_WORD_LENGTH];
    
    if (useList) {
        if (listStack->top == NULL) {
            printf("Стек пуст\n");
            return;
        }
        
        while (listStack->top != NULL) {
            popListStack(listStack, word);
            reverseWord(word);
        }
    } else {
        if (isArrayStackEmpty(arrStack)) {
            printf("Стек пуст\n");
            return;
        }
        
        while (!isArrayStackEmpty(arrStack)) {
            popArrayStack(arrStack, word);
            reverseWord(word);
        }
    }
    printf("\n");
}
/*
void performanceTest() {
    printf("\nАнализ эффективности\n");
    
    const int base_repetitions = 100000;
    char testWord[]="word";
    clock_t start, end;
    int testLevels[] = {10, 50, 100,1000}; 
    int numLevels = sizeof(testLevels) / sizeof(testLevels[0]);
    //printf("Элементов | Время массива (нс/оп) | Время списка (нс/оп)  | Массив быстрее на\n");
    printf("Элементов | Время массива         | Время списка          | Массив быстрее на\n");
    printf("----------|-----------------------|-----------------------|--------------------\n");
    
    for (int level = 0; level < numLevels; level++) {
        int testSize = testLevels[level];
        int repetitions = base_repetitions;
        if (testSize > 50) repetitions = base_repetitions / 10;
        if (testSize > 80) repetitions = base_repetitions / 100;
        
        ArrayStack arrStack;
        ListStack listStack;
        initArrayStack(&arrStack);
        start = clock();
        for (int r = 0; r < repetitions; r++) {
            for (int i = 0; i < testSize; i++) {
                pushArrayStack(&arrStack, testWord);
            }  
            for (int i = 0; i < testSize; i++) {
                popArrayStack(&arrStack, testWord);
            }
        }
        end = clock();
        double arrayTotalTime = (double)(end - start) / CLOCKS_PER_SEC;
        double arrayTimePerOp = (arrayTotalTime * 1e9) / (2*repetitions) ;
        initListStack(&listStack);
        start = clock();
        for (int r = 0; r < repetitions; r++) {
            for (int i = 0; i < testSize; i++) {
                pushListStack(&listStack, testWord);
            }
            for (int i = 0; i < testSize; i++) {
                popListStack(&listStack, testWord);
            }
        }
        end = clock();
        double listTotalTime = (double)(end - start) / CLOCKS_PER_SEC;
        double listTimePerOp = (listTotalTime * 1e9) / (2*repetitions) ;
        double timePenalty = ((listTimePerOp  - arrayTimePerOp) / listTimePerOp) * 100;
        printf("%9d | %21.2f | %21.2f | %17.1f%%\n", 
               testSize, arrayTimePerOp, listTimePerOp, timePenalty);
    }
    printf("Элементов | Массив (байт) | Список (байт) | Память массива больше на\n");
    printf("----------|---------------|---------------|------------------------\n");
    size_t arrayStructSize = sizeof(ArrayStack); 
    size_t listStructSize = sizeof(ListStack);   
    size_t listElementSize = sizeof(ListNode);    
    
    for (int level = 0; level < numLevels; level++) {
        int testSize = testLevels[level];
        size_t arrayMemory = arrayStructSize;
        size_t listMemory = listStructSize + (listElementSize * testSize);
        double memoryPenalty = ((double)arrayMemory -(double)listMemory ) / arrayMemory * 100;
        
        printf("%9d | %13lu | %13lu | %25.1f%%\n", 
               testSize, arrayMemory, listMemory, memoryPenalty);
    }
}
*/
void performanceTest() {
    printf("\nАнализ эффективности\n");
    
    const int base_repetitions = 100000;
    char testWord[]="word";
    clock_t start, end;
    int testLevels[] = {10, 50, 100,1000}; 
    int numLevels = sizeof(testLevels) / sizeof(testLevels[0]);
    printf("Элементов | Время массива (нс/оп) | Время списка (нс/оп)  | Массив быстрее на\n");
    printf("----------|-----------------------|-----------------------|--------------------\n");
    
    for (int level = 0; level < numLevels; level++) {
        int testSize = testLevels[level];
        int repetitions = base_repetitions;
        if (testSize > 50) repetitions = base_repetitions / 10;
        if (testSize > 80) repetitions = base_repetitions / 100;
        
        ArrayStack arrStack;
        ListStack listStack;
        initArrayStack(&arrStack);
        start = clock();
        for (int r = 0; r < repetitions; r++) {
            for (int i = 0; i < testSize; i++) {
                pushArrayStack(&arrStack, testWord);
            }  
            for (int i = 0; i < testSize; i++) {
                popArrayStack(&arrStack, testWord);
            }
        }
        end = clock();
        double arrayTotalTime = (double)(end - start) / CLOCKS_PER_SEC;
        double arrayTimePerOp = (arrayTotalTime * 1e9) / (repetitions * testSize * 2);
        initListStack(&listStack);
        start = clock();
        for (int r = 0; r < repetitions; r++) {
            for (int i = 0; i < testSize; i++) {
                pushListStack(&listStack, testWord);
            }
            for (int i = 0; i < testSize; i++) {
                popListStack(&listStack, testWord);
            }
        }
        end = clock();
        double listTotalTime = (double)(end - start) / CLOCKS_PER_SEC;
        double listTimePerOp = (listTotalTime * 1e9) / (repetitions * testSize * 2);
        double timePenalty = ((arrayTimePerOp -listTimePerOp ) / arrayTimePerOp) * 100;
        
        printf("%9d | %21.2f | %21.2f | %17.1f%%\n", 
               testSize, arrayTimePerOp, listTimePerOp, timePenalty);
    }
    printf("Элементов | Массив (байт) | Список (байт) | Память массива больше на\n");
    printf("----------|---------------|---------------|------------------------\n");
    size_t arrayStructSize = sizeof(ArrayStack); 
    size_t listStructSize = sizeof(ListStack);   
    size_t listElementSize = sizeof(ListNode);    
    
    for (int level = 0; level < numLevels; level++) {
        int testSize = testLevels[level];
        size_t arrayMemory = arrayStructSize;
        size_t listMemory = listStructSize + (listElementSize * testSize);
        double memoryPenalty = ((double)arrayMemory -(double)listMemory ) / arrayMemory * 100;
        
        printf("%9d | %13lu | %13lu | %25.1f%%\n", 
               testSize, arrayMemory, listMemory, memoryPenalty);
    }
}
void demonstrateFragmentation() {
    printf("Демонстрация фрагментации памяти\n");
    
    ListStack stack;
    initListStack(&stack);
    
    printf("Добавляем 4 элемента:\n");
    for (int i = 0; i < 4; i++) {
        char word[20];
        sprintf(word, "word%d", i);
        pushListStack(&stack, word);
        printf("Добавлен: %s по адресу %p\n", word, (void*)stack.top);
    }
    
    printf("Удаляем 2-й и 3-й элементы:\n");
    char temp[50];
    popListStack(&stack, temp);
    printf("Удален: %s\n", temp);
    popListStack(&stack, temp);
    printf("Удален: %s\n", temp);
    popListStack(&stack, temp);
    printf("Удален: %s\n", temp);

    printf("Освобожденные адреса:\n");
    printFreedAddresses(&stack);
    
    printf("Добавляем 2 новых элемента:\n");
    pushListStack(&stack, "new1");
    printf("Добавлен: new1 по адресу %p\n", (void*)stack.top);
    pushListStack(&stack, "new2"); 
    printf("Добавлен: new2 по адресу %p\n", (void*)stack.top);
    pushListStack(&stack, "new3"); 
    printf("Добавлен: new3 по адресу %p\n", (void*)stack.top);
    printf("Итоговые освобожденные адреса:\n");
    printFreedAddresses(&stack);
    
    printf("Текущий стек:\n");
    printListStack(&stack);
}
int main() {
    ArrayStack arrStack;
    ListStack listStack;
    initArrayStack(&arrStack);
    initListStack(&listStack);
    
    int choice;
    int currentMode = 1;
    do {
        printMenu();
        choice = getIntegerInput("", 0, 9);
        switch (choice) {
            case 1:
                currentMode = 1;
                printf("Введите слова (каждое слово с новой строки или несколько слов через пробел)\n");
                char line1[MAX_WORD_LENGTH];
                while(fgets(line1, sizeof(line1), stdin) != NULL && !is_empty(line1)) {
                    char *token = strtok(line1, " \t\n");
                    while(token != NULL) {
                        if(strlen(token) >= MAX_WORD_LENGTH) {
                            token[MAX_WORD_LENGTH - 1] = '\0';
                        }
                        if(pushArrayStack(&arrStack, token)) {
                            printf("Стек массива переполнен\n");
                            break;
                        }
                        token = strtok(NULL, " \t\n");
                    }
                }
                break;
                
            case 2:
                currentMode = 2;
                printf("Введите слова (каждое слово с новой строки или несколько слов через пробел)\n");
                char line2[MAX_WORD_LENGTH];
                while(fgets(line2, sizeof(line2), stdin) != NULL && !is_empty(line2)) {
                    char *token = strtok(line2, " \t\n");
                    while(token != NULL) {
                        if(strlen(token) >= MAX_WORD_LENGTH) {
                            token[MAX_WORD_LENGTH - 1] = '\0';
                        }
                        if(pushListStack(&listStack, token)) {
                            printf("Стек списка переполнен\n");
                            break;
                        }
                        token = strtok(NULL, " \t\n");
                    }
                }
                break;
                
            case 3:
                addElementToStack(&arrStack, &listStack, currentMode == 2);
                break;
                
            case 4:
                removeElementFromStack(&arrStack, &listStack, currentMode == 2);
                break;
                
            case 5:
                viewStack(&arrStack, &listStack, currentMode == 2);
                break;
                
            case 6:
                processStackMenu(&arrStack, &listStack, currentMode == 2);
                break;
                
            case 7:
                if (currentMode == 2) {
                    printFreedAddresses(&listStack);
                } else {
                    printf("Только для стека-списка\n");
                }
                break;
                
            case 8:
                performanceTest();
                break;
            case 9:
                demonstrateFragmentation();
                break;
                
            case 0:
                break;
                
            default:
                printf("Неверный выбор\n");
        }
        
        if (choice != 0) {
            printf("Нажмите Enter для продолжения");
            clearInputBuffer();
            getchar();
        }
        
    } while (choice != 0);
    
    return 0;
}