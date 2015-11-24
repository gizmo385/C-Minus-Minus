/* Test nested loops */
extern void print_string(char string[]);
extern void print_int(int n);

void main(void) {
    int i, j;
    for(i = 0; i < 5; i = i + 1) {
        for(j = 0; j < 5; j = j + 1) {
            print_int(j * i);
            print_string("\t");
        }
        print_string("\n");
    }
}
