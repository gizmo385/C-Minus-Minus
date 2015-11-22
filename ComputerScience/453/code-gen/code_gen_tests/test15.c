/* simple procedure call */

extern void print_int(int x);
extern void print_string(char x[]);

int x;

void foo(void)
{
  x = 123;

  print_int(x);
  print_string("\n");
}

void main(void)
{
  foo();
}
