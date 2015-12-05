/* assignment: char --> int conversion: globals-->locals */

char y;

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int x;

  y = 'Q';
  x = y;
  print_int(x);
  print_string("\n");
  print_int(y);
  print_string("\n");
}

