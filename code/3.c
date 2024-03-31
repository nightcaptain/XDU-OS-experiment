#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signalHandler(int signum) {
    printf("接收到信号 %d\n", signum);
    // 在此处进行相应的处理
    // ...
}

int main() {
    pid_t pid;

    // 创建有名事件（信号）
    if (signal(SIGUSR1, signalHandler) == SIG_ERR) {
        fprintf(stderr, "无法注册信号处理程序\n");
        return 1;
    }

    // 创建子进程
    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "无法创建子进程\n");
        return 1;
    } else if (pid == 0) {
        // 子进程发送信号给父进程
        kill(getppid(), SIGUSR1);
        exit(0);
    } else {
        // 父进程等待信号
        pause();
        printf("父进程收到信号，处理完成\n");
    }

    return 0;
}
