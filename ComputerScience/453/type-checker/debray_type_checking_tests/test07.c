/* variables: declaration before use */

int glob1, glob2;

void foo(void)
{
  int loc1, loc2;
  
  loc1 = glob1+glob2;
  loc2 = loc1+1;
  glob2 = loc2*glob1;
  return;
}

