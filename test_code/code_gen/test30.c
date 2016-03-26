/* Tests while loops */
extern void print_string(char string[]);
extern void print_int(int n);

void main(void) {
    int x;
    x = 5;

    print_string("Before loop, x = ");
    print_int(x);
    print_string("\n");

    while(x > 0) {
        print_string("x = ");
        print_int(x);
        print_string("\n");
        x = x - 1;
    }

    while(0 != 0) {
        print_string("This should not print!\n");
    }

    print_string("After loop, x = ");
    print_int(x);
    print_string("\n");
}
