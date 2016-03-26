/* function calls: int --> char parameter conversion */

int x;

char f(char x)
{
  return x+1;
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
