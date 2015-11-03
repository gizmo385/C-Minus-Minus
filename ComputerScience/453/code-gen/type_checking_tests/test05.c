/* Nested stuff */
int returnNumber(int a);

void main(void) {
    if(returnNumber(returnNumber(returnNumber(1))) < returnNumber(returnNumber(1 - 3 * 4 / 7))) {
        return;
    }
}
