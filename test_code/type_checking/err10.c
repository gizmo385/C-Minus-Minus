/* multiple occurrences of a formal parameter */

int foo(int x, char y[], int x)
{
  return x+y[x];
}
