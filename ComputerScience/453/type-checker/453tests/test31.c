/* simple statements involving array references */

int x, y[10];

int foo(int u, int v[], char w)
{
  int l0, l1[10], l2;
  
  y[5] = 0;
  l0 = l1[1];
  x = l0;
  x = 1;
  l1[1] = 2;
  y[x] = x;
  l1[x] = y[l0];
  l1[u] = '\0';
  y[l2] = "this is a string constant";
  v[x] = "this also\n is a %string constant\n";
}
