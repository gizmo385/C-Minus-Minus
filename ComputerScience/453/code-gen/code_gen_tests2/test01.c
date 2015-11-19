extern void print_string(char string[]);
extern void print_int(int n);

void print_result(int shouldBe, int result) {
    print_string("Result should be ");
    print_int(shouldBe);
    print_string(", and is: ");
    print_int(result);
    print_string("\n");
}

void main(void) {
    print_result(12 , 5 + 7);
    print_result(25, 5 * 5);
    print_result(2, 7 - 5);
    print_result(5, 25 / 5);
    print_result(-5, -(25 / 5));
}
