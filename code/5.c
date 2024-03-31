#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_NAME "/tmp/myfifo"
#define BUFFER_SIZE 1024

int main() {
    int fd;
    char buffer[BUFFER_SIZE];

    // 创建命名管道
    mkfifo(FIFO_NAME, 0666);

    // 创建子进程
    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "无法创建子进程\n");
        return 1;
    } else if (pid == 0) {
        // 子进程
        // 打开命名管道以进行写入
        fd = open(FIFO_NAME, O_WRONLY);
        if (fd == -1) {
            fprintf(stderr, "无法打开命名管道\n");
            return 1;
        }

        // 写入数据到命名管道
        char* message = "Hello, parent!";
        write(fd, message, strlen(message) + 1);

        // 关闭文件描述符
        close(fd);
        exit(0);
    } else {
        // 父进程
        // 打开命名管道以进行读取
        fd = open(FIFO_NAME, O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "无法打开命名管道\n");
            return 1;
        }

        // 从命名管道中读取数据
        read(fd, buffer, BUFFER_SIZE);
        printf("父进程收到消息: %s\n", buffer);

        // 关闭文件描述符
        close(fd);

        // 删除命名管道
        unlink(FIFO_NAME);
    }

    return 0;
}
