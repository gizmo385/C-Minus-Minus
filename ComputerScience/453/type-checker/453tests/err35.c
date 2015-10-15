/* malformed for loop */

int foo(void)
{
  for (x = 0; x < 10; x = x+1;)
    ;
}
