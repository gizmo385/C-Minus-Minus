/* compound statements */

void foo(int x, int y)
{
  {
    {
      {
        {
	  x = 1; x = x+1; {x=x-1; y=2; {{{{{{{{{{}}}}}}}}}}}
	}
      }
    }
  }
}

