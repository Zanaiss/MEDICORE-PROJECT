#ifndef CUSTOM_STRING_HELPER_H
#define CUSTOM_STRING_HELPER_H

int myStrLen(const char* str);
void myStrCpy(char* dest, const char* src); 
int myStrCmp(const char* str1, const char* str2);
bool myStrEqualsIgnoreCase(const char* str1, const char* str2);
int myAtoi(const char* str);
float myAtof(const char* str); 
void myItoa(int num, char* str);

#endif