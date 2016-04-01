/* parameter conversion: multiple parameters */

int f(char x, char y, char z)
{
  return x+y+z;
}

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int x;

  x = f(111, 222, 000);
  print_int(x);
  print_string("\n");
}
