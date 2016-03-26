/* simple procedure call */

extern void print_int(int x);
extern void print_string(char x[]);

int x;

void foo(void)
{
}

void main(void)
{
  x = 123;

  foo();

  print_int(x);
  print_string("\n");
}
