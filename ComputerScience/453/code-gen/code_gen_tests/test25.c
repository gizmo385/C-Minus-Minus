/* a chain of procedure calls with argument passing */

extern void print_int(int x);
extern void print_string(char x[]);

int x;

void bar(char a)
{
  print_int(a);
  print_string("\n");
}

void foo2(int b)
{
  print_int(b);
  print_string("\n");

  bar(b);
}

void foo1(char c)
{
  print_int(c);
  print_string("\n");

  foo2(c);
}

void foo0(int d)
{
  print_int(d);
  print_string("\n");

  foo1(d);
}

void main(void)
{
  x = 65535;

  foo0(x);
}
