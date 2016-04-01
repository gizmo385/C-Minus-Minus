/* assignment: int --> char conversion: globals */

char x;

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  x = 123;
  print_int(x);
  print_string("\n");
}
