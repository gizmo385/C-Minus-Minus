/* several assignments to several local arrays */

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int x[10], y[10];
  int u;

  x[4] = 1111;
  x[5] = 2222;
  x[6] = 3333;
  y[7] = 4444;
  y[8] = 5555;
  y[9] = 6666;
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
