extern void print_int(int n);
extern void print_string(char message[]);

struct Point {
    int x, y;
};

struct Point p;

void main(void) {
    p.x = 5;
    p.y = 7;
    print_int(p.x);
    print_string(", ");
    print_int(p.y);
    print_string("\n");
}
