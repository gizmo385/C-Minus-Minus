/* Calling functions */
extern void printf(char string[]);
int addAndPrint(int first, int second, int third, char message[]);

int addAndPrint(int first, int second, int third, char message[]) {
    int result;
    result = first + second + third;
    printf(message);

    return result;
}

int main(void) {
    printf("hello");
    addAndPrint(1, 2, 3, "Hello");
}
