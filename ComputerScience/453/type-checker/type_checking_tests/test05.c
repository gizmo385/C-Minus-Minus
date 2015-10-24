int returnNumber(int a);

/* Nested stuff */
void main(void) {
    if(returnNumber(returnNumber(returnNumber(1))) < returnNumber(returnNumber(1 - 3 * 4 / 7))) {
        return;
    }
}
