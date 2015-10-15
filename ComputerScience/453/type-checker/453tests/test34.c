/* procedure calls involving complicated actuals */

/* functions with assignments involving more complex expressions */

int x, y, z[10];

extern int bar(int x, int y);

void foo( int a, int b, char c, int d[], char e[], char f, char g[])
{
  int l0, l1[10], l2, l3;
  
  foo(z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]);
  foo(bar(z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]], z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]], z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]], 1));
  foo(bar(bar(bar(bar(bar(bar(0)))))));
  foo(bar(bar(bar(bar(bar(bar(0+1*2+3/4+5-6*7/8+9*x)))))));
  foo(bar(bar(bar(bar(bar(bar(z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]])))))));
  foo(bar(bar(bar(bar(bar(bar(z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]+1-'\n')))))));
}
