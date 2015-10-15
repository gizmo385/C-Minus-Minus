/* function definitions with mixed scalar and array formals,
   and local scalar and array declarations */

void foo0( int u[], int v )
{ 
  int x, y, z;
  int u[10], v, w[10];
}

void foo1( char v[], int w, int x ) 
{
  char a[10], b, c, d, e[10], f[10], g[20], h;
  int x, y, xx, yy, xxx[10], yyy[20], xxxx[30];
  int yyyy, zzz;
}

int foo2( int x, int w[], char v, int w[], char a ) 
{
  char a[10], b, c, d, e[10], f[10], g[20], h;
  int x, y, xx, yy, xxx[10], yyy[20], xxxx[30];
  int yyyy, zzz;
}

int foo3( int xx[], int yy, char zz[] ) 
{
  char a[10], b, c, d, e[10], f[10], g[20], h;
  int x, y, xx, yy, xxx[10], yyy[20], xxxx[30];
  int yyyy, zzz;
}

char foo4( int xx, int yy[] ) 
{
  char a[10], b, c, d, e[10], f[10], g[20], h;
  int x, y, xx, yy, xxx[10], yyy[20], xxxx[30];
  int yyyy, zzz;
}

