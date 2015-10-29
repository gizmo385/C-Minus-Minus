/* illegal LHS-type in an assignment */

int x[10], y, z;

int foo(void)
{
  foo = y+z;
  return y;
}
