/* type conversion: char->int->char */

extern void print_int(int x);
extern void print_string(char x[]);

int x;
char y, z;

void main(void)
{
  y = '\n';
  x = y;
  z = x;

  print_int(x);
  print_string("\n");
  print_int(z);
  print_string("\n");
}
