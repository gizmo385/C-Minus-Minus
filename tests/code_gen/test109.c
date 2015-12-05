/* a sequence of procedure calls */

int y;

void add(int x)
{
  y = x+y;
  return;
}

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int i, f;

  f = 1;
  for (i = 7; i > 0; i = i-1) {
    y = i;
    add(0);
    add(1);
    add(2);
    add(-5);
    add(2);
    f = f*y;
  }
  print_int(f);
  print_string("\n");  
}
