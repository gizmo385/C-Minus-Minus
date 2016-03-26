/* variables: scope distinctions for the same name */

int x, y, z[10];

int f(int x)
{
  int y, z[10];
  char f[100];
  
  return x;
}

int g(char g)
{
  int y, z[10];
  char f[100];
  
  return g;
}

