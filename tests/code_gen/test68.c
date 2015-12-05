/* assignment: char --> int conversion: local var -> local var */

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int x;
  char y;

  y = 'Q';
  x = y;
  print_int(x);
  print_string("\n");
  print_int(y);
  print_string("\n");
}
