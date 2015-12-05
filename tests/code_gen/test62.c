/* assignment: int --> char conversion: global var -> local var */

int x;

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  char y;

  x = 123456;
  y = x;
  print_int(x);
  print_string("\n");
  print_int(y);
  print_string("\n");
}
