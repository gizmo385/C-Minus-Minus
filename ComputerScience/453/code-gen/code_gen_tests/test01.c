extern void print_int(int b);
extern void print_string(char string[]);

void test(void) {
    int x, y, z, h;
    x = 5;
    y = x;
    z = y;
}

void second_test(int a) {
    int b;
    b = a;

    print_string("Result: ");
    print_int(b);
    print_string("\n");
}

void main(void) {
    int x;
    test();
    x= 5;
    second_test(x);
}
