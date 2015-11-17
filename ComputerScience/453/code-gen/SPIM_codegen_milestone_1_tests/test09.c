/* type conversion: char to int without need for sign extension */

extern void print_int(int x);
extern void print_string(char x[]);

int x;
char y;

void main(void)
{
  y = 'a';
  x = y;

  print_int(x);
  print_string("\n");
}
