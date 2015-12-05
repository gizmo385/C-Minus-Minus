/* assignment: int --> char conversion: locals */

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  char x;

  x = 123;
  print_int(x);
  print_string("\n");
}
