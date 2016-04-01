/* assignment: int --> char conversion: globals-->locals */

int y;

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  char x;

  y = 123456;
  x = y;
  print_int(x);
  print_string("\n");
  print_int(y);
  print_string("\n");
}
