/* Calling a void function in an expression */
extern void f1(void);

void main(void) {
    int anInt;
    anInt = anInt + f1();
}
