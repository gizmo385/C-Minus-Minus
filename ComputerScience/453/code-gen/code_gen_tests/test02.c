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
    print_int(5);
    print_string("\n");
    print_int(x);
    print_int2(x);
    print_string("\n");
}
