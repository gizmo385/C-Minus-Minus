/* function prototype doesn't match definition: types of args */

int f(int x, int y, int z);

int f(int x, char y, int z)
{
  return x+1;
}

int bar(int x, int y);

int bar(int x[], int y)
{
  return y+x[y];
}
