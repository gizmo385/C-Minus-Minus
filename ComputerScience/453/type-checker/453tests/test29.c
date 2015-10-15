/* function definitions with mixed scalar and array formals,
   and local scalar and array declarations, interspersed with
   function prototypes and global declarations */

void a( void ), b(int x), c(int x, int y, int z);

int a1( void ), b1(int x), c1(int x, char y, char z, int w);

char k, l, m;
char n[10], o[20];

void foo0( int u[], int v )
{ 
  int x, y, z;
  int u[10], v, w[10];
}

char a2( void ), b2(char x), c2(char x, char y, char z, int w);

extern int a3( void ), b3(int x), c3(int x, char y, char z, int w);

extern char a4( void ), b4(char x), c4(char x, char y, char z, int w);

int a, b[10], c, d[20], e, f[30];
char g[40], h[50];
int i, j;

char b6(char x[]), c6(char x, char y[], char z[], int w);

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

extern int a7( void ), b7(int x[]), c7(int x[], char y, char z[], int w[]);

extern char a8( void ), b8(char x[]), c8(char x, char y[], char z, int w[]);

