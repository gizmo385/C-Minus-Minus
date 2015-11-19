extern void print_string(char string[]);
extern void print_int(int n);

void number_on_line(int shouldBe, int result) {
    print_string("Result should be ");
    print_int(shouldBe);
    print_string(", and is: ");
    print_int(result);
    print_string("\n");
}

void main(void) {
    number_on_line(12 , 5 + 7);
    number_on_line(25, 5 * 5);
    number_on_line(2, 7 - 5);
    number_on_line(5, 25 / 5);
    number_on_line(-5, -(25 / 5));
}
