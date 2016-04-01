/* more complex expressions: scalar variables only */

int x, y, z;

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  char a, b, c;

  x = 2;
  y = 3;
  z = 4;
  a = 'a'*x;
  b = a/(x+y)*z;
  c = a/(x+y)*z-a+b;
  print_int(c);
  print_string("\n");
}
