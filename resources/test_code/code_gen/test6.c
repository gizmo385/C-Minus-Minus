/* assignment of global scalars of type char */

extern void print_int(int x);
extern void print_string(char x[]);

char x, y, z;

void main(void)
{
  x = 'a';
  y = x;
  z = y;

  print_int(z);
  print_string("\n");
}
