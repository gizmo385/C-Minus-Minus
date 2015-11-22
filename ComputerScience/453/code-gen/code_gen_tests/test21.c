/* a procedure call with int->char type conversion (var argument) */

extern void print_int(int x);
extern void print_string(char x[]);

int x;

void foo(char y)
{
  print_int(y);
  print_string("\n");
}

void main(void)
{
  x = 12345;
  foo(x);
}
