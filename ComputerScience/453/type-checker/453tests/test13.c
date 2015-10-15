/* function prototypes: each prototype declares a single function */

void a( void );
void b(int x);
void c(int x, int y, int z);

int a1( void );
int b1(int x);
int c1(int x, char y, char z, int w);

char a2( void );
char b2(char x);
char c2(char x, char y, char z, int w);

extern int a3( void );
extern int b3(int x);
extern int c3(int x, char y, char z, int w);

extern char a4( void );
extern char b4(char x);
extern char c4(char x, char y, char z, int w);

int a5( void );
int b5(int x[]);
int c5(int x, char y[], char z, int w[], int u[], int v);

char a6( void );
char b6(char x[]);
char c6(char x, char y[], char z[], int w);

extern int a7( void );
extern int b7(int x[]);
extern int c7(int x[], char y, char z[], int w[]);

extern char a8( void );
extern char b8(char x[]);
extern char c8(char x, char y[], char z, int w[]);

