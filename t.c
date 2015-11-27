struct Person {
    char name[20];
    int age;
};

struct Empty {};

struct BoxedInteger {
    int a;
};

struct Point {
    int x, y;
};

void main(void) {
    struct Point p;
    p.x = 5;
}
