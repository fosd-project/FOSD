#include <string.hpp>
#include <stdarg.h>

#define NONE 0

#define FMT_SIZE_h  1
#define FMT_SIZE_ll 2
#define FMT_SIZE_l  3
#define FMT_SIZE_L  4
#define FMT_SIZE_H  5
#define FMT_SIZE_DD 6
#define FMT_SIZE_D  7

struct FormatData {
    char flag;
    int width;
    int precision;
    int size;
    int type;
};

/// @brief get flag from format string
/// @param fmt format string
/// @param local_i local_i 
/// @return flag
char get_flag(const char *fmt , int &local_i) {
    char flag_list[] = {'-' , '+' , '#' , '0'};
    for(size_t i = 0; i < sizeof(flag_list); i++) {
        if(fmt[local_i] == flag_list[i]) {
            return fmt[local_i++];
        }
    }
    return NONE;
}

static bool isnumber(char c) {
    if('0' <= c && c <= '9') return true;
    return false;
}

int get_width(const char *fmt , int &local_i , va_list ap) {
    int i = 0;
    int width = 0;
    if(fmt[local_i] == '*') { // use from 
        width = va_arg(ap , int);
        local_i++;
        return width;
    }
    for(; isnumber(fmt[local_i+i]) == true && fmt[local_i+i] != 0; i++) {
        width = width*10+(fmt[local_i+i]-'0');
    }
    local_i = local_i+i;
    return width;
}

int get_precision(const char *fmt , int &local_i , va_list ap) {
    int i = 0;
    int precision = 0;
    if(fmt[local_i] != '.') return precision;
    local_i++;
    if(fmt[local_i] == '*') {
        precision = va_arg(ap , int);
        local_i++;
        return precision;
    }
    for(; isnumber(fmt[local_i+i]) == true && fmt[local_i+i] != 0; i++) {
        precision = precision*10+(fmt[local_i+i]-'0');
    }
    local_i = local_i+i;
    return precision;
}

int get_size(const char *fmt , int &local_i) {
    char size_lst[7][3] = {
        "h" , "ll" , "l" , "L" , "H" , "DD" , "D" , 
    };
    for(int i = 0; i < 7; i++) {
        if(strncmp(size_lst[i] , fmt+local_i , strlen(size_lst[i])) == 0) {
            local_i += strlen(size_lst[i]);
            return i+1;
        }
    }
    return NONE;
}

char get_type(const char *fmt , int &local_i) {
    return fmt[local_i];
}

template <typename T> int get_integer_digits(T value) {
    int c = 0;
    for(; value != 0; c++) {
        value /= 10;
    }
    return c;
}

void process_added_string_d_i(struct FormatData fd , char *added_string , va_list ap) {
    if(fd.size == NONE) {
        int value = va_arg(ap , int);
        itoa_variation<int>(value , added_string , 10);
    }
    else if(fd.size == FMT_SIZE_l) {
        long int value = va_arg(ap , long int);
        itoa_variation<long int>(value , added_string , 10);
    }
    else if(fd.size == FMT_SIZE_ll) {
        long long int value = va_arg(ap , long long int);
        itoa_variation<long long int>(value , added_string , 10);
    }
}

void process_added_string_u_o(struct FormatData fd , char *added_string , va_list ap) {
    int base = 10;
    base = (fd.type == 'u') ? 10 : 8;
    if(fd.size == NONE) {
        unsigned int value = va_arg(ap , unsigned int);
        itoa_variation<unsigned int>(value , added_string , base);
    }
    else if(fd.size == FMT_SIZE_l) {
        unsigned long int value = va_arg(ap , unsigned long);
        itoa_variation<unsigned long>(value , added_string , base);
    }
    else if(fd.size == FMT_SIZE_ll) {
        unsigned long long int value = va_arg(ap , unsigned long long int);
        itoa_variation<unsigned long long int>(value , added_string , base);
    }
}

