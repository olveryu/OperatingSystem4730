#ifndef ___QUEUES___
#define ___QUEUES___
#include <stdlib.h>

typedef struct node_t
{
    struct node_t* next;
    void* ptr;
} node_t;

typedef struct queue_t
{
    node_t* head;
    node_t* tail;
    int count;
} queue_t;

typedef struct queue_iterator_t 
{
    queue_t* q;
    node_t* prev;
    node_t* cur;
    node_t* next;
    int just_removed;
} queue_iterator_t;

queue_t* create_queue(void);
void enqueue(queue_t* , void* );
void* dequeue(queue_t*);
void destroy_queue(queue_t*);
queue_iterator_t* queue_iterator_create(queue_t*);
int queue_iterator_has_next(queue_iterator_t*);
void* queue_iterator_next(queue_iterator_t*);
void* queue_iterator_remove(queue_iterator_t*);
void queue_iterator_destroy(queue_iterator_t*);
#endif
