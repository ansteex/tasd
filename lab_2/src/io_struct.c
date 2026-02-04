#include "io_struct.h"
const char* get_spectacle_type_name(int type) {
    switch(type) {
        case 0: return "Пьеса";
        case 1: return "Музыкальный";
        default: return "Неизвестно";
    }
}

const char* get_play_type_name(int play_type) {
    switch(play_type) {
        case 0: return "Драма";
        case 1: return "Комедия";
        case 2: return "Сказка";
        default: return "Неизвестно";
    }
}

const char* get_musical_type_name(int musical_type) {
    switch(musical_type) {
        case 0: return "Балет";
        case 1: return "Опера";
        case 2: return "Мюзикл";
        default: return "Неизвестно";
    }
}
enum AGE_LIMIT age_limit_from_string(char* s) {
    s[strcspn(s, "\n")] = '\0';
    if (strcmp(s, "16+") == 0)
        return AGE_16;
    else if (strcmp(s, "10+") == 0)
        return AGE_10;
    else if (strcmp(s, "3+") == 0)
        return AGE_3;
    return AGE_ERROR;

}
const char* get_age(enum AGE_LIMIT age) {
    switch(age) {
        case 0: return "3+";
        case 1: return "10+";
        case 2: return "16+";
        default: return "Неизвестно";
    }
}
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
/*
int contains_digits(const char *str) {
        for (int i = 0; str[i] != '\0'; i++) {
            if (isdigit((unsigned char)str[i])) {
                return 1;
            }
        }
        return 0;
    }*/
void input_spectacle(struct Spectacle *s) {
    printf("\nВвод данных о спектакле\n");
    
    int valid_input;
    valid_input = 0;
    while (!valid_input) {
        printf("Название театра: ");
        if (fgets(s->theatre_name, sizeof(s->theatre_name), stdin) != NULL) {
            s->theatre_name[strcspn(s->theatre_name, "\n")] = 0;
            if (strlen(s->theatre_name) > 0) valid_input = 1;
        } else {
            clear_input_buffer();
        }
    }
    valid_input = 0;
    while (!valid_input) {
        printf("Название спектакля: ");
        if (fgets(s->spec_name, sizeof(s->spec_name), stdin) != NULL) {
            s->spec_name[strcspn(s->spec_name, "\n")] = 0;
            if (strlen(s->spec_name) > 0) valid_input = 1;
        } else {
            clear_input_buffer();
        }
    }
    valid_input = 0;
    while (!valid_input) {
        printf("Минимальная цена билета: ");
        if (scanf("%d", &s->min_price) == 1 && s->min_price > 0) {
            valid_input = 1;
        } else {
            clear_input_buffer();
        }
    }
    valid_input = 0;
    while (!valid_input) {
        printf("Максимальная цена билета: ");
        if (scanf("%d", &s->max_price) == 1 && s->max_price > 0) {
            valid_input = 1;
        } else {
            clear_input_buffer();
        }
    }
    valid_input = 0;
    while (!valid_input) {
        printf("Тип спектакля (0-Пьеса, 1-Музыкальный): ");
        if (scanf("%d", &s->type) == 1 && (s->type == 0 || s->type == 1)) {
            valid_input = 1;
        } else {
            clear_input_buffer();
        }
    }
    clear_input_buffer();
    if (s->type == 0) {
        valid_input = 0;
        while (!valid_input) {
            printf("Тип пьесы (0-Драма, 1-Комедия, 2-Сказка): ");
            if (scanf("%d", &s->play.play_type) == 1 && s->play.play_type >= 0 && s->play.play_type <= 2) {
                valid_input = 1;
            } else {
                clear_input_buffer();
            }
        }
        
        clear_input_buffer();
        
        if (s->play.play_type == 2) {
            valid_input = 0;
            while (!valid_input) {
                printf("Возрастная категория (3+, 10+, 16+): ");
                char s_age[AGE];
                if (fgets(s_age, sizeof(s_age), stdin)) {
                    s->play.age = age_limit_from_string(s_age);
                    if (s->play.age != AGE_ERROR) {
                        valid_input = 1;
                    }
                }
            }
        }
    } else {
        valid_input = 0;
        while (!valid_input) {
            printf("Композитор: ");
            if (fgets(s->musical.composer, sizeof(s->musical.composer), stdin) != NULL) {
                s->musical.composer[strcspn(s->musical.composer, "\n")] = 0;
                if (strlen(s->musical.composer) > 0) valid_input = 1;
            }
        }
        valid_input = 0;
        while (!valid_input) {
            printf("Страна: ");
            if (fgets(s->musical.country, sizeof(s->musical.country), stdin) != NULL) {
                s->musical.country[strcspn(s->musical.country, "\n")] = 0;
                if (strlen(s->musical.country) > 0) valid_input = 1;
            }
        }
        
        valid_input = 0;
        while (!valid_input) {
            printf("Тип музыкального спектакля (0-Балет, 1-Опера, 2-Мюзикл): ");
            if (scanf("%d", &s->musical.musical_type) == 1 && s->musical.musical_type >= 0 && s->musical.musical_type <= 2) {
                valid_input = 1;
            } else {
                clear_input_buffer();
            }
        }
        
        clear_input_buffer();
        valid_input = 0;
        while (!valid_input) {
            printf("Возрастная категория (3+, 10+, 16+): ");
            char s_age[AGE];
            if (fgets(s_age, sizeof(s_age), stdin)) {
                s->musical.age = age_limit_from_string(s_age);
                if (s->musical.age != AGE_ERROR) {
                    valid_input = 1;
                }
            }
        }
        valid_input = 0;
        while (!valid_input) {
            printf("Продолжительность (в минутах): ");
            if (scanf("%d", &s->musical.duration) == 1 && s->musical.duration > 0) {
                valid_input = 1;
            } else {
                clear_input_buffer();
            }
        }
        
        clear_input_buffer();
    }
}
void print_spectacle(struct Spectacle *s, int index) {
    printf("%d. %s ", index + 1, s->theatre_name);
    printf("%s ", s->spec_name);
    printf("%d-%d ", s->min_price, s->max_price);
    printf("%s ", get_spectacle_type_name(s->type));
    
    if (s->type == 0) {
        printf("%s ", get_play_type_name(s->play.play_type));
        if (s->play.play_type == 2) {
            printf("%s ", get_age(s->play.age));
        }
    } else {
        printf("%s ", s->musical.composer);
        printf("%s ", s->musical.country);
        printf("%s ", get_musical_type_name(s->musical.musical_type));
        printf("%s ", get_age(s->musical.age));
        printf("%d мин.", s->musical.duration);
    }
    printf("\n");
}