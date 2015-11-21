/* Function returns and recursion */
extern void print_string(char string[]);
extern void print_int(int n);

int identity(int n) {
    return n;
}

int recursive_fact(int n) {
    if(n < 2) {
        return n;
    } else {
        return n * recursive_fact(n - 1);
    }
}

void main(void) {
    int x, n;
    n = 5;
    x = identity(n);

    if(x == n) {
        print_string("x == n --> CORRECT\n");
    } else {
        print_string("x == n --> INCORRECT\n");
        print_string("x = ");
        print_int(x);
        print_string("\n");
    }

    print_string("The factorial of ");
    print_int(n);
    print_string(" is ");
    print_int(recursive_fact(n));
    print_string("\n");
}
