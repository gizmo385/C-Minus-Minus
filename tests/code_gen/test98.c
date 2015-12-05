/* function calls in if, while, and for statements */

int id(int x)
{
  return x;
}

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int x, i;

  x = 123;
  if (id(id(3)) > id(id(id(4)))) 
    x = 321;
  i = 12;
  while (id(id(id(i))) > 0) {
    x = x-1;
    i = i-1;
  }
  for (i = id(0); id(id(i)) < 6; i = id(i)+1)
    x = x+2;
  print_int(x);
  print_string("\n");
}
