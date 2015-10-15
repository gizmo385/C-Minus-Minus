/* mixed while and for loops */

int foo(int x, int y)
{
  for (x = 0; x < 10; x = x+1)
    for (x = 0; x < 10; x = x+1)
      while (y > 0)
	for (x = 0; x < 10; x = x+1)
	  while (x < 0)
	    while (x > y)
	      for (x = 0; x < 10; x = x+1)
		for (x = 0; x < 10; x = x+1)
		  for (x = 0; x < 10; x = x+1)
		    while (x > 22)
		      while (y < -10)
			x = y+1;
}
