/* Global char array test */
extern void print_string(char string[]);

char message[13];

void main(void) {
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
    message[11] = '\n';
    message[12] = '\0';

    print_string(message);
}
