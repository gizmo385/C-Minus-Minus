/* function calls */

int foo(void)
{
  int x;
  
  x = foo();
  return x;
}

void bar(void)
{
  bar();
}
