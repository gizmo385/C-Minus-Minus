/* more complex boolean expressions */

int x, y;

char foo(int z)
{
  if (z > 0 && x > 0 && y > 0)
    x = 1;
  if (z == 0 || x == 0 || y == 0)
    y = 1;
  if (!(x > y && y > z && z > x))
    z = 0;
  if (x > y && y > z || z > y && y > x || z == x && x == y)
    z = 1;
  if (!!!!!!!!!!(x > y && y > z || z > y && y > x || z == x && x == y))
    return z;
  if (!(foo(x) > foo(y) &&
	foo(z) > foo(x) ||
	foo(foo(foo(x))) == foo(foo(foo(foo(y)))) ||
	foo(foo(z)) != foo(foo(foo(x)))
	)
      )
    return x;
}
