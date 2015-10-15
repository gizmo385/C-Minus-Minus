/* malformed conditional */

int foo(int x)
{
  if x > 0)
    x = 1;
  else
    x = 2;
}
