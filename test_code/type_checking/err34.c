/* for loops */

int a[10];

char c[20];

extern int bar(int x);

int f(int x)
{
  for ( ; ; );
  for (x = 0; ; );
  for ( ; x < 100; );
  for ( ; ; x = x+1);
  for (x = 0; x < 100; );
  for (x = 0; x < 100; x = x+1);
  for (x = 0; x < 100; x = x+1)
    a[x] = a[x]+1;
  for (x = 0; x < 100 && x > 10; x = x+1)
    a[x] = a[x]+1;
  for (x = 0; !(x < 100); x = x+1)
    a[x] = a[x]+1;
  for (x = 0; !!!!!(x < 100); x = x+1)
    a[x] = a[x]+1;
  for (x = 0; (x < 100 && x == 0 || x > 10 && !x < 5); x = x+1)
    a[x] = a[x]+1;
  for (x = 0; bar(x) < 100; x = x+1)
    a[x] = a[x]+1;
  for (x = bar(a[bar(a[bar(c[0])+1]-1)+2]-2); bar(bar(bar(x))) < 100; x = x+1)
    a[x] = a[x]+1;
  for (x = bar(a[bar(a[bar(c[0])+1]-1)+2]-2); bar(bar(bar(x))) < 100; x = bar(a[bar(a[bar(c[0])+1]-1)+2]-2)+1)
    a[x] = a[x]+1;
  for (a[0] = 0; a[0] < 100; a[0] = a[0]+1)
    for (a[1] = 1; a[1] < 111; a[1] = a[1]+1)
      for (a[2] = 2; a[2] < 222; a[2] = a[2]+2)
	for (a[3] = 3; a[3] < 333; a[3] = a[3]+3)
	  for (a[4] = 4; a[4] < 444; a[4] = a[4]+4)
	    for (a[5] = 5; a[5] < 555; a[5] = a[5]+5)
	      for (a[6] = 6; a[6] < 666; a[6] = a[6]+6)
		;
  for (c[x] = 1; c[x] < x; c[x] = c[c[x]]+1)
    c[x] = c[x]+1;
  return x;
}
