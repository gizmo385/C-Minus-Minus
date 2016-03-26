/* mixed global and local declarations */

int x, y, z[10];

int f(int x)
{
  int a, b[10];
  char c[100];
  
  return x;
}

int g(char u)
{
  int v, w[10];
  char d[100];
  
  return v;
}

