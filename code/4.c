#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int pipefd[2];
    pid_t pid;
    char buffer[50];
    int bytesRead;

    // 创建匿名管道
    if (pipe(pipefd) == -1) {
        fprintf(stderr, "无法创建管道\n");
        return 1;
    }

    // 创建子进程
    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "无法创建子进程\n");
        return 1;
    } else if (pid == 0) {
        // 子进程关闭写入端
        close(pipefd[1]);

        // 从管道中读取数据
        bytesRead = read(pipefd[0], buffer, sizeof(buffer));
        printf("子进程收到消息: %s\n", buffer);

        // 关闭读取端
        close(pipefd[0]);
        exit(0);
    } else {
        // 父进程关闭读取端
        close(pipefd[0]);

        // 向管道中写入数据
        write(pipefd[1], "Hello, child!", 14);

        // 关闭写入端
        close(pipefd[1]);
    }

    return 0;
}
