/* function definitions interspersed with global declarations and
   function prototypes */

void a( void ), b(int x), c(int x, int y, int z);

int a1( void ), b1(int x), c1(int x, char y, char z, int w);
int x, y[10], z;
int x0, y0, z0[20];

void foo0( void ) {}

void foo1( int x ) {}

char u0, u1[10];
char a2( void ), b2(char x), c2(char x, char y, char z, int w);

extern int a3( void ), b3(int x), c3(int x, char y, char z, int w);

extern char a4( void ), b4(char x), c4(char x, char y, char z, int w);

void foo2( int x, int y, int z ) {}

int foo3( int x[], char y, int z[], char w[] ) {}

int x1, x2[100], x3, x4, x5[1000];
int b5(int x[]), c5(int x, char y[], char z, int w[], int u[], int v);

char b6(char x[]), c6(char x, char y[], char z[], int w);

char foo4( int x[], char y, int z[], char w[] ) {}

extern int a7( void ), b7(int x[]), c7(int x[], char y, char z[], int w[]);

extern char a8( void ), b8(char x[]), c8(char x, char y[], char z, int w[]);

