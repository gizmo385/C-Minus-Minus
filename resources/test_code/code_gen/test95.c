/* simple recursion */

int fact(int n)
{
  if (n == 0)
    return 1;
  else
    return n*fact(n-1);
}

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int x;

  x = fact(7);
  print_int(x);
  print_string("\n");
}
