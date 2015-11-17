/* a single global int variable; multiple assignments */

extern void print_int(int x);
extern void print_string(char x[]);

int x;

void main(void)
{
  x = 123;
  x = 456;
  x = 789;

  print_int(x);
  print_string("\n");
}
