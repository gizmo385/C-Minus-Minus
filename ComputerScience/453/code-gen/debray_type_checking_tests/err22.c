/* actual parameter in function call not compatible with formal */

int x[10], a;

int foo(int u, char v, int w)
{
  return 1+foo(x, a, w-1);  /* first argument */
}
