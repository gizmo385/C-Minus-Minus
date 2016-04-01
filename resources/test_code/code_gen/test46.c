/* assignments to several local scalar variables */

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int x, y, z, u, v;

  x = 111;
  y = 222;
  z = 333;
  u = 444;
  v = 555;
  print_int(x);
  print_string("\n");
  print_int(y);
  print_string("\n");
  print_int(z);
  print_string("\n");
  print_int(u);
  print_string("\n");
  print_int(v);
  print_string("\n");
}
