/* a single global char variable; a single assignment of a char constant */

extern void print_int(int x);
extern void print_string(char x[]);

char x;

void main(void)
{
  x = 'a';

  print_int(x);
  print_string("\n");
}
