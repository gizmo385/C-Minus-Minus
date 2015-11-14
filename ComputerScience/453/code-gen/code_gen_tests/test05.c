extern void print_string(char s[]);
extern void print_int(int n);

int x;
char a;

void main(void) {
    x = 128;
    a = 'a';
    print_string("The value of x is: ");
    print_int(x);
    print_string("\n");
    print_string("The value of a is: ");
    print_int(a);
    print_string("\n");
}
