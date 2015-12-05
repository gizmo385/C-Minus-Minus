/* more complex while loops */

int i, x[5];
int res;

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  i = 0;
  while (i < 5) {
    x[i] = i;
    i = i+1;
  }
  res = 123;
  while (!(x[i/5] <= x[i/4] && x[i/4] <= x[i/3] && 
      !!!(x[i/2] > x[i/3] || x[i/3] > x[i/4] || x[i/4] > x[i/5]))) {
    res = res * i;
    i = i - 1;
  }
  print_int(res);
  print_string("\n");
}
