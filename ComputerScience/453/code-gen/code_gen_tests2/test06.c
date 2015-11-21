extern void print_string(char string[]);
extern void print_int(int n);

/*void print_hello(void) {*/
    /*char hello[7];*/
    /*hello[0] = 'h';*/
    /*hello[1] = 'e';*/
    /*hello[2] = 'l';*/
    /*hello[3] = 'l';*/
    /*hello[4] = 'o';*/
    /*hello[5] = '\n';*/
    /*hello[6] = '\0';*/

    /*print_string(hello);*/
/*}*/

/*void print_world(void) {*/
    /*char world[7];*/
    /*world[0] = 'w';*/
    /*world[1] = 'o';*/
    /*world[2] = 'r';*/
    /*world[3] = 'l';*/
    /*world[4] = 'd';*/
    /*world[5] = '\n';*/
    /*world[6] = '\0';*/

    /*print_string(world);*/
/*}*/

/*void print_numbers(int nums[], int count) {*/
    /*int i;*/
    /*for(i = 0; i < count; i = i + 1) {*/
        /*print_string("The number at index ");*/
        /*print_int(i);*/
        /*print_string(" is: ");*/
        /*print_int(nums[i]);*/
        /*print_string("\n");*/
    /*}*/
/*}*/

void main(void) {
    int nums[10], i;
    /*print_hello();*/
    /*print_world();*/

    for(i = 0; i < 10; i = i + 1) {
        nums[i] = i;
    }

    for(i = 0; i < 10; i = i + 1) {
        /*print_string("nums[");*/
        /*print_int(i);*/
        /*print_string("] = ");*/
        print_int(nums[i]);
        print_string("\n");
    }
}
