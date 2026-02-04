#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_LENGTH 20
#define WORD_COUNT_1 100
#define WORD_COUNT_2 500  
#define WORD_COUNT_3 1000

const char* syllables[] = {
    "ab", "ac", "ad", "af", "ag", "al", "am", "an", "ap", "ar",
    "ba", "be", "bi", "bo", "bu", "ca", "ce", "ci", "co", "cu",
    "da", "de", "di", "do", "du", "fa", "fe", "fi", "fo", "fu",
    "ga", "ge", "gi", "go", "gu", "ha", "he", "hi", "ho", "hu",
    "ja", "je", "ji", "jo", "ju", "ka", "ke", "ki", "ko", "ku",
    "la", "le", "li", "lo", "lu", "ma", "me", "mi", "mo", "mu",
    "na", "ne", "ni", "no", "nu", "pa", "pe", "pi", "po", "pu",
    "ra", "re", "ri", "ro", "ru", "sa", "se", "si", "so", "su",
    "ta", "te", "ti", "to", "tu", "va", "ve", "vi", "vo", "vu"
};
const int syllable_count = 80;

void generate_word(char* word, int max_length) {
    int length = 0;
    word[0] = '\0';
    
    // Добавляем слоги пока не достигнем максимальной длины
    while (length < max_length - 3) {
        const char* syllable = syllables[rand() % syllable_count];
        int syllable_len = strlen(syllable);
        
        if (length + syllable_len >= max_length - 1) break;
        
        strcat(word, syllable);
        length += syllable_len;
    }
}

int is_word_unique(char** words, int count, const char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 0;
        }
    }
    return 1;
}

void generate_file(const char* filename, int word_count) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Ошибка создания файла %s\n", filename);
        return;
    }
    
    // Выделяем память для хранения уже сгенерированных слов
    char** generated_words = malloc(word_count * sizeof(char*));
    for (int i = 0; i < word_count; i++) {
        generated_words[i] = malloc(MAX_WORD_LENGTH * sizeof(char));
    }
    
    int unique_count = 0;
    int attempts = 0;
    const int max_attempts = word_count * 10;
    
    while (unique_count < word_count && attempts < max_attempts) {
        char new_word[MAX_WORD_LENGTH];
        generate_word(new_word, MAX_WORD_LENGTH);
        
        if (is_word_unique(generated_words, unique_count, new_word)) {
            strcpy(generated_words[unique_count], new_word);
            fprintf(file, "%s\n", new_word);
            unique_count++;
        }
        attempts++;
    }
    
    printf("Сгенерирован файл %s с %d уникальными словами\n", filename, unique_count);
    
    // Освобождаем память
    for (int i = 0; i < word_count; i++) {
        free(generated_words[i]);
    }
    free(generated_words);
    
    fclose(file);
}

int main() {
    srand(time(NULL));
    
    printf("Генерация тестовых файлов...\n");
    
    generate_file("words100.txt", WORD_COUNT_1);
    generate_file("words500.txt", WORD_COUNT_2);
    generate_file("words1000.txt", WORD_COUNT_3);
    
    printf("Все файлы успешно созданы!\n");
    
    return 0;
}