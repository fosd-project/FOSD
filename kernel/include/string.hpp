#ifndef _ESSENTIAL_LIBRARY_HPP_
#define _ESSENTIAL_LIBRARY_HPP_

#include <fosd/types.hpp>
#include <stdarg.h>

void *memset(void *s , int c , size_t n);
void *memcpy(void *dest , const void *src , size_t n);
int memcmp(const void *s1 , const void *s2 , size_t n);

size_t strlen(const char *s);
char *strcpy(char *dest , const char *src);
char *strncpy(char *dest , const char *src , size_t n);
char *strcat(char *dest , char *src);
char *strncat(char *dest , const char *src , size_t n);
int strcmp(const char *s1 , const char *s2);
int strncmp(const char *s1 , const char *s2 , size_t n);

int vsprintf(char *buf , const char *fmt , va_list ap);
int sprintf(char *buf , const char *fmt , ...);

int atoi(const char *nptr);
long int atol(const char *nptr);
long long int atoll(const char *nptr);
char *itoa(int value , char *result , int base);
template <typename T> char *itoa_variation(T value , char *result , int base , bool lowercase=true);

#endif