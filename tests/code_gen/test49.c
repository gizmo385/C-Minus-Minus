/* simple assignment to a global char variable */

char x;

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  x = 'Q';
  print_int(x);
  print_string("\n");
}
