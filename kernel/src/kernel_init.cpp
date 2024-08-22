#include <fosd/debug.hpp>

extern void main(int argc , char **argv);

extern "C" void kernel_init(void) {
    // call the main kernel
    debug::init();
    debug::out::clear_screen(debug::out::debugcolor(DEBUG_NONE));

    char *argv[] = {};
    main(0 , argv);
    
    while(1) {}
}