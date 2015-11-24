/* type conversion: int to char  without need for truncation */

extern void print_int(int x);
extern void print_string(char x[]);

int x;
char y;

void main(void)
{
  x = 12;
  y = x;

  print_int(y);
  print_string("\n");
}
