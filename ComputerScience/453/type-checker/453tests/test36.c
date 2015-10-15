/* simple conditionals */

int x, y, z[10];

int foo0( void )
{
  if (1 > 0) 
    x = 1;
}

int foo1( void )
{
  if (1 >= 0) 
    x = 1;
  else 
    x = 2;
}

int foo2( int u )
{
  if (u < 0) 
    x = 1;
  else 
    x = 2;
}

int foo3( int u )
{
  if (u <= x) 
    x = 1;
  else 
    x = 2;
}


int foo5( int u )
{
  if (u != x) 
    x = 1;
  else 
    x = 2;
}

int foo6( int u )
{
  if (!(u != x))
    x = 1;
  else 
    x = 2;
}
