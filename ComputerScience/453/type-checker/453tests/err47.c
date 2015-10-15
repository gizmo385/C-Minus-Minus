/* error recovery [5 points] : this file contains many errors, and
   at least some (> 1) of them should be caught */

f( ) { }; /* missing keyword in function formals */

/*-----------------------------------------------------------------*/

int x[10];

void a( void ), b(int x), c(int x, int y, int z);

int a1( void ), b1(int x), c1(int x, char y, char z, int w);

char a2( void ), b2(char x), c2(char x, char y, char z, int w);

int foo(int z[])
{
  z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]]]]]]] = 1;
                          /* unbalanced brackets in array references */
}

/*-----------------------------------------------------------------*/

void foo0( int u[] ) { }

void foo1( char v[] ) {}

int foo2( int w[] ) {}

int foo1( int x ) { /* missing "}" in function body */

/*-----------------------------------------------------------------*/

int foo2( int x ) { }

int foo3( int x ) { }

int foo( int x )
{
  foo (x-1) /* missing semicolon after procedure call */
  x = x+1;
  x = x-1;
  x = x*1;
  x = x/1;
  {
  {
    {
      {
	{
	  {
	    x = x+1;
	  }
	}
      }
   /* unbalanced braces */
}

/*-----------------------------------------------------------------*/

int foo( int x, int y, char z)
{
  return;
}

int foo0(int x)
{
  return x;
}

int foo( int x)
{
  if (x && x > 0) /* malformed boolean */
    ;
}

/*-----------------------------------------------------------------*/
