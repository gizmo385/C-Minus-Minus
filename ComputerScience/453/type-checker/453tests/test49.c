/* mixed if-thens, while loops, and for loops */

int foo(int x, int y)
{
  if (x > y)
    if (y < 0)
      for (x = 0; x < 10; x = x+1)
	for (x = 0; x < 10; x = x+1)
	  if (x == y)
	    while (y > 0)
	      for (x = 0; x < 10; x = x+1)
		while (x < 0)
		  while (x > y)
		    for (x = 0; x < 10; x = x+1)
		      x = y+1;
	  else 
	    for (x = 0; x < 10; x = x+1)
	      if (x != y)
		for (x = 0; x < 10; x = x+1)
		  while (x > 22)
		    while (y < -10)
		      x = y+1;
}

