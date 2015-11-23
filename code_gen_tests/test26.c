/* a procedure call with assignment to the parameter variable */

extern void print_int(int x);
extern void print_string(char x[]);

int x;

void foo(int y)
{
  print_int(y);
  print_string("\n");

  y = 456;

  print_int(y);       /* the value of y should change */
  print_string("\n");

  print_int(x);       /* the value of x should not */
  print_string("\n");
}

void main(void)
{
  x = 123;

  foo(x);
}
