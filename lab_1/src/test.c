#include "test.h"
bool test_parse_integer() {
    struct long_num result;
    char input1[] = "12345";
    char input2[] = "-67890";
    char input3[] = "+0";
    char input4[] = "0000123";
    char input5[] = "abc";
    char input6[] = "12345678901234567890123456789012345678901";
    char input7[] = "1a2b3c";
    char input8[] = "";
    char input9[] = " 123";
    char input10[] = "123 ";
    if (parse_integer(input1, &result) != 0) {
        printf("Test 1 failed\n");
        return false;
    }
    if (parse_integer(input2, &result) != 0) {
        printf("Test 2 failed\n");
        return false;
    }
    if (parse_integer(input3, &result) != 0) {
        printf("Test 3 failed\n");
        return false;
    }
    if (parse_integer(input4, &result) != 0) {
        printf("Test 4 failed\n");
        return false;
    }
    if (parse_integer(input5, &result) != ERROR) {
        printf("Test 5 faileds\n");
        return false;
    }
    if (parse_integer(input6, &result) != ERROR) {
        printf("Test 6 failed\n");
        return false;
    };
    if (parse_integer(input7, &result) != ERROR) {
        printf("Test 7 faileds\n");
        return false;
    }
    if (parse_integer(input8, &result) != ERROR) {
        printf("Test 8 failed\n");
        return false;
    }
    if (parse_integer(input9, &result) != ERROR) {
        printf("Test 9 failed\n");
        return false;
    }
    if (parse_integer(input10, &result) != ERROR) {
        printf("Test 10 failed\n");
        return false;
    }
    printf("All tests passed\n");
    return true;
}

bool test_parse_float() {
    struct short_num result;
    char input1[] = "1.5E+5";
    char input2[] = "-2.7E-3";
    char input3[] = "4";
    char input4[] = "abc";
    char input5[] = "0.9999999999999999999999999999999";
    char input6[] = " ";
    char input7[] = "1.5E+";
    char input8[] = "1.5E";
    char input9[] = "E321+5";
    char input10[] = "1.e+1";
    char input11[] = "1.4E+123456";
    char input12[] = "HELLo12";
    char input13[] = "1.5E+-9";
    char input14[] = "123.456";
    char input15[] = "0001.2300";
    char input16[] = ".5";
    char input17[] = "5.";
    char input18[] = "1.234E2";
    if (parse_float(input1, &result) != 0) {
        printf("Test 1 failed\n");
        return false;
    }
    if (parse_float(input2, &result) != 0) {
        printf("Test 2 failed\n");
        return false;
    }
    if (parse_float(input3, &result) != 0) {
        printf("Test 3 failedt\n");
        return false;
    }
    if (parse_float(input4, &result) != ERROR) {
        printf("Test 4 failed\n");
        return false;
    }
    if (parse_float(input5, &result) != ERROR) {
        printf("Test 5 failed\n");
        return false;
    }
    if (parse_float(input6, &result) != ERROR) {
        printf("Test 6 failed\n");
        return false;
    }

    if (parse_float(input7, &result) != ERROR) {
        printf("Test 7 failed\n");
        return false;
    }

    if (parse_float(input8, &result) != ERROR) {
        printf("Test 8 failed\n");
        return false;
    }

    if (parse_float(input9, &result) != ERROR) {
        printf("Test 9 failed:\n");
        return false;
    }
    if (parse_float(input10, &result) != ERROR) {
        printf("Test 10 failed\n");
        return false;
    }
    if (parse_float(input11, &result) != ERROR) {
        printf("Test 11 failed\n");
        return false;
    }
    if (parse_float(input12, &result) != ERROR) {
        printf("Test 12 faileds\n");
        return false;
    }
    if (parse_float(input13, &result) != ERROR) {
        printf("Test 13 failed\n");
        return false;
    }
    if (parse_float(input14, &result) != 0) {
        printf("Test 14 failed\n");
        return false;
    }
    if (parse_float(input15, &result) != 0) {
        printf("Test 15 faileds\n");
        return false;
    }
    if (parse_float(input16, &result) != 0) {
        printf("Test 16 failed\n");
        return false;
    }
    if (parse_float(input17, &result) != 0) {
        printf("Test 17 failed\n");
        return false;
    }
    if (parse_float(input18, &result) != 0) {
        printf("Test 18 failed\n");
        return false;
    }

    printf("All tests passed\n");
    return true;
}

bool test_multiply_numbers() {
    struct long_num a, result;
    struct short_num b;
    parse_integer("123", &a);
    parse_float("2.0", &b);
    if (multiply_numbers(&a, &b, &result) != 0) {
        printf("Test 1 failed\n");
        return false;
    }
    parse_integer("-123", &a);
    parse_float("2.0", &b);
    if (multiply_numbers(&a, &b, &result) != 0) {
        printf("Test 2 failed\n");
        return false;
    }
    parse_integer("0", &a);
    parse_float("2.0", &b);
    if (multiply_numbers(&a, &b, &result) != 0) {
        printf("Test 3 failed\n");
        return false;
    }
    parse_integer("999999999999999999999999999999", &a);
    parse_float("2.0", &b);
    if (multiply_numbers(&a, &b, &result) != 0) {
        printf("Test 4 failed\n");
        return false;
    }
    
    printf("All tests passed\n");
    return true;
}

bool test_print_normalized() {
    struct long_num num;
    parse_integer("123", &num);
    if (print_normalized(&num) != 0) {
        printf("Test 1 failedr\n");
        return false;
    }
    parse_integer("0", &num);
    if (print_normalized(&num) != 0) {
        printf("Test 2 failed\n");
        return false;
    }
    parse_integer("-123", &num);
    if (print_normalized(&num) != 0) {
        printf("Test 3 failed\n");
        return false;
    }
    num.exponent = 100000;
    if (print_normalized(&num) != ERROR) {
        printf("Test 4 failed\n");
        return false;
    }
    num.exponent = -100000;
    if (print_normalized(&num) != ERROR) {
        printf("Test 5 failed\n");
        return false;
    }
    
    printf("All tests passed\n");
    return true;
}

bool run_all_tests() {
    bool success = true;
    
    if (!test_parse_integer()) success = false;
    if (!test_parse_float()) success = false;
    if (!test_multiply_numbers()) success = false;
    if (!test_print_normalized()) success = false;
    
    return success;
}