/* more complex conditionals */

int a, b, c, d, e, f, g, h, i, j;
int res;

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  a = 1; b = 2; c = 3; d = 4; e = 5;
  f = 6; g = 7; h = 8; i = 9; j = 10;

  if (a <= b && b <= c && c <= d && d <= e && 
      !!!(e > f || f > g || g > h || h > i || i > j))
    res = 1111;
  else
    res = 2222;
  print_int(res);
  print_string("\n");
}
