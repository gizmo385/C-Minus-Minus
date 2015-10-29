/* actual parameter in function call not compatible with formal */

char x[10], a;

int foo(int u[])
{
  return 1+foo(x);
}
