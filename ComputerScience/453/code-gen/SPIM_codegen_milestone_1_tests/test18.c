/* a procedure call with a char argument; no type conversion */

extern void print_int(int x);
extern void print_string(char x[]);

char x;

void foo(char y)
{
  print_int(y);
  print_string("\n");
}

void main(void)
{
  x = 'a';

  foo(x);
}
