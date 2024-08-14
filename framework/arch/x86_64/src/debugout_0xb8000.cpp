#include <fosd/debug.hpp>

static struct TextScreenInformation {
    unsigned char *vmem;
    int width;
    int height;
    debug_color_t color_background;
    debug_color_t color_foreground;
    int x;
    int y;
}scrinfo;

void debug::out::init(void) {
    scrinfo.vmem = (unsigned char *)0xB8000;
    scrinfo.width = 80;
    scrinfo.height = 25;
    scrinfo.color_background = 0x00;
    scrinfo.color_foreground = 0x07;
}

void debug::out::clear_screen(debug_color_t color) {
    unsigned char *vmem = (unsigned char *)scrinfo.vmem;
    for(int i = 0; i < scrinfo.width*scrinfo.height*2; i += 2) {
        vmem[i] = 0x00;
        vmem[i+1] = color;
    }
    set_position(0 , 0);
}

debug_color_t debug::out::debugcolor(debug_mode_t mode) {
    switch(mode) {
        case DEBUG_NONE:      return 0x07;
        case DEBUG_TEXT:      return 0x07;
        case DEBUG_INFO:      return 0x03;
        case DEBUG_SPECIAL:   return 0x02;
        case DEBUG_ERROR:     return 0x04;
        case DEBUG_PANIC:     return 0x04;
        case DEBUG_WARNING:   return 0x06;
        case DEBUG_RAW_PRINT: return 0x07;
    }
    return 0x07;
}

static void process_newline(void) {
    if(scrinfo.y > 24) { // fix scrolling problem
        int x = 0 , y = 0;
        int elev = 1;
        for(y = 0; y < scrinfo.height-elev; y++) {
            for(x = 0; x < scrinfo.width; x++) {
                scrinfo.vmem[(y*scrinfo.width+x)*2] = scrinfo.vmem[((y+elev)*scrinfo.width+x)*2];
                scrinfo.vmem[(y*scrinfo.width+x)*2+1] = scrinfo.vmem[((y+elev)*scrinfo.width+x)*2+1];
            }
        }
        for(int y = scrinfo.height-elev; y < scrinfo.height; y++) {
            for(x = 0; x < scrinfo.width; x++) {
                scrinfo.vmem[(y*scrinfo.width+x)*2] = 0x00;
                scrinfo.vmem[(y*scrinfo.width+x)*2+1] = scrinfo.color_background;
            }
        }
        scrinfo.y = 24;
    }
}

void debug::out::print_str(const char *str) {
    int off;
    for(int i = 0; str[i] != 0; i++) {
        off = (scrinfo.y*scrinfo.width*2)+scrinfo.x*2;
        switch(str[i]) {
            case '\n':
                scrinfo.x = 0;
                scrinfo.y++;
                process_newline();
                break;
            case '\b':
                scrinfo.x -= 1;
                if(scrinfo.x < 0) {
                    scrinfo.x = 0;
                }
                scrinfo.vmem[(scrinfo.y*scrinfo.width*2)+scrinfo.x*2] = 0x00;
                break;
            case '\r':
                scrinfo.x = 0;
                break;
            case '\t':
                scrinfo.x += 5;
                break;
            default:
                scrinfo.vmem[off] = str[i];
                scrinfo.vmem[off+1] = (scrinfo.color_background << 4)|scrinfo.color_foreground;
                scrinfo.x++;
                if(scrinfo.x > 79) {
                    scrinfo.x = 0;
                    scrinfo.y++;
                }
                process_newline();
                break;
        }
    }
}


void debug::out::set_position(int x , int y) {
    scrinfo.x = x;
    scrinfo.y = y;
}

void debug::out::move_position(int x , int y) {
    scrinfo.x += x;
    scrinfo.y += y;
    if(scrinfo.x < 0) {
        scrinfo.x = 0;
    }
    if(scrinfo.y < 0) {
        scrinfo.y = 0;
    }
    if(scrinfo.x >= scrinfo.width-1) {
        scrinfo.x = scrinfo.width-1;
    }
    if(scrinfo.y >= scrinfo.height-1) {
        scrinfo.y = scrinfo.height-1;
    }
}

void debug::out::get_scr_info(int &x , int &y , debug_color_t &background_color , debug_color_t &foreground_color) {
    x = scrinfo.x;
    x = scrinfo.y;
    background_color = scrinfo.color_background;
    foreground_color = scrinfo.color_foreground;
}

void debug::out::set_background_color(debug_color_t background_color) {
    scrinfo.color_background = background_color;
}

void debug::out::set_foreground_color(debug_color_t foreground_color) {
    scrinfo.color_foreground = foreground_color;
}