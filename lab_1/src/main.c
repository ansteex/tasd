#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MULT_ERROR 1
#define MAX_LEN_MANT 30
#define MAX_EXPONENT 5
#define MAX_RESULT_MANT 40
#define MAX_INT_MANT 40
#define MAX_EXSP 99999
#define MAX_LEN 50
#define INP_ERROR 2
#define PRINT_ERROR 3
#define ERROR 1

struct short_num {
    char sign;
    char mantissa[MAX_LEN_MANT + 1];
    int exponent;
    int len_m;
};

struct long_num {
    char sign;
    char mantissa[MAX_RESULT_MANT + 1];
    int exponent;
    int len_m;
};

int parse_integer(const char* str, struct long_num* result) {
    result->sign = '+';
    result->mantissa[0] = '\0';
    result->exponent = 0;
    result->len_m = 0;
    if (str == NULL || str[0] == '\0')
        return ERROR;
    const char *p = str;
    if (*p == '-'){
        result->sign = '-';
        p++;
    } else if (*p == '+')
        p++;
    while (*p == '0' && *p != '\0') {
        p++;
    }
    if (*p == '\0') {
        result->mantissa[0] = '0';
        result->mantissa[1] = '\0';
        result->len_m = 1;
        return 0;
    }
    int i = 0;
    while (*p != '\0' && i < MAX_INT_MANT) {
        if (isdigit(*p)) {
            result->mantissa[i++] = *p;
        } else {
            return ERROR;
        }
        p++;
    }
    if (*p != '\0')
        return ERROR;
    result->mantissa[i] = '\0';
    result->len_m = i;
    return 0;
}

int parse_float(const char* str, struct short_num* result) {
    result->sign = '+';
    result->mantissa[0] = '\0';
    result->exponent = 0;
    result->len_m = 0;
    if (str == NULL || str[0] == '\0')
        return ERROR;
    const char *p = str;
    int has_point = 0;
    int decimal_digits = 0;
    if (*p == '-') {
        result->sign = '-';
        p++;
    } else if (*p == '+')
        p++;
    
    while (*p == '0' && *p != '\0' && *p != '.') {
        p++;
    }
    if (*p == '\0' || (*p == '.' && *(p+1) == '\0')) {
        result->mantissa[0] = '0';
        result->mantissa[1] = '\0';
        result->len_m = 1;
        result->exponent = 0;
        return 0;
    }
    int i = 0, total_digits = 0;
    while (*p != '\0' && *p != 'E' && *p != ' ') {
        if (*p == '.') {
            if (has_point) 
                return ERROR;
            has_point = 1;
            p++;
            continue;
        }
        
        if (isdigit(*p)) {
            total_digits++;
            if (total_digits > MAX_LEN_MANT)
                return ERROR;
            if (i < MAX_LEN_MANT) {
                result->mantissa[i++] = *p;
                if (has_point) {
                    decimal_digits++;
                }
            }
        } else {
            return ERROR;
        }
        p++;
    }
    if (total_digits == 0)
        return ERROR;
    result->mantissa[i] = '\0';
    result->len_m = i;
    if (has_point) {
        result->exponent = -decimal_digits;
    }
    int trailing_zeros = 0;
    while (result->len_m > 1 && result->mantissa[result->len_m - 1] == '0') {
        trailing_zeros++;
        result->len_m--;
    }
    result->mantissa[result->len_m] = '\0';
    if (has_point) {
        result->exponent += trailing_zeros;
    }
    while (*p == ' ') {
            p++;
        }
    if (*p == 'E') {
        p++;
        while (*p == ' ') {
            p++;
        }
        if (*p == '\0') 
            return ERROR;
        int exp_sign = 1;
        if (*p == '+') {
            p++;
        } else if (*p == '-') {
            exp_sign = -1;
            p++;
        }
        if (!isdigit(*p))
            return ERROR;
        while (*p == '0' && *p != '\0') {
            p++;
        }
        int exp_value = 0;
        int exp_digits = 0;
        
        while (*p != '\0' && isdigit(*p)) {
            exp_value = exp_value * 10 + (*p - '0');
            exp_digits++;
            p++;
        }
        while (*p == ' ') 
            p++;
        if (*p != '\0') 
            return ERROR;
        if (exp_digits > MAX_EXPONENT) {
            return ERROR;
        }
        
        if (exp_digits > 0) {
            result->exponent += exp_value * exp_sign;
        }
    }
    result->len_m = strlen(result->mantissa);
    
    return 0;
}

