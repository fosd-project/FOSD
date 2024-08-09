extern void main(int argc , char **argv);

extern "C" void kernel_init(void) {
    char *argv[] = {"hello" , "world"};
    main(0 , argv);
    
    while(1) {}
}