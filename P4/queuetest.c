
#include <stdio.h>
#include "queue.h"

int* makeFive(){
    int* k = (int*)malloc(sizeof(int));
    *k=5;
    return k;
}

int main(int argc, char** argv){
    queue_t* q = create_queue();
    int x = 7;
    int i; 
    for(i =0; i<10; i++){
        int* k = (int*)malloc(sizeof(int));
        *k = i;
        enqueue(q, (void*)k);
    }
    queue_iterator_t* qitr = queue_iterator_create(q);
    while(queue_iterator_has_next(qitr)){
        int* k  = (int*)queue_iterator_next(qitr);
        printf("%d",*k);
        if(*k == 3 || *k ==7 || *k==1 || *k == 0 || *k == 9 || 1){
            queue_iterator_remove(qitr);
            printf(" removed");
        }
        printf("\n");
    }
    queue_iterator_destroy(qitr);
    printf("**************\n");

    qitr = queue_iterator_create(q);
    while(queue_iterator_has_next(qitr)){
        int* k  = (int*)queue_iterator_next(qitr);
        printf("%d\n",*k);
    }
    queue_iterator_destroy(qitr);
}
