#ifndef FUNCTIONS_H
#define FUNCTIONS_H

/*
 * A comparison function takes two pointers to data and performs a comparison on them. If the two
 * items are equal, then this comparison function should return 1. If the first object is ordinally
 * less than the second object, then this should return -1. Otherwise, this function should return
 * some number 1.
 */
typedef int (*ComparisonFunction)(void *, void *);

#endif
