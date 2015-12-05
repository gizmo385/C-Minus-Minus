/* more complex recursion */

int g(int n);

int f(int n)
{
  if (n <= 1) 
    return 1;
  else
    return g(n-1) + f(n-2);
}

int g(int n)
{
  if (n <= 1) 
    return 1;
  else
    return f(n-1) + g(n-2);
}

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int x;

  x = f(5);
  print_int(x);
  print_string("\n");
}
