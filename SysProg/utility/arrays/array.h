#pragma once

#ifndef LAB1T1_ARRAY_H
#define LAB1T1_ARRAY_H

#include <stdio.h>
#include <strops.h>

typedef struct Array {
    char* val;
    unsigned int length;
    unsigned int capacity;
} Array;

int create_arr(unsigned int length, Array* arr);
int append(Array* arr, char value);
void reset(Array* arr);
int resize(Array* arr, int size_delta);
int extend(Array* arr);
void destroy(Array* arr);
int copy(Array* dst, const Array *src);
int reverse(Array* arr);
void print_arr(Array arr);
void reverse_print_arr(Array arr);
int value_to_arr(unsigned int value, Array* result);
void arr_to_value(Array arr, int *result);
int value_to_arr_base(unsigned int value, int base, Array* result);
int str_to_arr(char* str, Array* result);
int add(Array A, unsigned int B, Array* result);
int add_arrays(Array A, Array B, Array* result);
int add_arrays_base(Array A, Array B, Array* result, int base);
int multiply(Array A, int B, int base, Array *result);
int multiply_arrays(Array A, Array B, int base, Array* result);
int concat(Array* A, Array B);
int concat_str_to_arr(Array* A, char* B);
int slice(Array A, int start, int stop, int step, Array* result);
int base_char_to_dec(char x);
int seek_char(FILE **f, int *result);
int read_value(FILE **f, Array *result, char first);
int read_value_to_sc(FILE **f, Array *result, char first);
int read_whole_input(Array *result);
int is_arr_equal(Array str1, Array str2);
int arr_compare(Array str1, Array str2);
int compare_array_str(Array arr, const char *str);



#endif //LAB1T1_ARRAY_H
