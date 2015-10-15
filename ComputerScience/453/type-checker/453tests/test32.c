/* assignments involving more complex expressions */

int x, y, z[10];

void foo( int a, int b, char c, int d[], char e[], char f, char g[])
{
  int l0, l1[10], l2, l3;
  
  x = -1;
  x = 1+1;
  x = 1+1+1;
  x = 1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+-1+-2+-3+-4;
  x = y;
  x = y+z*y+z*y+z*y+z*y+z*y+z*y+z*y+z*y+z*y+z*y;
  z[0] = 0;
  z[1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1] = 0;
  z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]]]]]]]] = 1;
  a = z[0];
  a = z[1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1];
  a = z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]]]]]]]];
  z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]]]]]]]] = 1;
  a = z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]]]]]]]] + z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]]]]]]]];
  a = z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]]]]]]]] + z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]]]]]]]] * z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]]]]]]]] / z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]]]]]]]];
  a = (z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]]]]]]]] + z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]]]]]]]]) * z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]]]]]]]] / z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]]]]]]]];
  z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[x]]]]]]]]]]]]]]]]] = (z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]]]]]]]] + z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]]]]]]]]) * z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]]]]]]]] / z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]]]]]]]];
  l0 = a+2*(a-3*(b+4*(b-5*(c+6*(c-7*(x+8*(x-f+9*(f-g))))))));
  l2 = a+2*(a-3*(b+4*(b-5*(c+6*(c-7*(x+8*(x-f+9*(f-g))))))))/(a+2*(a-3*(b+4*(b-5*(c+6*(c-7*(x+8*(x-f+9*(f-g)))))))));
  l2 = foo();
  x = foo(1);
  y = foo(1,1);
  y = foo(1,-1,1);
  y = foo(-x);
  x = foo(1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1);
  y = foo(x);
  y = foo(x, y, z);
  l0 = foo(y+z*y+z*y+z*y+z*y+z*y+z*y+z*y+z*y+z*y+-z*-y);
  l0 = foo(y+z*y+z*y+z*y+z*y+z*y+z*y+z*y+z*y+z*y+z*y, 1+2+3+4+5+6+7+8+9+-1*-2/-3);
  l2 = foo(z[0]);
  l2 = foo(z[0], z[1], z[2]);
  l2 = foo(z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]);
  l2 = foo(z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]], z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]], z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]);
  l2 = foo(foo(foo(foo(foo(foo(0))))));
  l2 = foo(foo(foo(foo(foo(foo(0+1+2+3+4+5+6+7+8+9+x))))));
  l2 = foo(foo(foo(foo(foo(foo(z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]))))));
  l2 = foo(foo(foo(foo(foo(foo(z[z[z[z[z[z[z[z[z[z[0]]]]]]]]]]+1-'\n'))))));
  l2 = z[z[z[z[z[z[foo(foo(foo(foo(foo(foo(foo(foo(foo(foo(0))))))))))+1-'\n']]]]]];
}
