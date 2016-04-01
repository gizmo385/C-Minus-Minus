/* nested function calls */

int add(int x, int y)
{
  return x+y;
}

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int a, b, c, d, e;
  int i, f;

  f = 1;
  for (i = 7; i > 0; i = i-1) {
    f = f*add(add(1, add(add(2, add(add(i, 0), 1)), -5)), 1);
  }
  print_int(f);
  print_string("\n");  
}
