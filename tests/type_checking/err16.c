/* call to non-void function in statement */

extern int p(int x);

int foo(int y)
{
  p(y);
  return y+1;
}
