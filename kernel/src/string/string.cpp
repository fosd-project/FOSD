#include <string.hpp>

#define size_t unsigned int

void *memset(void *s , int c , size_t n) {
    unsigned char *ptr = (unsigned char *)s;
    for(size_t i = 0; i < n; i++) {
        *ptr++ = c;
    }
    return ptr;
}

void *memcpy(void *dest , const void *src , size_t n) {
    unsigned char *dest_ptr = (unsigned char *)dest;
    unsigned char *src_ptr = (unsigned char *)src;
    for(size_t i = 0; i < n; i++) {
        *dest_ptr++ = *src_ptr++;
    }
    return dest;
}

int memcmp(const void *s1 , const void *s2 , size_t n) {
    unsigned char *s1_ptr = (unsigned char *)s1;
    unsigned char *s2_ptr = (unsigned char *)s2;
    for(size_t i = 0; i < n; i++) {
        if(*(s1_ptr+i) < *(s2_ptr+i)) return -1;
        else if(*(s1_ptr+i) > *(s2_ptr+i)) return 1;
    }
    return 0;
}

size_t strlen(const char *s) {
    size_t sz = 0;
    while(s[sz] != 0) {
        sz++;
    }
    return sz;
}

char *strcpy(char *dest , const char *src) {
    unsigned long i;
    unsigned long OriginLength = strlen(src);
    for(i = 0; i < OriginLength; i++) {
        dest[i] = src[i];
    }
    dest[i] = 0x00;
    return dest;
}

char *strncpy(char *dest , const char *src , size_t n) {
    size_t i;
    size_t length = strlen(src);
    if(length > n) length = n;
    for(i = 0; i < length; i++) {
        dest[i] = src[i];
    }
    dest[i]  = 0;
    return dest;
}

char *strcat(char *dest , char *src) {
    size_t i;
    size_t dest_sz = strlen(dest);
    size_t src_sz = strlen(src);
    for(i = dest_sz; i < dest_sz+src_sz; i++) {
        dest[i] = src[i-dest_sz];
    }
    dest[i] = 0;
    return dest;
}

char *strncat(char *dest , const char *src , size_t n) {
    size_t i;
    size_t dest_sz = strlen(dest);
    for(i = dest_sz; i < dest_sz+n; i++) {
        dest[i] = src[i-dest_sz];
    }
    dest[i] = 0;
    return dest;
}

int strcmp(const char *s1 , const char *s2) {
    size_t s1_sz = strlen(s1) , s2_sz = strlen(s2);
    if(s1_sz > s2_sz) return 1;
    if(s1_sz < s2_sz) return -1;
    for(size_t i = 0; s1[i] != 0 && s2[i] != 0; i++) {
        if(s1[i] > s2[i]) return 1;
        else if(s1[i] < s2[i]) return -1;
    }
    return 0;
}

int strncmp(const char *s1 , const char *s2 , size_t n) {
    for(size_t i = 0; s1[i] != 0 && s2[i] != 0 && i < n; i++) {
        if(s1[i] > s2[i]) return 1;
        else if(s1[i] < s2[i]) return -1;
    }
    return 0;
}


int atoi(const char *nptr) {
    int result = 0;
    int sign = (nptr[0] == '-') ? -1 : 1;
    int i = (sign > 0) ? 0 : 1;
    if(nptr[0] == '+') i = sign = 1;
    for(; nptr[i] != 0; i++) {
        result = result*10+(nptr[i]-'0');
    }
    return result*sign;
}

long int atol(const char *nptr) {
    long int result = 0;
    long int sign = (nptr[0] == '-') ? -1 : 1;
    int i = (sign > 0) ? 0 : 1;
    if(nptr[0] == '+') i = sign = 1;
    for(; nptr[i] != 0; i++) {
        result = result*10+(nptr[i]-'0');
    }
    return result*sign;
}

long long int atoll(const char *nptr) {
    long long int result = 0;
    long long int sign = (nptr[0] == '-') ? -1 : 1;
    int i = (sign > 0) ? 0 : 1;
    if(nptr[0] == '+') i = sign = 1;
    for(; nptr[i] != 0; i++) {
        result = result*10+(nptr[i]-'0');
    }
    return result*sign;
}

// Source from https://www.strudel.org.uk/itoa/
/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 * 
 */
char* itoa(int value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }
	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;
	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );
	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}