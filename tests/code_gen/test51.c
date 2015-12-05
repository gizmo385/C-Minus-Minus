/* assignments to several global chars */

char x, y, z;

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  x = 'A';
  y = 'B';
  z = 'C';
  print_int(x);
  print_string("\n");
  print_int(y);
  print_string("\n");
  print_int(z);
  print_string("\n");
}
