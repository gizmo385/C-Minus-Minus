/* boolean expressions */

int x, y;

char foo(int z)
{
  if (z > 0)
    x = 1;
  if (z == 0)
    y = 1;
  if (!(x > y))
    z = 0;
  return z;
}
