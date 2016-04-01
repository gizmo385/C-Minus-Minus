/* assignment to a single local array */

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int x[10];
  int y;

  x[5] = 4321;
  y = x[5];
  print_int(y);
  print_string("\n");
}
