/* while loops */

int a[10];

char c[20];

int bar(int x);

int f(int x)
{
  while (x == 0)
    ;
  while (x == 0)
    x = x+1;
  while (x < 100 && x > 10)
    a[x] = a[x]+1;
  while (!(x < 100))
    a[x] = a[x]+1;
  while (!!!!!(x < 100))
    a[x] = a[x]+1;
  while ((x < 100 && x == 0 || x > 10 && !x < 5))
    a[x] = a[x]+1;
  while (bar(x) < 100)
    a[x] = a[x]+1;
  while (bar(a[bar(a[bar(c[0])+1]-1)+2]-2) <= bar(bar(bar(x))))
    a[x] = a[x]+1;
  while (a[0] < 100)
    while (a[1] < 111)
      while (a[2] < 222)
	while (a[3] < 333)
	  while (a[4] < 444)
	    while (a[5] < 555)
	      while (a[6] < 666)
		;
  while (c[x] < c[c[x]]+1)
    c[x] = c[x]+1;
  return a[c[x]];
}
