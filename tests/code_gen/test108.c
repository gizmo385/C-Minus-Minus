/* simple procedure calls */

int y;

void acc(int x)
{
  y = y*x;
}

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int n, f;

  y = 1;
  for (n = 7; n > 0; n = n-1)
    acc(n);
  print_int(y);
  print_string("\n");
}
