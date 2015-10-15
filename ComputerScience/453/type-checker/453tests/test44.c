/* mixed if-then-elses and while loops */

int f(int x, char y, int z)
{
  if (x > 0)
    while (x > 1) 
      if (x > 2)
	while (x > 3) 
	  ;
  while (x > 0)
    if (x > 1) 
      if (x > 2)
	if (x > 3) 
	  while (x < 4) 
	    while (y > 0)
	      while (y == z)
		x = x+1;
	else x = x-1;
  if (x > 0)
    if (x > 1) {
      while (x > 2)
	while (x > 3) 
	  x = x+1;
    }
    else if (x > 4) {
      while (x > 5)
	while (x > 6) 
	  if (x > 7)
	    x = x+1;
	  else x = x-1;
    }
    else z=z-1;
}
