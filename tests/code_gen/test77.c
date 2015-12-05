/* simple arithmetic expressions: array variables */

char y[5];

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int x[5];

  int u;

  x[0] = 0;  y[0] = 1;
  x[1] = 1;  y[1] = 2;
  x[2] = 2;  y[2] = 3;
  x[3] = 3;  y[3] = 4;
  x[4] = 4;  y[4] = 55;

  u = x[0]+y[0];
  u = x[1]+u;
  u = y[u]*x[3];
  u = u/x[2];
  u = y[u] - x[0];
  print_int(u);
  print_string("\n");
}
