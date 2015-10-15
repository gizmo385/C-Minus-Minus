/* repeated simple conditionals */

int x, y, z[10];

int foo0( void )
{
  if (1 > 0) 
    x = 1;
  if (1 >= 0) 
    x = 1;
  else 
    x = 2;
  if (u < 0) 
    x = 1;
  else 
    x = 2;
  if (u <= x) 
    x = 1;
  else 
    x = 2;
  if (u != x) 
    x = 1;
  else 
    x = 2;
  if (!(u != x))
    x = 1;
  else 
    x = 2;
}
