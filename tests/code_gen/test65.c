/* assignment: char --> int conversion: global var -> global var */

int x;
char y;

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  y = 'Q';
  x = y;
  print_int(x);
  print_string("\n");
  print_int(y);
  print_string("\n");
}
