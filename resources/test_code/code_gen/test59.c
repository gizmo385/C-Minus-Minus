/* assignment: int --> char conversion: locals-->globals */

char x;

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int y;

  y = 123456;
  x = y;
  print_int(x);
  print_string("\n");
  print_int(y);
  print_string("\n");
}
