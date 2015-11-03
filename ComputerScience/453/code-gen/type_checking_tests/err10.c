/* Invalid function arguments */
extern void f1(void);
extern int f2(int a);
extern int f3(int anInt, char aChar, char message[], int nums[]);

void main(void) {
    int anInt;
    char aChar;
    char message[7];
    int  nums[7];
    anInt = f2(f1());
    anInt = f3(f1(), aChar, message, nums);
    anInt = f3(anInt, f1(), message, nums);
    anInt = f3(anInt, aChar, f1(), nums);
    anInt = f3(anInt, aChar, message, f1());
}
