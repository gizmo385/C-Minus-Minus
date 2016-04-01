/* arithmetic expressions not compatible with int */

int x, y[10];

int foo(int a)
{
  a = -y + 1;
  return a;
}
