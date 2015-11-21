extern void print_string(char string[]);

int printAndReturn4(void) {
    print_string("EXECUTED printAndReturn4!\n");
    return 4;
}

void main(void) {
    print_string("Should NOT print:\n");
    if((1 == 1) || (4 == printAndReturn4())) { }
    print_string("---------\n\n");

    print_string("Should print:\n");
    if((1 != 1) || (4 == printAndReturn4())) { }
    print_string("---------\n\n");

    print_string("Should NOT print:\n");
    if((1 != 1) && (4 == printAndReturn4())) { }
    print_string("---------\n\n");

    print_string("Should print:\n");
    if((1 == 1) && (4 == printAndReturn4())) { }
    print_string("---------\n\n");
}
