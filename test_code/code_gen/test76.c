/* simple arithmetic expressions: scalar variables only */

int x;
char y;
int z;
char u;

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  x = 345;
  y = 10;
  x = x-y;
  y = -2;
  x = x+y;
  z = x/y;
  u = z*y;
  print_int(x);
  print_string("\n");
  print_int(y);
  print_string("\n");
  print_int(z);
  print_string("\n");
  print_int(u);
  print_string("\n");
}
