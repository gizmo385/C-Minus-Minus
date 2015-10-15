/* simple for loops */

int a[10];

int foo(int x)
{
  for ( ; ; )
    ;
  for ( ; ; )
    x = 1;
  for (x = 0; ; )
    x = 1;
  for ( ; x < 0; )
    x = 1;
  for ( ; ; x = 1)
    x = 1;
  for (x = 0; x < 10; )
    x = 1;
  for (x = 0; ; x = 1)
    x = 2;
  for ( ; x < 0; x = 1)
    x = 2;
  for (x = 0; x < 10; x = x+1)
    a[x] = 1;
  for (x = 0; x > 0 && !(x <= y && !(y > 1 || !(u == x))) || (x <= y && !(y > 1 || !(u == x))); x = x+1)
    a[0] = 2;
  for (a[a[a[a[a[a[a[a[a[a[x]]]]]]]]]] = 0; a[a[a[a[a[a[a[a[a[a[x+1]]]]]]]]]] > 0; a[a[a[a[a[a[a[a[a[a[x]]]]]]]]]] = a[a[a[a[a[a[a[a[a[a[x-1]]]]]]]]]]+1)
    ;
}
