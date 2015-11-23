/* a chain of simple procedure calls */

extern void print_int(int x);
extern void print_string(char x[]);

int x;

void bar(void)
{
  x = 123;

  print_int(x);
  print_string("\n");
}

void foo2(void)
{
  bar();
}

void foo1(void)
{
  foo2();
}

void foo0(void)
{
  foo1();
}

void main(void)
{
  foo0();
}
