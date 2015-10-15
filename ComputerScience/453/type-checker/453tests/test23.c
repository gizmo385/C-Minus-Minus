/* function definitions involving local scalars and arrays */

void foo0( void )
{
  int x;
  int y[10];
  int z[2];
  char a;
  char b;
}

void foo1( void )
{
  int x, y[10], z[10], u, v, w[10];
  char a, b[100], c, d, e, f[100], g[10];
}

int foo2( void )
{
  int x, y, z;
  int u, v[10], w[100];
  char a, b[5];
}

int foo3( int xx, int yy, char zz )
{
  int x, y, z[10];
  char a, b[10], c[100];
}

char foo4( int xx, int yy )
{
  int x[10], y, z;
  char a[10], b, c;
}

