/* assignment to a single local scalar variable */

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int x;

  x = 1234;
  print_int(x);
  print_string("\n");
}
