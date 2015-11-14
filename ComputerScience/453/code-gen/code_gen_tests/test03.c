extern void print_int(int x);
extern void print_string(char x[]);

char x;

void foo(char y)
{
    int a;
    char b;

    print_int(y);
    print_string("\n");

    x = y;

    print_int(x);
    print_string("\n");

    a = y;

    print_int(a);
    print_string("\n");

    b = 'b';

    print_int(b);
    print_string("\n");
}

void main(void)
{
    foo('a');
}