void round_result(struct long_num* num) {
    if (num->len_m <= MAX_RESULT_MANT) return;
    int round_pos = MAX_RESULT_MANT;
    int carry = (num->mantissa[round_pos] >= '5') ? 1 : 0;
    num->len_m = MAX_RESULT_MANT;
    num->mantissa[num->len_m] = '\0';
    for (int i = num->len_m - 1; i >= 0 && carry; i--) {
        int digit = (num->mantissa[i] - '0') + carry;
        num->mantissa[i] = (digit % 10) + '0';
        carry = digit / 10;
    }
    if (carry) {
        memmove(num->mantissa + 1, num->mantissa, num->len_m);
        num->mantissa[0] = '1';
        num->len_m++;
        num->exponent++;
    }
}

int multiply_numbers(const struct long_num* a, const struct short_num* b, struct long_num* result) {
    if (a->sign == b->sign)
        result->sign = '+';
    else 
        result->sign = '-';
    result->exponent = a->exponent + b->exponent;
    int digits_a[MAX_INT_MANT] = {0};
    int digits_b[MAX_LEN_MANT] = {0};
    int len_a = a->len_m;
    int len_b = b->len_m;
    for (int i = 0; i < len_a; i++) {
        digits_a[i] = a->mantissa[i] - '0';
    }
    for (int i = 0; i < len_b; i++) {
        digits_b[i] = b->mantissa[i] - '0';
    }
    int total_len = len_a + len_b;
    int product[MAX_INT_MANT + MAX_LEN_MANT] = {0};
    for (int i = 0; i < total_len; i++) {
        product[i] = 0;
    }
    for (int i = len_a - 1; i >= 0; i--) {
        for (int j = len_b - 1; j >= 0; j--) {
            int mul = digits_a[i] * digits_b[j];
            int sum = product[i + j + 1] + mul;
            product[i + j + 1] = sum % 10;
            product[i + j] += sum / 10;
        }
    }
    for (int i = total_len - 1; i > 0; i--) {
        if (product[i] >= 10) {
            product[i - 1] += product[i] / 10;
            product[i] %= 10;
        }
    }
    int start_index = 0;
    while (start_index < total_len && product[start_index] == 0) {
        start_index++;
    }
    if (start_index == total_len) {
        result->mantissa[0] = '0';
        result->mantissa[1] = '\0';
        result->len_m = 1;
        result->exponent = 0;
        result->sign = '+';
        return 0;
    }
    int result_index = 0;
    for (int i = start_index; i < total_len && result_index < MAX_RESULT_MANT + 1; i++) {
        result->mantissa[result_index++] = product[i] + '0';
    }
    result->mantissa[result_index] = '\0';
    result->len_m = result_index;
    result->exponent += (total_len-start_index);
    round_result(result);
    return 0;
}

int print_normalized(const struct long_num* num) {
    if (num->len_m == 0 || (num->len_m == 1 && num->mantissa[0] == '0')) {
        printf("+0.0 E +0\n");
        return 0;
    }
    if (num->exponent > MAX_EXSP) {
        return ERROR;
    } else if (num->exponent < -MAX_EXSP) {
        return ERROR;
    }
    
    printf("%c0.", num->sign);
    
    for (int i = 0; i < num->len_m && i < MAX_RESULT_MANT; i++) {
        printf("%c", num->mantissa[i]);
    }
    
    printf(" E %+d\n", num->exponent);
    return 0;
}

int main()
{
    char integer_str[MAX_LEN];
    char float_str[MAX_LEN];
    struct short_num float_num;
    struct long_num integer_num, result;
    printf("----------------------------------------\n");
    if (fgets(integer_str, sizeof(integer_str), stdin) == NULL) {
        printf("Ошибка ввода целого числа\n");
        return INP_ERROR;
    }
    integer_str[strcspn(integer_str, "\n")] = '\0';
    if (parse_integer(integer_str,&integer_num)) {
        printf("Ошибка ввода целого числа\n");
        return INP_ERROR;
    }
    printf("----------------------------------------\n");
    if (fgets(float_str, sizeof(float_str), stdin) == NULL) {
        printf("Ошибка ввода действительного числа\n");
        return INP_ERROR;
    }
    float_str[strcspn(float_str, "\n")] = '\0';
    if (parse_float(float_str,&float_num)) {
        printf("Ошибка ввода действительного числа\n");
        return INP_ERROR;
    }
    
    /*
    printf("%c%s E %d\n", integer_num.sign, integer_num.mantissa, integer_num.exponent);
    printf("%c%s E %d\n", float_num.sign, float_num.mantissa, float_num.exponent);
    */
    
    if (multiply_numbers(&integer_num, &float_num, &result)) {
        printf("Ошибка умножения\n");
        return MULT_ERROR;
    }
    if(print_normalized(&result)) {
        printf("Переполнение порядка\n");
        return PRINT_ERROR;
    }
    
    return 0;
}
