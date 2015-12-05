/* function calls: recursion on array params */

int sum(char x[], int i)
{
  if (x[i] == 0)
    return 0;
  else
    return x[i] + sum(x, i+1);
}

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int x;

  x = sum("hello world", 0);
  print_int(x);
  print_string("\n");
}
