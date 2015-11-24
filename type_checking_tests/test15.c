/* string constants and array parameters */

int a[10];

char foo(int x[], char y[])
{
  int b[10];

  if (x[0] > 0)
    return foo(a, "hello world\n");
  else {
    y[0] = 'b';
    return x[foo(a, y)] + y[foo(a, "hi\n")];
  }
}
