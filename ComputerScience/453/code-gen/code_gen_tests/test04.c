extern void print_int(int a);
extern void print_string(char s[]);

void main(void) {
    print_string("Before print_int call:\n");
    /* Print as a constant */
    print_int('x');
    print_string("\n");
}
