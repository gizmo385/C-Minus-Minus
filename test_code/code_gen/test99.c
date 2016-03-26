/* function calls: char --> int parameter conversion, globals */

int x;
char y;

char f(int x)
{
  return x+1;
}

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  y = 1;
  x = f(y);
  y = x+f(x);
  x = y+f('Q');
  print_int(x);
  print_string("\n");
}
