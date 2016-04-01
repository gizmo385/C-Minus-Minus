/* nested for loops */

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int i, j, k, res;

  res = 0;
  for (i = 0; i < 5; i = i+1)
    for (j = 0; j < 10; j = j+2)
      for (k = 0; k < 15; k = k+3)
	res = res + (i+j+k)/3;
  print_int(res);
  print_string("\n");
}
