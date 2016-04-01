/* This code is here to interface with printf() from the C library.
   For use with GCC. */

#include <stdio.h>

void print_int(int x)
{
  printf("%d", x);
}

void print_string(char x[])
{
  printf("%s", x);
}