void process_added_string_x_X(struct FormatData fd , char *added_string , va_list ap) {
    bool lowercase = (fd.type == 'x') ? true : false;

    if(fd.size == NONE) {
        unsigned int value = va_arg(ap , unsigned int);
        itoa_variation<unsigned int>(value , added_string , 16 , lowercase);
    }
    else if(fd.size == FMT_SIZE_l) {
        unsigned long value = va_arg(ap , unsigned long);
        itoa_variation<unsigned long>(value , added_string , 16 , lowercase);
    }
    else if(fd.size == FMT_SIZE_ll) {
        unsigned long long int value = va_arg(ap , unsigned long long int);
        itoa_variation<unsigned long long int>(value , added_string , 16 , lowercase);
    }
}

void process_format(char *buf , struct FormatData fd , int &buf_i , va_list ap) {
    char temp_buffer[512] = {0 , };

    char *added_string = temp_buffer;
    char width_fill = ' ';
    if(fd.type == 'd'||fd.type == 'i') {
        process_added_string_d_i(fd , added_string , ap);
        width_fill = (fd.flag == '0') ? fd.flag : ' ';
    }
    else if(fd.type == 'u'||fd.type == 'o') {
        process_added_string_u_o(fd , added_string , ap);
        width_fill = (fd.flag == '0') ? fd.flag : ' ';
    }
    else if(fd.type == 'x'||fd.type == 'X') {
        process_added_string_x_X(fd , added_string , ap);
        width_fill = (fd.flag == '0') ? fd.flag : ' ';
    }
    else if(fd.type == 'c') {
        added_string[0] = va_arg(ap , int);
        added_string[1] = 0;
    }
    else if(fd.type == 's') {
        added_string = va_arg(ap , char*);
    }

    if(fd.flag == '-') { // sort to left
        strcat(buf , added_string);
        if(fd.width != NONE && fd.width > strlen(added_string)) {
            int c = fd.width-strlen(added_string);
            char flag[2] = {width_fill , 0};
            for(int i = 0; i < c; i++) {
                strcat(buf , flag);
            }
            buf_i += c;
        }
        buf_i += strlen(added_string);
    }
    else {
        if(fd.width != NONE && fd.width > strlen(added_string)) {
            int c = fd.width-strlen(added_string);
            char flag[2] = {width_fill , 0};
            for(int i = 0; i < c; i++) {
                strcat(buf , flag);
            }
            buf_i += c;
        }
        strcat(buf , added_string);
        buf_i += strlen(added_string);
    }
}

// %[flags][width][.precision][size]type

int vsprintf(char *buf , const char *fmt , va_list ap) {
    int buf_i = 0;
    struct FormatData fd = {0 , 0 , 0 , 0 , 0};
    buf[0] = 0;
    for(int i = 0; fmt[i] != 0; i++) {
        if(fmt[i] == '%') {
            int local_i = i+1;
            
            fd.flag = get_flag(fmt , local_i);
            fd.width = get_width(fmt , local_i , ap);
            fd.precision = get_precision(fmt , local_i , ap);
            fd.size = get_size(fmt , local_i);
            fd.type = get_type(fmt , local_i);
            process_format(buf , fd , buf_i , ap);
            i = local_i;
        }
        else {
            buf[buf_i++] = fmt[i];
            buf[buf_i] = 0;
        }
    }
    return buf_i;
}

int sprintf(char *buf , const char *fmt , ...) {
    va_list ap;
    va_start(ap , fmt);
    int ret = vsprintf(buf , fmt , ap);
    va_end(ap);
    return ret;
}

template <typename T> char* itoa_variation(T value, char* result, int base , bool lowercase) {
	// check that the base if valid
    char sample_str_low[] =  "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz";
    char sample_str_high[] = "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *sample_str = (lowercase) ? sample_str_low : sample_str_high;
	if (base < 2 || base > 36) { *result = '\0'; return result; }
	char* ptr = result, *ptr1 = result, tmp_char;
	T tmp_value;
	do {
		tmp_value = value;
		value /= base;
		*ptr++ = sample_str[35 + (tmp_value - value * base)];
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