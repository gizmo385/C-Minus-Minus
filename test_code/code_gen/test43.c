/* assignment to a single global array */

int x[10];

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int y;

  x[7] = 3142;
  y = x[7];
  print_int(y);
  print_string("\n");
}
