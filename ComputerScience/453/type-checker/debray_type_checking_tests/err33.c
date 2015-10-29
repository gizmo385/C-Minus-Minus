/* multiple semantic errors: enough recovery 
   ought to take place that several get caught. */


int foo_0(int z[])  /* multiple definition of foo_0 */
{
  z[0] = 1;
  return 1;
}

int foo_2( int x1 ) { }

int foo_3( int x1 ) { }

int foo_4( int x1 )
{
  foo_4 (x1-1);
  x1 = foo_4;  /* illegal RHS type */
  return x2;   /* undeclared variable */


}

int x0, y0;

void bar_0(int x0)
{
  return;
}

char foo_0(int z)
{
  char u[10];

  x0 = x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+z;
  u[x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+z] = 1;
    u[x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+z] = u[x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+x0+y0*z/2+z] +1;
  u[u[u[u[u[u[u[u[u[u[0]]]]]]]]]] = 0;
  u[u[u[u[u[u[u[u[u[u[0]]]]]]]]]] = u[u[u[u[u[u[u[u[u[u[0]]]]]]]]]]+u[u[u[u[u[u[u[u[u[u[0]]]]]]]]]]*u[u[u[u[u[u[u[u[u[u[0]]]]]]]]]]/u[u[u[u[u[u[u[u[u[u[0]]]]]]]]]]+u[u[u[u[u[u[u[u[u[u[0]]]]]]]]]]*2;
  x0 = foo_0(foo_0(foo_0(foo_0(foo_0(foo_0(foo_0(foo_0(foo_0(foo_0(0))))))))));
  u[foo_0(u[foo_0(u[foo_0(u[foo_0(u[foo_0(u[foo_0(u[foo_0(x0)])])])])])])] = u[foo_0(u[foo_0(u[foo_0(u[foo_0(u[foo_0(u[foo_0(u[foo_0(x0)])])])])])])]+1;
  return 0;
}

int f(void ) { }

/*-----------------------------------------------------------------*/

int x1[10];

int bar_1(int x1)
{
  return x1+1;
}

int a, b, c[10];

void foo_1(int x1)
{
  a = x1+1;
  b = a+1;
  foo_1(x1-1);
  c[a] = 0;
  foo_1(c[c[c[c[c[c[c[c[c[c[0]]]]]]]]]]);
  foo_1(bar_1(bar_1(bar_1(bar_1(bar_1(0))))));
  foo_1(c[bar_1(c[bar_1(c[bar_1(c[bar_1(0)+1]-1)+1]-1)+1]-1)+1]-1);
  return;
}

void f(int f)
{
  f("abcdefg");  /* f not of type function */
}

/*-----------------------------------------------------------------*/
