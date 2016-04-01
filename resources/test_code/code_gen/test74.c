/* nested array assignents: local arrays */

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int x[10];

  int u;

  x[0] = 1;
  x[1] = 2;
  x[2] = 3;
  x[3] = 4;
  x[4] = 5;
  x[5] = 6;
  x[6] = 7;
  x[7] = 8;
  x[8] = 9;
  x[9] = 10;

  u = x[x[x[x[x[x[x[x[x[x[0]]]]]]]]]];
  print_int(u);
  print_string("\n");
}
