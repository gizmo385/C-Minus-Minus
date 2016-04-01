/* procedure calls */

int a, b, c[10];

int bar(int x)
{
  return x+1;
}

void foo(int x)
{
  a = x+1;
  b = a+1;
  foo(x-1);
  c[a] = 0;
  foo(c[c[c[c[c[c[c[c[c[c[0]]]]]]]]]]);
  foo(bar(bar(bar(bar(bar(0))))));
  foo(c[bar(c[bar(c[bar(c[bar(0)+1]-1)+1]-1)+1]-1)+1]-1);
  return;
}

void f(char x[])
{
  f("abcdefg");
}
