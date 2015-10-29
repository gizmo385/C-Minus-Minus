/* RHS-type of assignment not compatible with LHS-type */

int x[10], y, z;

int foo(void)
{
  x[1] = "abcde";
  return y;
}
