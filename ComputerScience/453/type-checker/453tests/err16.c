/* unbalanced brackets in array references */

int foo(int z[])
{
  z z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]]]]]]] = 1;
}
