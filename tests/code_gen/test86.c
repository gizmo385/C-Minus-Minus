/* mixed while loops and conditionals */

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  int i, j, k, a, b;
  int res;

  i = 0;
  res = 123;
  while (i < 5) {
    j = 0;
    while (j < 5) {
      k = (i+j)/2;
      if (k > i) {
	a = 0;
	while (a < 5) {
	  b = (a+k)/2;
	  if (b > j) 
	    res = res + b;
	  else
	    res = res + k;
	  a = a+1;
	}
      }
      j = j+1;
    }
    i = i+1;
  }
  print_int(res);
  print_string("\n");
}
