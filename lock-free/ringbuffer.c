#include "ringbuffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sched.h>

#define CAS(P, O, V) __sync_bool_compare_and_swap(P, O, V)

static uint32_t GetSpace(RingBuffer *ring)
{
    return ring->capacity - (ring->prodHead - ring->cons);
}

static uint32_t GetExist(RingBuffer *ring)
{
    return ring->prodHead - ring->cons;
}

static int UpdateProdHead(RingBuffer *ring, uint32_t len, uint32_t *oldHead, uint32_t *newHead)
{

#if LOCK
    *oldHead = ring->prodHead;
    *newHead = *oldHead + len;
    ring->prodHead = *newHead;
#else
    do {
        *oldHead = ring->prodHead;

        if (GetSpace(ring) < len) {
            return -1;
        }
        *newHead = *oldHead + len;
    } while(!CAS(&ring->prodHead, *oldHead, *newHead));
#endif

    return 0;
}

static void UpdateProdTail(RingBuffer *ring, uint32_t oldHead, uint32_t newHead) 
{
#if LOCK
    (void)oldHead;
    (void)newHead;
    ring->prodTail = ring->prodHead;
#else
    while (!CAS(&ring->prodTail, oldHead, newHead)) {
        sched_yield();
    }
#endif
}

int WriteRingBuffer(RingBuffer *ring, void *buffer, uint32_t len)
{
    uint32_t oldHead;
    uint32_t newHead;

#if LOCK
    pthread_mutex_lock(&ring->mutex);
#endif

    if (UpdateProdHead(ring, len, &oldHead, &newHead) != 0) {
        return -1;
#if LOCK
        pthread_mutex_unlock(&ring->mutex);
#endif
    }

    // printf("oldHead:%u newHead:%u\n", oldHead, newHead);

    uint32_t headMask = oldHead % ring->capacity;
    uint32_t left = ring->capacity - headMask;
    if (left > len) {
        memcpy(ring->buffer + headMask, buffer, len);
    } else {
        memcpy(ring->buffer + headMask, buffer, left);
        memcpy(ring->buffer, (uint8_t *)buffer + left, len - left);
    }
    UpdateProdTail(ring, oldHead, newHead);

    // if (ring->prodTail < 40) {
    //     printf("headMask:%u ring->buffer:%s\n", headMask, ring->buffer);
        
    //     for (int i = 0; i < ring->prodTail; i++) {
    //         if (ring->buffer[i] == 0) {
    //             printf(" ");
    //         } else {
    //             printf("%c", ring->buffer[i]);
    //         }
    //     }
    //     printf("\nlen:%d\n", len);
    // }

#if LOCK
    pthread_mutex_unlock(&ring->mutex);
#endif

    return 0;
}

int ReadRingBuffer(RingBuffer *ring, void *buffer, uint32_t len)
{
    if (GetExist(ring) < len) {
        return -1;
    }
    
    uint32_t headMask = ring->cons % ring->capacity;
    uint32_t left = ring->capacity - headMask;
    if (left > len) {
        memcpy(buffer, ring->buffer + headMask, len);
    } else {
        memcpy(buffer, ring->buffer + headMask, left);
        memcpy((uint8_t *)buffer + left, ring->buffer, len - left);
    }
    ring->cons += len;
    
    // printf("ring->buffer:%s\n", ring->buffer + headMask);
    // printf("consHead:%u\n", ring->cons);
    return 0;
}

RingBuffer *CreateRingBuffer(uint32_t capacity)
{
    RingBuffer *ring = (RingBuffer *)malloc(sizeof(RingBuffer) + capacity);
    if (ring == NULL) {
        return NULL;
    }
    ring->capacity = capacity;
    ring->prodHead = 0;
    ring->prodTail = 0;
    ring->cons = 0;
    // ring->consHead = 0;
    // ring->consTail = 0;
#if LOCK
    pthread_mutex_init(&ring->mutex, NULL);
#endif
    return ring;
}

void DeleteRingBuffer(RingBuffer *ring)
{

}
