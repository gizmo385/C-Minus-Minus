/* nested conditionals mixed with compound statements */

int f(int x, char y, int z)
{
  if (x > 0)
    if (x > 1) 
      if (x > 2)
	if (x > 3) ;
  if (x > 0)
    if (x > 1) 
      if (x > 2)
	if (x > 3) 
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
	else
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
  if (x > 0)
    if (x > 1) {
      if (x > 2)
	if (x > 3) 
	  x = x+1;
	else x = x-1;
    }
    else if (x > 4) {
      if (x > 5)
	if (x > 6) 
	  x = x+1;
	else x = x-1;
    }
    else z=z-1;
}
