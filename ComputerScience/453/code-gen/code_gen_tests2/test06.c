extern void print_string(char string[]);
extern void print_int(int n);

void print_ascii_table(void) {
    char start;
    char string[2];
    string[1] = '\0';

    for(start = 'A'; start <= 'z'; start = start + 1) {
        string[0] = start;
        print_string(string);
        print_string(" <--> ");
        print_int(string[0]);
        print_string("\n");
    }
}

void print_hello_world(void) {
    char message[14];
    message[0] = 'H';
    message[1] = 'e';
    message[2] = 'l';
    message[3] = 'l';
    message[4] = 'o';
    message[5] = ' ';
    message[6] = 'W';
    message[7] = 'o';
    message[8] = 'r';
    message[9] = 'l';
    message[10] = 'd';
    message[11] = '!';
    message[12] = '\n';
    message[13] = '\0';

    print_string(message);
}

void main(void) {
    print_hello_world();
    print_ascii_table();
}
