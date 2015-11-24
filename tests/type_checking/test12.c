/* more complex expressions */

int x, y;

void bar(int x)
{
  return;
}

int foo(int z)
{
  char u[10];

  x = x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+z;
  u[x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+z] = 1;
    u[x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+z] = u[x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+x+y*z/2+z] +1;
  u[u[u[u[u[u[u[u[u[u[0]]]]]]]]]] = 0;
  u[u[u[u[u[u[u[u[u[u[0]]]]]]]]]] = u[u[u[u[u[u[u[u[u[u[0]]]]]]]]]]+u[u[u[u[u[u[u[u[u[u[0]]]]]]]]]]*u[u[u[u[u[u[u[u[u[u[0]]]]]]]]]]/u[u[u[u[u[u[u[u[u[u[0]]]]]]]]]]+u[u[u[u[u[u[u[u[u[u[0]]]]]]]]]]*2;
  x = foo(foo(foo(foo(foo(foo(foo(foo(foo(foo(0))))))))));
  u[foo(u[foo(u[foo(u[foo(u[foo(u[foo(u[foo(x)])])])])])])] = u[foo(u[foo(u[foo(u[foo(u[foo(u[foo(u[foo(x)])])])])])])]+1;
  return 0;
}
