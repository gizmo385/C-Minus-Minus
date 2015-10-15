/* function definitions involving local arrays */

void foo0( void )
{
  int x[10];
  int y[10];
  int z[10];
  char a[10];
  char b[10];
}

void foo1( void )
{
  int x[10], y[20], z[30];
  char a[40], b[50];
}

int foo2( void )
{
  int x[10], y[20], z[1];
  char a[2], b[3000];
}

int foo3( int xx, int yy, char zz )
{
  int x[10], y[2], z[20];
  char a[30], b[40];
}

char foo4( int xx, int yy )
{
  int x[50], y[60], z[70];
  char a[80], b[90];
}

