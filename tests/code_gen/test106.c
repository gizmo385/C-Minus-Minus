/* function calls: char array parameters */

char x[15];

int copy(char a[], char b[])
{
  int i;

  i = 0;
  for (i = 0; a[i] > 0; i = i+1)
    b[i] = a[i];
  b[i] = 0;
  return i;
}

int sum(char b[])
{
  int i, sum;

  i = 0;
  sum = 0;
  while (b[i] > 0) {
    sum = sum+b[i];
    i = i+1;
  }
  return sum;
}

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int y;

  y = copy("hello world", x);
  y = y+sum(x);
  print_int(y);
  print_string("\n");
}
