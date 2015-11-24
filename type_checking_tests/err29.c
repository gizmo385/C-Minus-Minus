/* RHS-type of assignment not compatible with LHS-type */

int x[10], y, z;

int foo(int x[])
{
  y = x+1;
  return y;
}
