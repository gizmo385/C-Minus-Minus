/* multiple declarations of a global variable */

int x;

char foo(int y)
{
  return y;
}

char x[10];  /* x already declared as global */

int bar(int y)
{
  return y+1;
}
