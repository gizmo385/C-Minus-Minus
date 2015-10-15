/* unbalanced braces in function body */

int foo(int x)
{
  {
    {
      {
	{
	  {
	    x = x+1;
	  }
	}
      }
    }
  }
}
}

