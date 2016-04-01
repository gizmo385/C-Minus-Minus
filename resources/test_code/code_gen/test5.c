/* assignment of global scalars of type int */

extern void print_int(int x);
extern void print_string(char x[]);

int x, y, z;

void main(void)
{
  x = 123;
  y = x;
  z = y;

  print_int(z);
  print_string("\n");
}
