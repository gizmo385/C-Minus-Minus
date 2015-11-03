/* call to void function in expression */

extern void p(int x);

int foo(int y)
{
  return y+p(y);
}
