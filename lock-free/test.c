#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdint.h>
#include <string.h>

#include "queue.h"
#include "ringbuffer.h"

#define TEST_CNT 10 * 10000
#define DATA_CNT 1000
#define THREAD_CNT 10

pthread_t thread[THREAD_CNT];
long gtime[THREAD_CNT];
Queue *q;
RingBuffer *ring;

void *TestEnQueue(void *arg)
{
    struct timeval start,end;
    gettimeofday(&start, NULL);

    for (int i = 0; i < TEST_CNT; i++) {
        if (EnQueue(q, (void *)123) != 0) {
            printf("enQueue failed!\n");
            break;
        }
    }
    gettimeofday(&end, NULL);

    long time = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    long index = (long)arg;
    gtime[index] = time;
    printf("enQueue[%ld] finish time: %ldms\n", index, time / 1000);
}

static void QueueTest()
{
    q = CreateQueue();
    if (q == NULL) {
        printf("Create queue failed!\n");
    }
    
    for (int i = 0; i < THREAD_CNT; i++) {
        if (pthread_create(&thread[i], NULL, TestEnQueue, (void *)(long)i)) {
            printf("thread %d create failed!\n", i);
            return;
        }
    }

    long time = 0;
    for (int i = 0; i < THREAD_CNT; i++) {
	    pthread_join(thread[i],NULL);
        time += gtime[i];
    }

    int cnt = 0;
    while (DeQueue(q) != NULL) {
        ++cnt;
    } 
    
    printf("enQueue cnt:%d total time: %ldms\n", THREAD_CNT * TEST_CNT, time/1000);
    printf("deQueue cnt:%d\n\n\n", cnt);
}

#if 1
static const char testBuf[] = "1234567890";
#else
static const char testBuf[] = "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
"123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
"123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
;
#endif

#define testBufLen sizeof(testBuf)

void *TestWriteRingbuffer(void *arg)
{
    struct timeval start,end;
    gettimeofday(&start, NULL);
    int i;
    for (i = 0; i < TEST_CNT; i++) {
        if (WriteRingBuffer(ring, (void *)testBuf, testBufLen) != 0) {
            printf("WriteRingBuffer failed!\n");
            break;
        }
    }
    gettimeofday(&end, NULL);

    long time = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    long index = (long)arg;
    gtime[index] = time;
    printf("WriteRingBuffer[%ld] cnt[%d] finish time: %ldms\n", index, i, time / 1000);
}

static void RingbufferTest()
{
    ring = CreateRingBuffer(THREAD_CNT * TEST_CNT * testBufLen);
    if (ring == NULL) {
        printf("Create ringbuffer failed!\n");
        return;
    }
    
    for (int i = 0; i < THREAD_CNT; i++) {
        if (pthread_create(&thread[i], NULL, TestWriteRingbuffer, (void *)(long)i)) {
            printf("thread %d create failed!\n", i);
            return;
        }
    }

    long time = 0;
    for (int i = 0; i < THREAD_CNT; i++) {
	    pthread_join(thread[i],NULL);
        time += gtime[i];
    }

    int cnt = 0;
    while(1) {
        char data[testBufLen] = {0};
        if (ReadRingBuffer(ring, (void *)data, testBufLen) != 0) {
            printf("ReadRingBuffer failed!\n");
            break;
        }
        if (strcmp(data, testBuf) != 0) {
            printf("ReadRingBuffer data err!\n");
            printf("data %x %x %x %x\n", data[0], data[1], data[2], data[3]);
            break;
        }
        if (++cnt == THREAD_CNT * TEST_CNT) {
            break;
        }
    }
    
    printf("Write cnt:%d total time: %ldms\n", THREAD_CNT * TEST_CNT, time/1000);
    printf("Read  cnt:%d\n\n\n", cnt);
}

int main()
{

#if LOCK
    printf("\nLOCK\n\n");
#else
    printf("\nLOCK-FREE\n\n");
#endif

    QueueTest();
    
    // RingbufferTest();

    return 0;
}

