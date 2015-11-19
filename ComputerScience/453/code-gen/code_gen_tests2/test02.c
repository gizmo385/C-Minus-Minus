extern void print_string(char string[]);
extern void print_int(int n);

void main(void) {
    int x;
    x = 1;

    print_string("x = ");
    print_int(x);
    print_string("\n");

    if(x == 1) {
        print_string("Hello\n");
    }

    if(x != 1) {
        print_string("It is not 1!\n");
    } else {
        print_string("It is 1!\n");
    }
}
