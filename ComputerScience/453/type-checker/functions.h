#ifndef FUNCTIONS_H
#define FUNCTIONS_H

/*
 * A comparison function takes two pointers to data and performs a comparison on them. If the two
 * items are equal, then this comparison function should return 1. If the first object is ordinally
 * less than the second object, then this should return -1. Otherwise, this function should return
 * some number 1.
 */
typedef int (*ComparisonFunction)(void *, void *);

/*
 * An element consumer is a function that is provided to some structure iterator and performs an
 * operation on each element within the structure. This ignores the actual internal structure of
 * whatever is being iterated over. For example, in a BST, it will perform the operation on the data
 * element within each node rather than performing an operation on the node itself.
 */
typedef void (*ElementConsumer)(void *);

/*
 * A map function takes an element, performs some modification on that element and returns the
 * modified element.
 */
typedef void *(*MapFunction)(void *);

#endif
