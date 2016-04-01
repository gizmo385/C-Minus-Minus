/* global arrays and functions */

int x[10];

int id(int x)
{
  return x;
}

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int i;
  int res;

  for (i = 0; i < 10; i = i+1)
    x[i] = i;

  res = x[id(x[id(x[id(x[id(5)])])])];
  x[id(x[id(x[id(x[id(0)+1]+1)+1]+1)+1]+1)+1] = res+1;
  res = x[id(x[id(x[id(x[id(0)+1]+1)+1]+1)+1]+1)+1];
  print_int(res);
  print_string("\n");
}
