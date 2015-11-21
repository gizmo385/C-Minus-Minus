/* Variable name safety */
extern void print_int(int n);
extern void print_string(char string[]);

int j;
int jal;
int beq;
int bne;
int addi;
int addiu;
int slti;
int andi;
int ori;
int xori;
int lui;
int lb;
int lh;
int lw;
int sb;
int sh;
int sw;
int sll;
int srl;
int sra;
int jr;
int syscall;
int add;
int addu;
int sub;
int subu;
int and;
int or;
int xor;
int nor;
int slt;
int sltu;

void f1(int parameter) { }
void f2(int parameter) { }
void f3(int parameter) { }
void f4(int parameter) { }
void f5(int parameter) { }

void main(void) {
    j = 0;
    jal = 0;
    beq = 0;
    bne = 0;
    addi = 0;
    addiu = 0;
    slti = 0;
    andi = 0;
    ori = 0;
    xori = 0;
    lui = 0;
    lb = 0;
    lh = 0;
    lw = 0;
    sb = 0;
    sh = 0;
    sw = 0;
    sll = 0;
    srl = 0;
    sra = 0;
    jr = 0;
    syscall = 0;
    add = 0;
    addu = 0;
    sub = 0;
    subu = 0;
    and = 0;
    or = 0;
    xor = 0;
    nor = 0;
    slt = 0;
    sltu = 0;

    print_string("Zeroes: ");
    print_int(j);
    print_int(jal);
    print_int(beq);
    print_int(bne);
    print_int(addi);
    print_int(addiu);
    print_int(slti);
    print_int(andi);
    print_int(ori);
    print_int(xori);
    print_int(lui);
    print_int(lb);
    print_int(lh);
    print_int(lw);
    print_int(sb);
    print_int(sh);
    print_int(sw);
    print_int(sll);
    print_int(srl);
    print_int(sra);
    print_int(jr);
    print_int(syscall);
    print_int(add);
    print_int(addu);
    print_int(sub);
    print_int(subu);
    print_int(and);
    print_int(or);
    print_int(xor);
    print_int(nor);
    print_int(slt);
    print_int(sltu);
    print_string("\n");

    f1(0);
    f2(0);
    f3(0);
    f4(0);
    f5(0);
}
