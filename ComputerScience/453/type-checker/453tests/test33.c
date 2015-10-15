/* procedure calls */

int x, y, z[10];

void foo0( void )
{
  bar(x);
  foo(x, y);
  bar(z[0]);
}

void foo1( int x, int y )
{
  bar(x);
  foo(x, y);
  bar(z[0]);
}

int foo2( int x, int y )
{
  int u, v, w;

  bar(w);
  foo(u, y);
  bar(z[0]);
}

