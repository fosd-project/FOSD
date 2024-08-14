#ifndef _PSEUDOSTRING_H_
#define _PSEUDOSTRING_H_

#include <stdarg.h>

unsigned int memset(void *Destination , int Data , unsigned int Size);
unsigned int memcpy(void *Destination , const void *Source , unsigned int Size);
int strlen(const char *Destination);
char *strcpy(char *Destination , const char *Source);
char *strncpy(char *Destination , const char *Source , int Length);
char *strcat(char *Destination , const char *Source);
char *strncat(char *Destination , const char *Source , int Length);
int strcmp(const char *Destination , const char *Source);
int strncmp(const char *Destination , const char *Source , int Length);

unsigned long atoi(const char *String);
unsigned long atol(const char *String);
char *itoa(unsigned long Number , char *String , int radix);

void vsprintf(char *Destination , const char *Format , va_list ap);
void sprintf(char *Destination , const char *Format , ...);

#endif