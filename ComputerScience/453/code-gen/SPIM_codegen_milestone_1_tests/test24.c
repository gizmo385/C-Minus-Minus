/* a procedure call with char->int type conversion (const argument) */

extern void print_int(int x);
extern void print_string(char x[]);

char x;

void foo(int y)
{
  print_int(y);
  print_string("\n");
}

void main(void)
{
  foo('\n');
}
