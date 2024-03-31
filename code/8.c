#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5

sem_t empty;    // 空缓冲区信号量
sem_t full;     // 满缓冲区信号量
int buffer[BUFFER_SIZE];   // 缓冲区
int in = 0;     // 生产者写入位置
int out = 0;    // 消费者读取位置

void *producer(void *arg) {
    int item = 1;

    while (1) {
        // 等待空缓冲区
        sem_wait(&empty);

        // 生产产品
        buffer[in] = item;
        printf("生产者生产产品: %d\n", item);
        item++;

        // 更新写入位置
        in = (in + 1) % BUFFER_SIZE;

        // 发送满缓冲区信号
        sem_post(&full);
    }

    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int item;

    while (1) {
        // 等待满缓冲区
        sem_wait(&full);

        // 消费产品
        item = buffer[out];
        printf("消费者消费产品: %d\n", item);

        // 更新读取位置
        out = (out + 1) % BUFFER_SIZE;

        // 发送空缓冲区信号
        sem_post(&empty);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t producerThread, consumerThread;

    // 初始化信号量
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    // 创建生产者线程
    if (pthread_create(&producerThread, NULL, producer, NULL) != 0) {
        fprintf(stderr, "无法创建生产者线程\n");
        return 1;
    }

    // 创建消费者线程
    if (pthread_create(&consumerThread, NULL, consumer, NULL) != 0) {
        fprintf(stderr, "无法创建消费者线程\n");
        return 1;
    }

    // 等待线程结束
    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);

    // 销毁信号量
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
