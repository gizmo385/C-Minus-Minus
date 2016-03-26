/* type conversion: char to int with need for sign extension */

extern void print_int(int x);
extern void print_string(char x[]);

int x;
char y;

void main(void)
{
  y = 255;
  x = y;

  print_int(x);
  print_string("\n");
}
