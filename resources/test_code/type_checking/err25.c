/* arithmetic expressions not compatible with int */

int x, y[10];

int foo(int a)
{
  a = "hi\n" + 1;
  return a;
}
