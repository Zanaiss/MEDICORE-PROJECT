#include "CustomStringHelper.h"

int myStrLen(const char* str) {
    if (!str) return 0;
    int len = 0;
    while (str[len] != '\0') len++;
    return len;
}

void myStrCpy(char* dest, const char* src) {
    if (!dest || !src) return;
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

int myStrCmp(const char* str1, const char* str2) {
    if (!str1 || !str2) return 0;
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) return str1[i] - str2[i];
        i++;
    }
    return str1[i] - str2[i];
}

bool myStrEqualsIgnoreCase(const char* str1, const char* str2) {
    if (!str1 || !str2) return false;
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        char c1 = (str1[i] >= 'A' && str1[i] <= 'Z') ? str1[i] + 32 : str1[i];
        char c2 = (str2[i] >= 'A' && str2[i] <= 'Z') ? str2[i] + 32 : str2[i];
        if (c1 != c2) return false;
        i++;
    }
    return str1[i] == '\0' && str2[i] == '\0';
}

int myAtoi(const char* str) {
    if (!str) return 0;
    int res = 0, sign = 1, i = 0;
    if (str[0] == '-') { sign = -1; i++; }
    for (; str[i] != '\0'; ++i) {
        if (str[i] >= '0' && str[i] <= '9') {
            res = res * 10 + str[i] - '0';
        }
        else break;
    }
    return sign * res;
}


float myAtof(const char* str) {
    if (!str) return 0.0f;
    float res = 0.0f;
    float fraction = 1.0f;
    int sign = 1;
    int i = 0;

    if (str[i] == '-') { sign = -1; i++; }

    for (; str[i] != '\0' && str[i] != '.'; ++i) {
        if (str[i] >= '0' && str[i] <= '9') {
            res = res * 10.0f + (str[i] - '0');
        }
        else break;
    }

    if (str[i] == '.') {
        i++;
        for (; str[i] != '\0'; ++i) {
            if (str[i] >= '0' && str[i] <= '9') {
                fraction *= 0.1f;
                res += (str[i] - '0') * fraction;
            }
            else break;
        }
    }
    return sign * res;
}

void myItoa(int num, char* str) {
    if (!str) return;
    int i = 0;
    bool isNegative = false;
    if (num == 0) { str[i++] = '0'; str[i] = '\0'; return; }
    if (num < 0) { isNegative = true; num = -num; }

    while (num != 0) {
        int rem = num % 10;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / 10;
    }
    if (isNegative) str[i++] = '-';
    str[i] = '\0';

    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++; end--;
    }
}