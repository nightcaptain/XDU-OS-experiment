#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define KEY 1234
#define BUFFER_SIZE 5

void P(int sem_id) {
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = -1;
    buf.sem_flg = SEM_UNDO;
    semop(sem_id, &buf, 1);
}

void V(int sem_id) {
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg = SEM_UNDO;
    semop(sem_id, &buf, 1);
}

int main() {
    int sem_id;
    pid_t pid;

    // 创建信号量
    sem_id = semget(KEY, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("无法创建信号量");
        exit(1);
    }

    // 初始化信号量为1，表示可用
    semctl(sem_id, 0, SETVAL, 1);

    // 创建子进程
    pid = fork();
    if (pid == -1) {
        perror("无法创建子进程");
        exit(1);
    } else if (pid == 0) {
        // 子进程为生产者进程
        int i;
        for (i = 1; i <= 10; i++) {
            sleep(1);
            P(sem_id);
            printf("生产者生产产品: %d\n", i);
            V(sem_id);
        }
        exit(0);
    } else {
        // 父进程为消费者进程
        int i;
        for (i = 1; i <= 10; i++) {
            sleep(2);
            P(sem_id);
            printf("消费者消费产品: %d\n", i);
            V(sem_id);
        }
        wait(NULL);
    }

    // 删除信号量
    semctl(sem_id, 0, IPC_RMID);

    return 0;
}
