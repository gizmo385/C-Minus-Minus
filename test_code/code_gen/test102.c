/* function calls: int --> char return value conversion */

int x;

char f(int x)
{
  return x+12345;
}

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  x = 12345;
  x = f(x);
  x = x+f(x);
  print_int(x);
  print_string("\n");
}
