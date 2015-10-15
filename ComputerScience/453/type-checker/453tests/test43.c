/* nested while loops */

int foo(int x)
{
  while (x > 0) 
    while (x > 1)
      while (x > 2)
	while (x > 3)
	  while (x > 4)
	    while (x > 5)
	      while (x > 6)
		while (x > 7)
		  while (x > 8)
		    while (x > 9)
		      ;
  while (x > 0) 
    while (x > 1)
      while (x > 2)
	while (x > 3)
	  while (x > 4)
	    while (x > 5)
	      while (x > 6)
		while (x > 7)
		  while (x > 8)
		    while (x > 9)
		      x = x+1;
}
