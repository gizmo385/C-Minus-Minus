/* simple assignment to a local char variable */

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  char x;

  x = 'Q';
  print_int(x);
  print_string("\n");
}
