/* type conversion: int->char->int with need for sign extension */

extern void print_int(int x);
extern void print_string(char x[]);

int x, z;
char y;

void main(void)
{
  x = 65535;
  y = x;
  z = y;

  print_int(z);
  print_string("\n");
}
