/* simple statements */

void foo0( void )
{
  int x;
  
  x = 1;
}

void foo1( int x )
{  
  x = 1;
}

int foo2( void )
{
  int x;
  
  x = 1;
}

int global1, global2, global3[10];


int foo3( int x )
{  
  x = 1;
  global2 = x;
  x = global1;
}

char foo4( int x, int y, int w, char u, int v )
{
  x = 1;
  y = 1;
  z = 'x';
  u = z;
  u = '\n';
  v = u;
}

