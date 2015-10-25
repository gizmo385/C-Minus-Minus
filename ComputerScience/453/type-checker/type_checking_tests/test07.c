/* Multiple prototypes per line */
extern int addOne(int a), addTwo(int a), addThree(int a);
int addFour(int a), addFive(int a), addSix(int a);

int addFour(int a) { return a + 4; }
int addFive(int a) { return a + 5; }
int addSix(int a) { return a + 6; }

void main(void) {
    addOne(1);
    addTwo(1);
    addThree(1);
}
