#include <stdlib.h>
#include <stdio.h>

#include "vector.h"
#include "utils.h"

/* Function prototypes */
int isInBounds( Vector *vector, int indexToCheck );
void resizeIfNecessary( Vector *vector );


int isInBounds( Vector *vector, int indexToCheck ) {
    return ( indexToCheck < vector->capacity );
}

void resizeIfNecessary( Vector *vector ) {
    if( vector->size == vector->capacity ) {
        int newCapacity = (vector->capacity * 1.25);
        void** newElements  = (void **)realloc( vector->elements, sizeof(void *) * newCapacity );

        if( ! newElements ) {
            free( vector->elements );
        }

        // Prevent unitialized pointers from being passed to free()
        for( int i = vector->capacity; i < newCapacity; i++ ) {
            newElements[i] = NULL;
        }

        vector->elements = newElements;
        vector->capacity = newCapacity;

        debug( E_DEBUG, "Resized vector to have capacity %d @ location %p\n", newCapacity,
                vector->elements);
    }
}

Vector *newVector( int initialCapacity ) {
    Vector *vector = NULL;

    if( initialCapacity >= 0 ) {
        vector = (Vector *) malloc( sizeof(Vector) );
        vector->capacity = initialCapacity;
        vector->size = 0;
        vector->elements = (void **) calloc( initialCapacity, sizeof(void *) );

        for( int i = 0; i < vector->capacity; i++ ) {
            vector->elements[i] = NULL;
        }
    }

    return vector;
}

void vectorAdd( Vector *vector, void *element ) {
    if( element == NULL ) {
        debug( E_WARNING, "Cannot add a NULL element to the vector!\n" );
        return;
    }

    resizeIfNecessary( vector );
    vector->elements[ (vector->size) ] = element;
    vector->size += 1;
}

int vectorInsert( Vector *vector, void *element, int index ) {
    if( isInBounds( vector, index ) ) {
        vector->elements[index] = element;
        return 1;
    } else {
        return 0;
    }
}

void *vectorRemove( Vector *vector, int index ) {
    if( isInBounds( vector, index ) ) {
        void *removedElement = vector->elements[index];
        vector->elements[index] = NULL;
        vector->size -= 1;

        return removedElement;
    }

    return NULL;
}

int vectorIsEmpty( Vector *vector ) {
    return (vector->size == 0);
}

void *vectorGet( Vector *vector, int index ) {
    if( isInBounds( vector, index ) ) {
        return vector->elements[index];
    } else {
        debug( E_WARNING, "WARNING: GET called with index that is out of bounds.\n" );
        return NULL;
    }
}

void freeVector( Vector *vector ) {
    for( int i = 0; i < vector->capacity; i++ ) {
        void *itemToFree = vectorGet(vector, i);
        if( itemToFree ) {
            free( itemToFree );
        }
    }

    free( vector->elements );
    free( vector );
}

void vectorFreeStructure( Vector *vector ) {
    free( vector->elements );
    free( vector );
}
