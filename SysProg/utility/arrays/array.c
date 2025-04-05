#include "array.h"
#include "../error_handler.h"

int create_arr(unsigned int length, Array *arr) {

    arr->capacity = length;
    arr->length = 0;
    arr->val = (char *) malloc(length);

    if (!arr->val) return throw_err(MEMORY_NOT_ALLOCATED);
    arr->val[0] = '\0';

    return 0;
}

int append(Array *arr, char value) {
    if (arr->length >= (arr->capacity - 2)) {
        int err = extend(arr);
        if (err)
            return err;
    }

    arr->val[arr->length++] = value;
    arr->val[arr->length] = '\0';
    return 0;
}

void reset(Array *arr) {
    arr->val[0] = '\0';
    arr->length = 0;
}

int resize(Array *arr, int size_delta) {
    char *new_addr = (char *) realloc(arr->val, arr->capacity + size_delta);

    if (!new_addr) {
        free(arr->val);
        arr->val = NULL;

        return throw_err(MEMORY_NOT_ALLOCATED);
    }

    arr->val = new_addr;
    arr->capacity += size_delta;
    return 0;
}

// Double array length.
int extend(Array *arr) {
    return resize(arr, arr->capacity);
}

void destroy(Array *arr) {
    if (arr->val)
        free(arr->val);
    arr->val = NULL;
    arr->length = 0;
    arr->capacity = 0;
}

int copy(Array *dst, const Array *src) {
    destroy(dst);
    dst->val = (char *) calloc(src->capacity, sizeof(char));
    if (!dst->val) {
        return throw_err(MEMORY_NOT_ALLOCATED);
    }
    dst->length = src->length;
    dst->capacity = src->capacity;

    char *tempdst = dst->val, *tempsrc = src->val;
    while ((*tempdst++ = *tempsrc++));
//    dst->val = tempdst;
//    src->val = tempsrc;
    return 0;
}

void print_arr(const Array arr) {
    for (size_t i = 0; i < arr.length; ++i) {
        printf("%c", arr.val[i]);
    }
    printf("\n");
}

void reverse_print_arr(const Array arr) {
    for (int i = arr.length - 1; i >= 0; --i) {
        printf("%u", arr.val[i]);
    }
}

int reverse(Array *arr) {
    int size = arr->length;
    for (int i = 0; i < size / 2; ++i) {
        char temp = arr->val[i];
        arr->val[i] = arr->val[size - i - 1];
        arr->val[size - i - 1] = temp;
    }

    return 0;
}

int value_to_arr(unsigned int value, Array *result) {
    if (result)
        destroy(result);
    int err = create_arr(10, result);
    if (err)
        return err;
    while (value > 0) {
        err = append(result, value % 10 + '0');
        if (err) {
            destroy(result);
            return err;
        }
        value /= 10;
    }
    reverse(result);
    return 0;
}

int value_to_arr_base(unsigned int value, int base, Array *result) {
    if (result)
        destroy(result);
    int err = create_arr(10, result);
    if (err)
        return err;

    char letters[37] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (value > 0) {
        err = append(result, letters[value % base]);
        if (err) {
            destroy(result);
            return err;
        }
        value /= base;
    }
    reverse(result);
    return 0;
}

int str_to_arr(char *str, Array *result) {
    if (result)
        destroy(result);
    int err = create_arr(10, result), i = 0;
    if (err)
        return err;
    while (str[i]) {
        if (str[i] == '0' && result->length == 0) {
            i++;
            continue;
        }
        err = append(result, str[i++]);
        if (err)
            return err;
    }
    return 0;

}

void arr_to_value(Array arr, int *result) {
    int sign = 1;

    if (arr.val[0] == '-')
        sign = -1;

    *result = 0;
    for (size_t i = (-sign + 1) / 2; i < arr.length; ++i) {
        *result *= 10;
        *result += arr.val[i] - '0';
    }

    *result *= sign;
}

int add(const Array A, unsigned int B, Array *result) {
    if (result)
        destroy(result);
    int err = create_arr(5, result);
    if (err)
        return err;

    int i = 0, sum = 0, shift = 0;

    while (B > 0) {
        if (A.val[i])
            sum = A.val[i++] - '0' + B % 10 + shift;
        else
            sum = B % 10 + shift;
        shift = sum / 10;
        append(result, sum % 10 + '0');
        B /= 10;
    }

    while (shift > 0) {
        append(result, shift % 10 + '0');
        shift /= 10;
    }
    return 0;
}

