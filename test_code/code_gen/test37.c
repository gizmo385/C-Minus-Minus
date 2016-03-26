/* Testing constants */
extern void print_int(int n);
extern void print_string(char string[]);

void main(void) {
    int a, b, c, d;

    a = -1;
    b = -2147483648;
    c = 2147483647;
    d = 65356;

    print_int(a);
    print_string("\n");
    print_int(b );
    print_string("\n");
    print_int(c);
    print_string("\n");
    print_int(d);
    print_string("\n");
}
