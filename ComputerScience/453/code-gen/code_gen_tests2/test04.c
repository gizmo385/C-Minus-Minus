extern void print_string(char string[]);
extern void print_int(int n);

void print_factorial(int n) {
    int i, acc;
    acc = 1;

    for(i = n; i > 0; i = i - 1) {
        acc = acc * i;
    }

    print_string("The factorial of ");
    print_int(n);
    print_string(" is ");
    print_int(acc);
    print_string(".\n");
}

void main(void) {
    print_factorial(5);
    print_factorial(0);

    for(;1 > 1;) {
        print_string("This should not print!\n");
    }
}
