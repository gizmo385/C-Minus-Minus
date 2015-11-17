/* a procedure call with an int argument; no type conversion */

extern void print_int(int x);
extern void print_string(char x[]);

int x;

void foo(int y)
{
  print_int(y);
  print_string("\n");
}

void main(void)
{
  x = 123;

  foo(x);
}
