#include "queue.h"

queue_t* create_queue()
{
    queue_t* q = (queue_t*) malloc(sizeof(queue_t));
    q->head = NULL;
    q->tail = NULL;
    q->count = 0;
    return q;
}


void enqueue(queue_t* q, void* val)
{
    
    q->count++;
    node_t* n = (node_t*) malloc(sizeof(node_t));
    n->next =NULL;
    n->ptr = val;
    if(q->tail == NULL) 
    {
        q->tail = n;
        q->head = n;
    }
    else {
        q->tail->next = n;
        q->tail=n;
    }
}

void* dequeue(queue_t* q){
    if(q->count<1) return NULL;
    q->count--;
    node_t* n = q->head;
    q->head = n->next;
    if(q->head == NULL) q->tail = NULL;
    void* s =  n->ptr;
    free(n);
    return s;
}

void destroy_queue(queue_t* q){
    while(q->count >0) dequeue(q);
    free(q);

}

queue_iterator_t* queue_iterator_create(queue_t* q){
    queue_iterator_t* qitr = (queue_iterator_t*)malloc(sizeof(queue_iterator_t));
    qitr->q = q;
    qitr->cur = NULL;
    qitr->prev = NULL;
    qitr->next = q->head;
    qitr->just_removed=0;
    return qitr;
}
int queue_iterator_has_next(queue_iterator_t* qitr){
    return (qitr->next !=NULL);
}
void* queue_iterator_next(queue_iterator_t* qitr){
    if(!queue_iterator_has_next(qitr)) return NULL;
    if(!qitr->just_removed)
        qitr->prev = qitr->cur;
    qitr->cur = qitr->next;
    qitr->next = qitr->next->next;
    qitr->just_removed=0;
    return qitr->cur->ptr;
}

/*remove the current node the iterator points to*/
void* queue_iterator_remove(queue_iterator_t* qitr){
    if (qitr->cur == NULL) return NULL; /* can't remove what is not there*/
    if (qitr->prev !=NULL){ 
        qitr->prev->next = qitr->next;
        if(qitr->q->tail == qitr->cur)
            qitr->q->tail = qitr->prev;
    }else if(qitr->q->head == qitr->cur){/*no prev, cur must be head, check to be sure*/
        qitr->q->head = qitr->next;
        if(qitr->q->head == NULL) 
            qitr->q->tail = NULL;
    }
    void* ptr = qitr->cur->ptr;
    free(qitr->cur);
    qitr->q->count --;
    qitr->cur = NULL;
    qitr->just_removed=1;
    return ptr;
}

void queue_iterator_destroy(queue_iterator_t* qitr){
    free(qitr);
}

