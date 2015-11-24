/* simple expressions */

int x, y;

void bar(int x)
{
  return;
}

int foo(int z)
{
  char u[10];

  x = 0;
  x = y;
  y = x+1;
  u[1] = y;
  z = u[0];
  u[x] = x + 'a';
  y = u[y];
  z = foo(y*x);
  x = foo(u[x]);
  bar(u[0]);
  bar(u[x]);
  bar(u['b']);
  bar(u[x+y]);
  return foo(z);
}
