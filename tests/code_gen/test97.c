/* function calls: multiple parameters */

int f(int a, int b, int c, int d, int e)
{
  return a+b*(c+d*e);
}

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int x;

  x = f(1,2,3,4,5);
  x = f(x,x,x,x,x);
  print_int(x);
  print_string("\n");
}
