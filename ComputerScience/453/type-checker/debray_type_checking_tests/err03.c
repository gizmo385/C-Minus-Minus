/* multiple prototypes for the same function */

int foo(int x);

char bar(int x)
{
  return x+1;
}

int foo(int y);

void f(void){ }

