/* recursive procedures */

int u;

void f(char s[], int n)
{
  if (s[n] == 0)
    return;
  else {
    u = u+s[n];
    f(s, n+1);
  }
}

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  f("hello world", 0);
  print_int(u);
  print_string("\n");
}
