/* nested for loops */

int foo(int x)
{
  for (x = 0; x < 10; x = x+1)
    for (x = 0; x < 10; x = x+1)
      for (x = 0; x < 10; x = x+1)
	for (x = 0; x < 10; x = x+1)
	  for (x = 0; x < 10; x = x+1)
	    for (x = 0; x < 10; x = x+1)
	      for (x = 0; x < 10; x = x+1)
		for (x = 0; x < 10; x = x+1)
		  for (x = 0; x < 10; x = x+1)
		    for (x = 0; x < 10; x = x+1)
		      for (x = 0; x < 10; x = x+1)
			for (x = 0; x < 10; x = x+1)
			  for (x = 0; x < 10; x = x+1)
			    x = x+1;
}
