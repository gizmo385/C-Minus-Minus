/* illegal LHS-type in an assignment */

int x[10], y, z;

int foo(void)
{
  x = y+z;
  return y;
}
