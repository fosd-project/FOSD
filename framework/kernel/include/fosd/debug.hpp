/**
 * The FOSD Debugging System
 * Check out document for 
 */

#ifndef _DEBUG_HPP_
#define _DEBUG_HPP_

#include <fosd/types.hpp>
#include <string.hpp>
#include <stdarg.h>

#define DEBUG_NONE      0b00000001
#define DEBUG_TEXT      0b00000010 // white
#define DEBUG_INFO      0b00000100 // blue
#define DEBUG_SPECIAL   0b00001000 // light green
#define DEBUG_WARNING   0b00010000 // yellow
#define DEBUG_ERROR     0b00100000 // red
#define DEBUG_PANIC     0b01000000 // red
#define DEBUG_RAW_PRINT 0b10000000 // raw

#define DEBUG_OPTION_DISPLAY_INFO  0b0000001

#define DISABLE_DEBUG_MSG debug::disable();
#define ENABLE_DEBUG_MSG  debug::enable();

typedef uint16_t debug_mode_t;
typedef uint32_t debug_color_t;

namespace debug {
    namespace out {
        void vprintf(debug_mode_t mode , const char *fmt , va_list ap);
        void printf(debug_mode_t mode , const char *fmt , ...);
        void printf(const char *fmt , ...);

        void ARCHDEP init(void);
        void ARCHDEP clear_screen(debug_color_t color);        
        void ARCHDEP print_str(const char *str);
    
        void    ARCHDEP set_position(int x , int y);
        void    ARCHDEP move_position(int relative_x , int relative_y);
        void    ARCHDEP get_scr_info(int &x , int &y , debug_color_t &background_color , debug_color_t &foreground_color);
        uint8_t ARCHDEP get_char(int y , int x);
        
        void ARCHDEP set_background_color(debug_color_t background_color);
        void ARCHDEP set_foreground_color(debug_color_t foreground_color);

        debug_color_t ARCHDEP debugcolor(debug_mode_t mode);
        const char   *ARCHDEP debugstr(debug_mode_t mode);
    }
    void init(void);

    void dump_memory(max_t address , max_t length , bool debug_string=false);
    void set_option(uint16_t option , bool flag);
    void display_set(uint16_t option);
    void display_mask(uint16_t option);

    void enable(void);
    void disable(void);

    void panic_line(const char *source , int line , const char *fmt , ...);
    void panic(const char *fmt , ...);
    void panic(void);
}

#endif