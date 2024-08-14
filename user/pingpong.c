//Use pipe to create a pipe.
//Use fork to create a child.
//Use read to read from the pipe, and write to write to the pipe.
//Use getpid to find the process ID of the calling process.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int
main(int argc, char **argv)
{   
    if(argc != 1){
        fprintf(2, "usage: pingpong...\n");
        exit(1);
    }

    // 父进程创建两个pipe，每个方向各一个
    // p1: 父->子，父（read关，write开），子（read开，write关）
    // p2: 子->父，父（read开，write关），子（read关，write开）
    int p1[2],p2[2];
    if(pipe(p1) < 0 || pipe(p2) < 0) {
        fprintf(2,"Error:fail to call pipe()...");
    }

    int pid = fork();
    char buf1[1], buf2[1];
    int n1 = 0;
    int n2 = 0;
    if(pid == 0) {  //子进程
        //子进程关闭 p1 的write
        // close(0);
        // dup(p1[0]); //重定向子进程的输入为标准输入
        close(p1[1]);
        close(p2[0]);
        n1 = read(p1[0],buf1,sizeof(buf1));
        if(n1 < 0) {
            fprintf(2,"Error:pipe1 fails to transport data...\n");
            exit(1);
        }
        else {
            printf("%d: received ping\n",getpid());
            write(p2[1], "o", sizeof(buf2));
            exit(0);
        }
    }
    else {  // 父进程
        close(p1[0]);
        close(p2[1]);
        write(p1[1], "o", sizeof(buf1));

        wait(0);

        n2 = read(p2[0], buf2, sizeof(buf2));
        if(n2 < 0) {
            fprintf(2,"Error:pipe2 fails to transport data...\n");
            exit(1);
        }
        else {
            printf("%d: received pong\n",getpid());
            exit(0);
        }
    }
}
