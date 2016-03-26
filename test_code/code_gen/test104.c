/* function calls: side effects on globals */

int x;

int f(int i)
{
  int y;

  y = 1;
  while (i > 0) {
    x = x*i;
    y = 2*y;
    i = i-1;
  }
  return y;
}

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int a;

  x = 1;
  a = f(6);
  x = a+x;
  print_int(x);
  print_string("\n");
}
