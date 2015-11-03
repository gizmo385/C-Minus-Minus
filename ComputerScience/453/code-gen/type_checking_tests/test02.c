/* Valid loops */
void main(void) {
    int x;

    while(1 == 1) { }

    for(x = 1; x < 5; x = x + 1) { }
    for(/* Missing initialization */; x < 5; x = x + 1) { }
    for(/* Missing initialization */; x < 5; /* Missing update */) { }
    for(/* Missing initialization */;/* Missing condition */; x = x + 1) { }
    for(/* Missing initialization */;/* Missing condition */; /* Missing update */) { }
    for(x = 1; /* Missing condition */ ; x = x + 1) { }
    for(x = 1; /* Missing condition */ ; /* Missing update */ ) { }
}
