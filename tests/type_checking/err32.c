/* procedure call: compatibility of actuals and formals */

void foo(int x, char y[])
{
  foo(y, x);
}
