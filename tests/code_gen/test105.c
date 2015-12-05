/* function calls: int array parameter */

int x[10];

int init(int x[], int n)
{
  int i;

  for (i = 0; i < n; i = i+1)
    x[i] = i;
  return n;
}

int count(int x[], int n)
{
  int c;

  c = 0;
  n = n-1;
  while (n >= 0) {
    c = c+x[n];
    n = n-1;
  }
  return c;
}

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int z;

  z = init(x, 10);
  z = z+count(x, 10);
  print_int(z);
  print_string("\n");
}
