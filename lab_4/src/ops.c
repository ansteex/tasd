#include "ops.h"
void reverseWord(char *word)
{
    int len = strlen(word);
    for (int i = len - 1; i >= 0; i--) {
        printf("%c", word[i]);
    }
    printf(" ");
}
