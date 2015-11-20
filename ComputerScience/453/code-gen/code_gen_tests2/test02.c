extern void print_string(char string[]);
extern void print_int(int n);

void main(void) {
    print_string("Testing the == operator\n");

    /* Checking == */
    if(1 == 1) {
        print_string("1 == 1 --> CORRECT\n");
    } else {
        print_string("1 == 1 --> INCORRECT\n");
    }

    if(2 == 1) {
        print_string("2 == 1 --> INCORRECT\n");
    } else {
        print_string("2 == 1 --> CORRECT\n");
    }

    print_string("Testing the != operator\n");

    /* Checking != */
    if(1 != 1) {
        print_string("1 != 1 --> INCORRECT\n");
    } else {
        print_string("1 != 1 --> CORRECT\n");
    }

    if(2 != 1) {
        print_string("2 != 1 --> CORRECT\n");
    } else {
        print_string("2 != 1 --> INCORRECT\n");
    }

    print_string("Testing the >= operator\n");

    /* Checking >= */
    if(3 >= 2) {
        print_string("3 >= 2 --> CORRECT\n");
    } else {
        print_string("3 >= 2 --> INCORRECT\n");
    }

    if(3 >= 3) {
        print_string("3 >= 3 --> CORRECT\n");
    } else {
        print_string("3 >= 3 --> INCORRECT\n");
    }

    if(2 >= 3) {
        print_string("2 >= 3 --> INCORRECT\n");
    } else {
        print_string("2 >= 3 --> CORRECT\n");
    }

    print_string("Testing the > operator\n");

    /* Checking > */
    if(3 > 2) {
        print_string("3 > 2 --> CORRECT\n");
    } else {
        print_string("3 > 2 --> INCORRECT\n");
    }

    if(3 > 3) {
        print_string("3 > 3 --> INCORRECT\n");
    } else {
        print_string("3 > 3 --> CORRECT\n");
    }

    if(2 > 3) {
        print_string("2 > 3 --> INCORRECT\n");
    } else {
        print_string("2 > 3 --> CORRECT\n");
    }

    print_string("Testing the <= operator\n");

    /* Checking >= */
    if(3 <= 2) {
        print_string("3 <= 2 --> INCORRECT\n");
    } else {
        print_string("3 <= 2 --> CORRECT\n");
    }

    if(3 <= 3) {
        print_string("3 <= 3 --> CORRECT\n");
    } else {
        print_string("3 <= 3 --> INCORRECT\n");
    }

    if(2 <= 3) {
        print_string("2 <= 3 --> CORRECT\n");
    } else {
        print_string("2 <= 3 --> INCORRECT\n");
    }

    print_string("Testing the < operator\n");

    /* Checking >= */
    if(3 < 2) {
        print_string("3 < 2 --> INCORRECT\n");
    } else {
        print_string("3 < 2 --> CORRECT\n");
    }

    if(3 < 3) {
        print_string("3 < 3 --> INCORRECT\n");
    } else {
        print_string("3 < 3 --> CORRECT\n");
    }

    if(2 < 3) {
        print_string("2 < 3 --> CORRECT\n");
    } else {
        print_string("2 < 3 --> INCORRECT\n");
    }

    print_string("Testing the ! (NOT) operator\n");

    /* Checking ! */
    if(! (1 == 1)) {
        print_string("!(1 == 1) --> INCORRECT\n");
    } else {
        print_string("!(1 == 1) --> CORRECT\n");
    }

    if(! (1 != 1)) {
        print_string("!(1 != 1) --> CORRECT\n");
    } else {
        print_string("!(1 != 1) --> INCORRECT\n");
    }
}
