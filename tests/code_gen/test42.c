/* assignments to a number of global scalars */

int x, y, z;

extern void print_int (int n); 
extern void print_string (char s[]);

void main(void)
{
  x = 13579;
  y = 24680;
  z = 21436;
  print_int(x);
  print_string("\n");
  print_int(y);
  print_string("\n");
  print_int(z);
  print_string("\n");
}
