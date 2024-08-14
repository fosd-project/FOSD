#include <fosd/debug.hpp>

#define DEBUG_PRINTF_STR_STACK 1024

struct {
    bool info_display;
    bool function_display;
    bool func_indent;
    bool enable_debug;

    uint16_t option_flags;
}debug_info;

void debug::init(void) {
    set_option(DEBUG_OPTION_DISPLAY_INFO , true);
    debug_info.option_flags = 0b11111111;
    debug::out::init();
    ENABLE_DEBUG_MSG
}

void debug::dump_memory(max_t address , max_t length , bool debug_string) {
    uint8_t *ptr = (uint8_t *)address;
    int margin = 5;
    int char_per_line = 80;
    int x = (char_per_line-margin-3)/4; // character 
    debug::out::printf("dumping memory from 0x%X - 0x%X\n" , address , address+length);
    int line_count = (length/x)+(length%x != 0);
    debug::out::printf("x = %d\n" , x);

    // remove for display
    max_t max = 0;
    max_t offset = 0;
    for(max_t j = 1; j <= line_count; j++) {
        debug::out::printf("");

        unsigned char *old_ptr = ptr;
        max_t count = MIN(x , length-offset);
        for(max_t i = 1; i <= count; i++) {
            debug::out::printf(DEBUG_RAW_PRINT , "%02X " , *ptr);
            ptr++;
        }
        max = MAX(max , count);
        if(count < max) {
            for(max_t i = 0; i < max-count; i++) {
                debug::out::printf(DEBUG_RAW_PRINT , "   ");
            }
        }
        // debug the string
        debug::out::printf(DEBUG_RAW_PRINT , " | ");
        for(max_t i = 1; i <= count; i++) {
            unsigned char c = *old_ptr;
            switch(c) {
                case '\n':
                case '\t':
                case '\b':
                case '\r':
                case 0:
                    c = '.';
                    break ;
            }
            debug::out::printf(DEBUG_RAW_PRINT , "%c" , c);
            old_ptr++;
        }
        debug::out::print_str("\n");
        offset += x;
    }
}

void debug::enable(void) { debug_info.enable_debug = true; }
void debug::disable(void) { debug_info.enable_debug = false; }

void debug::panic_line(const char *source , int line , const char *fmt , ...) {
    va_list ap;
    va_start(ap , fmt);
    char buffer[512];
    vsprintf(buffer , fmt , ap);
    debug::out::printf(DEBUG_PANIC , "(%s:%d) %s", source , line , buffer);
    va_end(ap);
    while(1) {
        ;
    }
}

void debug::panic(const char *fmt , ...) {
    va_list ap;
    va_start(ap , fmt);
    char buffer[512];
    vsprintf(buffer , fmt , ap);
    debug::out::printf(DEBUG_PANIC , buffer);
    va_end(ap);
    while(1) {
        ;
    }
}

void debug::panic(void) {
    debug::out::printf(DEBUG_PANIC , "kernel panic\n");
    while(1) {
        ;
    }
}

const char *debug::out::debugstr(debug_mode_t mode) {
    switch(mode) {
        case DEBUG_NONE:    return "";
        case DEBUG_TEXT:    return "[ ] ";
        case DEBUG_INFO:    return "[I] ";
        case DEBUG_SPECIAL: return "[S] ";
        case DEBUG_WARNING: return "[W] ";
        case DEBUG_ERROR:   return "[E] ";
        case DEBUG_PANIC:   return "[P] ";
    }
    return "    ";
}

void debug::set_option(uint16_t option , bool flag) {
    if((option & DEBUG_OPTION_DISPLAY_INFO) == DEBUG_OPTION_DISPLAY_INFO) {
        debug_info.info_display = flag;
    }
}

void debug::display_set(uint16_t option) { debug_info.option_flags |= option; }
void debug::display_mask(uint16_t option) { debug_info.option_flags &= ~option; }

void debug::out::vprintf(debug_mode_t mode , const char *fmt , va_list ap) {
    if(debug_info.enable_debug == false) return;
    debug_color_t color = debugcolor(mode);
    set_foreground_color(color);
    if(debug_info.info_display||mode != DEBUG_RAW_PRINT) {
        print_str(debugstr(mode));
    }

    char string[DEBUG_PRINTF_STR_STACK];
    vsprintf(string , fmt , ap);
    print_str(string);

    set_foreground_color(debugcolor(DEBUG_TEXT));
}

void debug::out::printf(debug_mode_t mode , const char *fmt , ...) {
    if(debug_info.enable_debug == false) return;
    va_list ap;
    va_start(ap , fmt);
    debug::out::vprintf(mode , fmt , ap);
    va_end(ap);
}

void debug::out::printf(const char *fmt , ...) {
    if(debug_info.enable_debug == false) return;
    va_list ap;
    va_start(ap , fmt);
    debug::out::vprintf(DEBUG_TEXT , fmt , ap);
    va_end(ap);
}