/* nested conditionals */

int a, b, c, d, e, f, g;
int res;

extern void print_int (int n);
extern void print_string (char s[]);

void main(void)
{
  a = 1; b = 2; c = 3; d = 4; e = 5; f = 6;

  if (a < b)
    if (b >= c)
      if (c < d)
	if (!(d == e))
	  if (e < f)
	    if (g < f)
	      if (g < a)
		res = 1111;
	      else
		res = 2222;
	    else
	      res = 3333;
	  else
	    res = 4444;
	else 
	  res = 5555;
      else
	res = 6666;
    else
      res = 7777;
  else
    res = 8888;
  print_int(res);
  print_string("\n");
}

