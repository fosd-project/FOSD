#include <basicstring.h>

unsigned int memset(void *dest , int d , unsigned int size) {
	int i;
	for(i = 0; i < size; i++) {
		((unsigned char*)dest)[i] = d;
	}
	return size;
}

unsigned int memcpy(void *dest , const void *src , unsigned int size) {
    int i;
    for(i = 0; i < size; i++) {
        ((unsigned char*)dest)[i] = ((unsigned char*)src)[i];
    }
	return size;
}

int strlen(const char *dest) {
    int i = 0;
	for(; dest[i] != '\0'; i++) {}
	return i;
}

char *strcpy(char *dest , const char *src) {
	int i;
	memset(dest , 0 , sizeof(dest));
	for(i = 0; (dest[i] != '\0')||(src[i] != '\0'); i++) {
		dest[i] = src[i];
	}
	return dest;
}

char *strncpy(char *dest , const char *src , int len) {
	int i;
	memset(dest , 0 , sizeof(dest));
	for(i = 0; i < len; i++) {
		dest[i] = src[i];
	}
	return dest;
}

char *strcat(char *dest , const char *src) {
	int i = 0;
	int j = 0;
	while(dest[i] != '\0') {
		i++;
	}
	while(src[j] != '\0') {
		dest[i++] = src[j++];
	}
	dest[i] = '\0';
	return dest;
}

char *strncat(char *dest , const char *src , int len) {
	int i = 0;
	int j;
	while(dest[i] != '\0') {
		i++;
	}
	for(j = 0; j < len; j++) {
		dest[i++] = src[j++];
	}
	dest[i] = '\0';
	return dest;
}

int strcmp(const char *dest , const char *src) {
	int i;
	int not_equal = 0;
	for(i = 0; (dest[i] != '\0')||(src[i] != '\0'); i++) {
		if(dest[i] != src[i]) {
			not_equal++;
		}
	}
	return not_equal;
}

int strncmp(const char *dest , const char *src , int len) {
	int i;
	int not_equal = 0;
	for(i = 0; i < len; i++) {
		if(dest[i] != src[i]) {
			not_equal++;
		}
	}
	return not_equal;
}

unsigned long atoi(const char *str) {
	unsigned long num = 0;
	int positive = 1;
	if(*str == '-') {
		positive = -1;
		str++;
	}
	if(*str == '+') {
		positive = 1;
		str++;
	}
	if(*str == '\0') {
		return 0;
	}
	while(*str != 0x00) {
		if((*str >= '0') && (*str <= '9')) {
			num = num*10+(*str)-'0';
			str++;
		}
	}
	num *= positive;
	return num;
}

unsigned long atol(const char *str) {
	unsigned long num = 0;
	int positive = 1;
	if(*str == '-') {
		positive = -1;
		str++;
	}
	if(*str == '+') {
		positive = 1;
		str++;
	}
	if(*str == '\0') {
		return 0;
	}
	while(*str != 0x00) {
        num *= 16;
		if(((*str >= 'A') && (*str <= 'Z'))) {
			num += ((*str)-'A')+10;
			str++;
		}
        else if(((*str >= 'a') && (*str <= 'z'))) {
			num += ((*str)-'a')+10;
			str++;
        }
        else {
            num += ((*str)-'0');
            str++;
        }
	}
	num *= positive;
	return num;
}

char *itoa(unsigned long num , char *str , int Radix) {
	int i = 0;
	int len;
	unsigned long TempBuffer;
	char tmp;
	if(num == 0) {
		str[0] = '0';
		str[1] = '\0';
		return str;
	}
	while(1) {
		if(num == 0) {
			break;
		}
		if(Radix <= 10) {
			str[i++] = (num%Radix)+'0';
		}
		else {
			TempBuffer = num%Radix;
			if(TempBuffer <= 9) {
				str[i++] = TempBuffer+'0';
			}
			else {
				str[i++] = TempBuffer-10+'A';
			}
		}
		num /= Radix;
	}
	str[i] = 0x00;
	len = i;
	for(i = 0; i < len/2; i++) {
		tmp = str[i];
		str[i] = str[strlen(str)-i-1];
		str[strlen(str)-i-1] = tmp;
	}
	return str;
}

void vsprintf(char *dest , const char *fmt , va_list ap) {
	int i;
	int j = 0;
	for(i = 0; i < strlen(fmt); i++) {
		switch(fmt[i]) {
			case '%':
				switch(fmt[i+1]) {
					case 'c': {
						unsigned char buffer = va_arg(ap , int);
						dest[j++] = buffer;
						i++;
						break;
					}
					case 's': {
						static char buffer[4096];
						memset(buffer , 0 , 4096);
						strcpy(buffer , va_arg(ap , char*));
						strcpy(dest+j , buffer);
						j += strlen(buffer);
						i++;
						break;
					}
					case 'd':
					case 'i': {
						unsigned long value = va_arg(ap , unsigned long);
						static char buffer[128];
						memset(buffer , 0 , 128);
						itoa(value , buffer , 10);
						strcpy(dest+j , buffer);
						j += strlen(buffer);
						i++;
						break;
					}
					case 'X': {
						unsigned long value = va_arg(ap , unsigned long);
						static char buffer[128];
						memset(buffer , 0 , 128);
						itoa(value , buffer , 16);
						strcpy(dest+j , buffer);
						j += strlen(buffer);
						i++;
						break;
					}
					case 'f': {
						break; 
					}
					case '%': {
						dest[j++] = '%';
						i++;
						break;
					}
					default: {
						i++;
						break;
					}
				}
				break;
			default:
				dest[j++] = fmt[i];
				break;
		}
	}
	dest[j] = '\0';
}

void sprintf(char *dest , const char *fmt , ...) {
	va_list ap;
	va_start(ap , fmt);
	vsprintf(dest , fmt , ap);
	va_end(ap);
}
