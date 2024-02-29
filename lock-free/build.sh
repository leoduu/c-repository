#!/bin/bash

gcc test.c queue.c ringbuffer.c -lpthread -o lock-free -I. -DLOCK=0
./lock-free

gcc test.c queue.c ringbuffer.c -lpthread -o lock-free -I. -DLOCK=1
./lock-free
