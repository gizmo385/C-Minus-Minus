/* enforcing alignment restrictions */

extern void print_int(int x);
extern void print_string(char x[]);

int a;
char b;
int c;
char d;
int e;

void foo(void)
{
  print_int(a);
  print_string("\n");

  print_int(b);
  print_string("\n");

  print_int(c);
  print_string("\n");

  print_int(d);
  print_string("\n");

  print_int(e);
  print_string("\n");
}

void main(void)
{
  a = 10;
  b = 11;
  c = 12;
  d = 13;
  e = 14;

  foo();
}
