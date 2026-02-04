#include "read_from_file.h"

void clear_file_buffer(FILE *f) {
    int c;
    while ((c = fgetc(f)) != '\n' && c != EOF);
}

int input_spectacle_from_file(FILE *f, struct Spectacle *s) {
    if (fgets(s->theatre_name, sizeof(s->theatre_name), f) == NULL)
        return ERROR;
    s->theatre_name[strcspn(s->theatre_name, "\n")] = 0;
    
    if (fgets(s->spec_name, sizeof(s->spec_name), f) == NULL)
        return ERROR;
    s->spec_name[strcspn(s->spec_name, "\n")] = 0;
    if (fscanf(f, "%d", &s->min_price) != 1 || s->min_price <= 0)
        return ERROR;
        
    clear_file_buffer(f);
    if (fscanf(f, "%d", &s->max_price) != 1 || s->max_price <= 0)
        return ERROR;
    clear_file_buffer(f);
    if (fscanf(f, "%d", &s->type) != 1 || (s->type < 0 || s->type > 1))
        return ERROR;
    clear_file_buffer(f);
    if (s->type == 0) {
        if (fscanf(f, "%d", &s->play.play_type) != 1 || (s->play.play_type < 0 || s->play.play_type > 2))
            return ERROR;
        clear_file_buffer(f);
        if (s->play.play_type == 2) {
            char s_age[AGE];
            if (fgets(s_age, sizeof(s_age), f) == NULL)
                return ERROR;
            s_age[strcspn(s_age, "\n")] = 0;
            s->play.age = age_limit_from_string(s_age);
            if (s->play.age == AGE_ERROR)
                return ERROR;
        }
    } else {
        if (fgets(s->musical.composer, sizeof(s->musical.composer), f) == NULL)
            return ERROR;
        s->musical.composer[strcspn(s->musical.composer, "\n")] = 0;
        
        if (fgets(s->musical.country, sizeof(s->musical.country), f) == NULL)
            return ERROR;
        s->musical.country[strcspn(s->musical.country, "\n")] = 0;
        
        if (fscanf(f, "%d", &s->musical.musical_type) != 1)
            return ERROR;
        clear_file_buffer(f);
        
        char s_age[AGE];
        if (fgets(s_age, sizeof(s_age), f) == NULL)
            return ERROR;
        s_age[strcspn(s_age, "\n")] = 0;
        s->musical.age = age_limit_from_string(s_age);
        if (s->musical.age == AGE_ERROR)
            return ERROR;
        
        if (fscanf(f, "%d", &s->musical.duration) != 1)
            return ERROR;
        clear_file_buffer(f);
    }
    return 0;
}