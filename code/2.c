#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int sharedData = 0; // 全局共享数据

void* threadFunc(void* arg) {
    // 在线程中更改共享数据
    sharedData = 100;

    // 输出共享数据
    printf("线程中的共享数据: %d\n", sharedData);

    pthread_exit(NULL);
}

int main() {
    pthread_t thread;
    int ret;

    // 创建线程
    ret = pthread_create(&thread, NULL, threadFunc, NULL);
    if (ret != 0) {
        fprintf(stderr, "无法创建线程\n");
        return 1;
    }

    // 等待线程结束
    ret = pthread_join(thread, NULL);
    if (ret != 0) {
        fprintf(stderr, "无法等待线程\n");
        return 1;
    }

    // 输出共享数据
    printf("进程中的共享数据: %d\n", sharedData);

    return 0;
}
