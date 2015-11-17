/* a single global int variable; a single assignment */

extern void print_int(int x);
extern void print_string(char x[]);

int x;

void main(void)
{
  x = 123;

  print_int(x);
  print_string("\n");
}
