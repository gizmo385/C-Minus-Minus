/* return statements */

int x, y, z[10];

extern int bar(int x, int y);

int foo( int x, int y, char z)
{
  return;
}

int foo0(int x)
{
  return x;
}

int foo1(int x, int y)
{
  return x+y;
}

int foo2(int x, char y, int z)
{
  return x+x*x+x*x+x*x+x*x+x*x+x*x+x*x+x*x+x*x+x*x+x*x+x*x+x*x+x*2;
}

char foo( int a, int b, char c, int d[], int e[], char f, char g[])
{
  int l0, l1[10], l2, l3;
  
  return foo(z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]);
  return foo(bar(z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]], z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]], z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]], 1));
  return foo(bar(bar(bar(bar(bar(bar(0)))))));
  return foo(bar(bar(bar(bar(bar(bar(0+1*2+3/4+5-6*7/8+9*x)))))));
  return foo(bar(bar(bar(bar(bar(bar(z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]])))))));
  return foo(bar(bar(bar(bar(bar(bar(z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]+1-'\n')))))));
}
