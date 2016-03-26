/* array subscript not of appropriate type */

int a[10];

int foo(int x)
{
  x = a[foo];
  return x;
}
