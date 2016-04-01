/* simple function calls */

int id(int x)
{
  return x;
}

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int n, f;

  f = 1;
  for (n = 7; n > 0; n = n-1)
    f = id(n)*f;
  print_int(f);
  print_string("\n");
}
