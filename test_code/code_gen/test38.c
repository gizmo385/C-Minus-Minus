/* Int array test */
extern void print_int(int n);
extern void print_string(char string[]);

void main(void) {
    int array[10];
    int i;

    for(i = 0; i < 10; i = i + 1) {
        array[i] = i;
    }

    for(i = 0; i < 10; i = i + 1) {
        print_int(array[i]);
        print_string("\n");
    }
}
