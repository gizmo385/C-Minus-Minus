/* assignment: int --> char conversion: local var -> global var */

char y;

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int x;

  x = 123456;
  y = x;
  print_int(x);
  print_string("\n");
  print_int(y);
  print_string("\n");
}
