#ifndef RINGBUFF_H
#define RINGBUFF_H

#include <pthread.h>
#include <stdint.h>

// #define LOCK 0

typedef struct {
    uint32_t prodHead;
    uint32_t prodTail;
    uint32_t cons;
    // uint32_t consHead;
    // uint32_t consTail;

    uint32_t capacity;
#if LOCK
    pthread_mutex_t mutex;
#endif
    uint8_t buffer[0];
} RingBuffer;

RingBuffer *CreateRingBuffer(uint32_t capacity);
int WriteRingBuffer(RingBuffer *ring, void *buffer, uint32_t len);
int ReadRingBuffer(RingBuffer *ring, void *buffer, uint32_t len);
void DeleteRingBuffer(RingBuffer *ring);

#endif // RINGBUFF_H
