/* mixed if-then-elses and for loops */

int f(int x, char y, int z)
{
  if (x > 0)
    for (x = 0; x > 1; x = x+1) 
      if (x > 2)
	for (x = 0; x > 3; x = x-1) 
	  ;
  for (x = 1; x > 0; x = x+1)
    if (x > 1) 
      if (x > 2)
	if (x > 3) 
	  for (x = 2; x < 4; x = x-1) 
	    for (y = 0; y > 0; y = y-1)
	      for (y = z; y == z; y = y-1)
		x = x+1;
	else x = x-1;
  if (x > 0)
    if (x > 1) {
      for (x = 0; x > 2; x = x+1)
	for (x = 1; x > 3; x = x-1) 
	  x = x+1;
    }
    else if (x > 4) {
      for ( ; x > 5; x = y-1)
	for (z = 0; x > 6; ) 
	  if (x > 7)
	    x = x+1;
	  else x = x-1;
    }
    else z=z-1;
}