int add_arrays(Array A, Array B, Array *result) {
    if (result)
        destroy(result);
    int err = create_arr(5, result);
    if (err)
        return err;

    int i = 0, sum = 0, shift = 0;
    reverse(&A);
    reverse(&B);

    while (A.val[i] && B.val[i]) {
        sum = A.val[i] - '0' + B.val[i] - '0' + shift;
        shift = sum / 10;
        append(result, sum % 10 + '0');
        i++;
    }

    int j = i;
    while (A.val[i]) {
        sum = A.val[i] - '0' + shift;
        shift = sum / 10;
        append(result, sum % 10 + '0');
        i++;
    }
    while (B.val[j]) {
        sum = B.val[j] - '0' + shift;
        shift = sum / 10;
        append(result, sum % 10 + '0');
        j++;
    }

    while (shift > 0) {
        append(result, shift % 10 + '0');
        shift /= 10;
    }

    reverse(result);
    reverse(&A);
    reverse(&B);

    return 0;
}

int base_char_to_dec(char x) {
    if (is_digit(x))
        return x - '0';
    if (is_letter(x))
        return to_lower(x) - 'a' + 10;
    return throw_err(-2);
}

int add_arrays_base(Array A, Array B, Array *result, int base) {
    if (base < 2 || base > 36)
        return throw_err(INCORRECT_ARGUMENTS);

    if (result)
        destroy(result);
    int err = create_arr(5, result);
    if (err)
        return err;

    reverse(&A);
    reverse(&B);

    char letters[37] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    int i = 0, sum = 0, shift = 0;
    // 1000 0100 0010 0001
    // 0001 0010 0100 1000

    while (A.val[i] && B.val[i]) {
        int Adec = base_char_to_dec(A.val[i]);
        int Bdec = base_char_to_dec(B.val[i]);
        if (Adec >= base || Bdec >= base)
            return throw_err(INCORRECT_ARGUMENTS);

        sum = Adec + Bdec + shift;
        shift = sum / base;
        append(result, letters[sum % base]);
        i++;
    }

    int j = i;
    while (A.val[i]) {
        int Adec = base_char_to_dec(A.val[i]);
        if (Adec >= base)
            return throw_err(INCORRECT_ARGUMENTS);

        sum = Adec + shift;
        shift = sum / base;
        append(result, letters[sum % base]);
        i++;
    }
    while (B.val[j]) {
        int Bdec = base_char_to_dec(B.val[j]);
        if (Bdec >= base)
            return throw_err(INCORRECT_ARGUMENTS);

        sum = Bdec + shift;
        shift = sum / base;
        append(result, letters[sum % base]);
        j++;
    }

    while (shift > 0) {
        append(result, letters[shift % base]);
        shift /= base;
    }

    reverse(result);
    reverse(&A);
    reverse(&B);

    return 0;
}

int multiply(Array A, int B, int base, Array *result) {
    if (result)
        destroy(result);
    int err = create_arr(5, result);
    if (err) {
        destroy(result);
        return err;
    }

    reverse(&A);

    char letters[37] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    int shift = 0;

    for (size_t i = 0; i < A.length; i++) {
        int product = base_char_to_dec(A.val[i]) * B + shift;
        shift = product / base;
        err = append(result, letters[product % base]);
        if (err) {
            destroy(result);
            return err;
        }
    }

    while (shift > 0) {
        err = append(result, letters[shift % base]);
        if (err) {
            destroy(result);
            return err;
        }
        shift /= base;
    }

    reverse(result);
    reverse(&A);

    return 0;
}

