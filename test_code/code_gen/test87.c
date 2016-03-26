/* simple for loops */

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int i, res;

  i = 0;
  res = 0;
  for ( ; i < 10; i = i+1)
    res = res+i;

  for (i = 0; i < 10; ) {
    res = res+i;
    i = i+1;
  }

  for (i = 0; ; i = i+1) {
    res = res+i;
    if (i > 10) {
      print_int(res);
  print_string("\n");
      return;
    }
  }
}
