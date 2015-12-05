/* a sequence of function calls */

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
    a = add(i, 0);
    b = add(a, 1);
    c = add(2, b);
    d = add(c, -5);
    e = add(d, 1);
    f = f*add(e, 1);
  }
  print_int(f);
  print_string("\n");  
}