int multiply_arrays(Array A, Array B, int base, Array *result) {
    if (base < 2 || base > 36)
        return throw_err(INCORRECT_ARGUMENTS);

//    char letters[37] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    int i = 0;
    Array temp, temp2;
    int err = create_arr(5, &temp);
    if (err)
        return err;

    err = create_arr(5, &temp2);
    if (err)
        return err;
    append(&temp2, '0');

    // 120
    // 210
    // MULTIPLY EVERY DIGIT AND ADD THEM UP

    while (B.val[i]) {
        multiply(A, base_char_to_dec(B.val[i]), base, &temp);
//        for (int j = 0; j < shift; ++j) {
//        }
//        if (i + 1 < B.length)
        add_arrays_base(temp, temp2, result, base);
        copy(&temp2, result);
        append(&temp2, '0');
//        shift++;
        i++;
    }

//    int j = i;
//    while (A.val[i]) {
//        int Adec = base_char_to_dec(A.val[i]);
//        if (Adec >= base)
//            return throw_err(INCORRECT_ARGUMENTS);
//
//        sum = Adec + shift;
//        shift = sum / base;
//        append_int(result, letters[sum % base]);
//        i++;
//    }
//    while (B.val[j]) {
//        int Bdec = base_char_to_dec(B.val[j]);
//        if (Bdec >= base)
//            return throw_err(INCORRECT_ARGUMENTS);
//
//        sum = Bdec + shift;
//        shift = sum / base;
//        append_int(result, letters[sum % base]);
//        j++;
//    }
//
//    while (shift > 0) {
//        append_int(result, letters[shift % base]);
//        shift /= base;
//    }

    destroy(&temp);
    destroy(&temp2);

    return 0;
}

// Concatenate B to A.
int concat(Array *A, Array B) {
    for (size_t i = 0; i < B.length; ++i) {
        int err = append(A, B.val[i]);
        if (err)
            return err;
    }

    return 0;
}

// Concatenate B to A.
int concat_str_to_arr(Array *A, char *B) {
    int i = 0;
    while (B[i] != '\0') {
        int err = append(A, B[i++]);
        if (err)
            return err;
    }

    return 0;
}

int slice(Array A, int start, int stop, int step, Array *result) {
    for (int i = start; i < stop; i += step) {
        if (i >= (int) A.length)
            return throw_err(INCORRECT_ARGUMENTS);
        if (result->length == 0 && A.val[i] == '0')
            continue;
        append(result, A.val[i]);
    }

    return 0;
}

int seek_char(FILE **f, int *result) {
    if (!(*f))
        return throw_err(FILE_ERROR);

    while (!feof(*f)) {
        int cur = fgetc(*f);
        if (cur > ' ') {
            *result = cur;
            return 0;
        }
    }
    *result = -1;
    return 0;
}

int read_value(FILE **f, Array *result, char first) {
    int err;

    if (first) {
        err = append(result, first);
        if (err)
            return err;
    }

    int character = fgetc(*f);

    while (character > ' ') {
        err = append(result, (char) character);
        if (err)
            return err;
        character = fgetc(*f);
    }

    if (*f != stdin)
        fseek(*f, -1, SEEK_CUR);

    if (character == -1)
        return -1;

    return 0;
}

int read_value_to_sc(FILE **f, Array *result, char first) {
    int err;

    if (first) {
        err = append(result, first);
        if (err)
            return err;
    }

    int character = fgetc(*f);

    while (character > ' ' && !is_special_character(character)) {
        err = append(result, (char) character);
        if (err)
            return err;
        character = fgetc(*f);
    }

    if (*f != stdin)
        fseek(*f, -1, SEEK_CUR);

    if (character == -1)
        return -1;

    return 0;
}

int read_whole_input(Array *result) {
    int err;
    int ch = fgetc(stdin);

    while (ch > 0 && ch != '\n') {
        err = append(result, (char) ch);
        if (err)
            return err;

        ch = fgetc(stdin);
    }
    return 0;
}

int is_arr_equal(Array str1, Array str2) {
    if (str1.length != str2.length) return 0;
    for (size_t i = 0; i < str1.length; ++i) {
        if (str1.val[i] != str2.val[i]) return 0;
    }
    return 1;
}

int arr_compare(Array str1, Array str2) {
    int min_length = str1.length < str2.length ? str1.length : str2.length;
    for (int i = 0; i < min_length; ++i) {
        if (str1.val[i] < str2.val[i]) return -1;
        if (str1.val[i] > str2.val[i]) return 1;
    }
    if (str1.length < str2.length) return -1;
    if (str1.length > str2.length) return 1;
    return 0;
}

int compare_array_str(Array arr, const char *str) {
    size_t i = 0;
    while (i < arr.length && str[i] != '\0') {
        if (arr.val[i] != str[i]) {
            return arr.val[i] - str[i];
        }
        i++;
    }

    if (i < arr.length) {
        return arr.val[i];
    }
    if (str[i] != '\0') {
        return -str[i];
    }

    return 0;
}


