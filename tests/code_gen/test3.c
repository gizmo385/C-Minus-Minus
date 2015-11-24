/* a single global char variable; a single assignment of an int constant */

extern void print_int(int x);
extern void print_string(char x[]);

char x;

void main(void)
{
  x = 10;

  print_int(x);
  print_string("\n");
}
