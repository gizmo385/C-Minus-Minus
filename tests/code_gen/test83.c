/* simple while loops */

int i;

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  i = 0;
  while (i < 100) {
    print_int(i);
  print_string("\n");
    i = i+i+1;
  }
}
