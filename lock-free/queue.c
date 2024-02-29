#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

#define CAS(P, O, V) __sync_bool_compare_and_swap(P, O, V)

Queue *CreateQueue(void)
{
    Queue *q = (Queue *)malloc(sizeof(Queue));
    // 第一个节点作为哨兵节点
    if (q != NULL) {
        Node *head = (Node *)malloc(sizeof(Node));
        if (head == NULL) {
            return NULL;
        }
        q->head = head;
        q->tail = q->head;
    }
#if LOCK
    pthread_mutex_init(&q->mutex, NULL);
#endif
    return q;
}

#if LOCK
int EnQueue(Queue *queue, void *element)
{
    if (queue == NULL) {
        return -1;
    }

    Node *tail;
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        return -1;
    }

    node->next = NULL;
    node->element = element;

    pthread_mutex_lock(&queue->mutex);
    queue->tail->next = node;
    queue->tail = node;
    pthread_mutex_unlock(&queue->mutex);

    return 0;
}

#else

int EnQueue(Queue *queue, void *element)
{
    if (queue == NULL) {
        return -1;
    }

    Node *tail;
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        return -1;
    }

    node->next = NULL;
    node->element = element;

    while (1) {
        tail = queue->tail;
        if (CAS(&tail->next, NULL, node)) {
            break;
        } else {
            CAS(&queue->tail, tail, tail->next);
        }
    }

    CAS(&queue->tail, tail, node);
    return 0;
}
#endif

void *DeQueue(Queue *queue)
{
    if (queue == NULL) {
        return NULL;
    }

    Node *node;
    void *element;
    
    do {
        node = queue->head;
        if (node->next == NULL) {
            return NULL;
        }
    } while (!CAS(&queue->head, node, node->next));

    element = node->next->element;
    free(node);
    return element;
}

void DeleteQueue(Queue *queue)
{

}


