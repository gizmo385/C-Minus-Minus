/* several assignments to several global arrays */

int x[10], y[10], z[10];

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int u;

  x[4] = 111111;
  x[5] = 222222;
  x[6] = 333333;
  y[7] = 444444;
  y[8] = 555555;
  y[9] = 666666;
  u = x[4];
  print_int(u);
  print_string("\n");
  u = x[5];
  print_int(u);
  print_string("\n");
  u = x[6];
  print_int(u);
  print_string("\n");
  u = y[7];
  print_int(u);
  print_string("\n");
  u = y[8];
  print_int(u);
  print_string("\n");
  u = y[9];
  print_int(u);
  print_string("\n");
}
