/* assignment: char --> int conversion: local var -> global var */

int x;

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  char y;

  y = 'Q';
  x = y;
  print_int(x);
  print_string("\n");
  print_int(y);
  print_string("\n");
}
