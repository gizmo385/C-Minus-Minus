/* Calling function with different number of arguments */
int function(int a, char b, char string[]);

void main(void) {
    function(97, 'b', "Hello");
    function(97, 'b', "Hello", "This will not work");
    function(97, 'b');
}
