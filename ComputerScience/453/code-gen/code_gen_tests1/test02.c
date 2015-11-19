extern void print_int(int number);
extern void print_string(char string[]);

int test_global;

void print_int2(int x) {
    print_string("\n");
    print_string("The number is: ");
    print_int(x);
    print_string("\n");
}

int main(void) {
    int x;
    print_string("This is a test\n");

    x = 7;
    print_int(x);
    print_string("\n");
    print_int(240);
    print_int2(12345);
    print_string("\n");
}
