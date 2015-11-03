#ifndef LLIST_H
#define LLIST_H

typedef int (*ComparisonFunction)(void *, void *);

typedef struct ListNode {
    void *data;
    struct ListNode *next;
} ListNode;

typedef struct List {
    ListNode *head;
    int size;
    ComparisonFunction comparisonFunction;
} List;


extern ListNode *newListNode( void* data, ListNode *next );
extern List *newList(ComparisonFunction comparisonFunction);
extern void listInsert( List *list, void *data );
extern void *listRemove( List *list, void *data );
extern ListNode *listFind( List *list, void *data );
extern void listFree( List *list );

#endif
