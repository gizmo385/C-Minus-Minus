extern void print_string(char string[]);

int thisShouldNotExecute(void) {
    print_string("THIS SHOULD NOT PRINT!\n");
    return 4;
}

int shouldExecute(void) {
    return 4;
}

void main(void) {
    if((1 == 1) || (4 == thisShouldNotExecute())) { }
    if((1 != 1) || (4 == shouldExecute())) { }
    if((1 != 1) && (4 == thisShouldNotExecute())) { }
    if((1 == 1) && (4 == shouldExecute())) { }
}
