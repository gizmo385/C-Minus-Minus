#ifndef VECTOR_H
#define VECTOR_H

typedef struct Vector {
    int size;
    int capacity;
    void **elements;
} Vector;

extern Vector *newVector( int initialCapacity );
extern void vectorAdd( Vector *vector, void *element );
extern int vectorInsert( Vector *vector, void *element, int index );
extern void *vectorRemove( Vector *vector, int index );
extern int vectorIsEmpty( Vector *vector );
extern void *vectorGet( Vector *vector, int index );
extern void freeVector( Vector *vector );
extern void vectorFreeStructure( Vector *vector );

#endif
