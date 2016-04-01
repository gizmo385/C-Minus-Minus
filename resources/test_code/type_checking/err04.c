/* multiple definitions of a function */

int foo(int x);

int foo(int x)
{
  return x+1;
}

char x[10];

char foo(int x)
{
  return x+1;
}

