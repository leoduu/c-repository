#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>

// #define LOCK 1

// 单链表
typedef struct _Node {
    struct _Node *next;
    void *element;
} Node;

typedef struct {
    Node *head;
    Node *tail;
#if LOCK
    pthread_mutex_t mutex;
#endif
} Queue;

Queue *CreateQueue(void);
int EnQueue(Queue *queue, void *element);
void * DeQueue(Queue *queue);
void DeleteQueue(Queue *queue);


#endif // QUEUE_H
