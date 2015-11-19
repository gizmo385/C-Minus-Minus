extern void print_string(char string[]);

void main(void) {
    int x;
    x = 1;

    if(x == 1) {
        print_string("Hello");
    }

    if(x != 1) {
        print_string("It is not 1!");
    } else {
        print_string("It is 1!");
    }
}
