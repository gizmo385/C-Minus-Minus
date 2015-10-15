/* malformed for loop */

int foo(int x)
{
  for (x=0; x < 10; x += 1) 
    ;
}
