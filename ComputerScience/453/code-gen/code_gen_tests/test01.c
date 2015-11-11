extern int squares(int a);
extern void print_int(int b);
extern void print_string(char string[]);

void test(void) {
    int x, y, z, h;
    x = 5;
    y = x;
    z = y;

    h = squares(squares(squares(squares(squares(squares(squares(squares(x))))))));
}

void second_test(int a) {
    int b;
    b = a;

    print_string("Result: ");
    print_int(b);
    print_string("\n");
}
