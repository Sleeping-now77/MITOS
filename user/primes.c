// Write a concurrent version of prime sieve using pipes.  
// 实现素数筛 


#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void PipeTrans(int left[2]) {
    close(left[1]);

    int prime;
    // 如果没读到，递归结束
    // 如果读到了，第一个一定是 prime
    if(!read(left[0], &prime, sizeof(int))) {
        close(left[0]);
        exit(0);
    }

    printf("prime %d\n", prime);

    

    int right[2];
    if(pipe(right) >= 0) {
        int pid = fork();
        if(pid == 0) {
            PipeTrans(right);
        }
        else {
            close(right[0]);

            int num;
            while(read(left[0], &num, sizeof(int)) > 0) {
                // printf("%d ", num);
                if(num % prime) {
                    write(right[1], &num, sizeof(int));
                }
            }
            // printf("\n\n");
            close(left[0]);
            close(right[1]);

            wait(0);
            exit(0);
        }
    }
}  



int
main(int argc, char *argv[])
{
    if(argc > 1){
        fprintf(2, "primes: error command\n");
        exit(1);
    }

    int fd[2];
    if(pipe(fd) < 0) {   //第一个管道，输出2，筛掉偶数
        fprintf(2,"Error:fail to call pipe()...");
    }

    int pid = fork();
    if(pid == 0) {
        PipeTrans(fd);
        exit(0);
    }
    else {
        close(fd[0]); //关闭 write-end
        for(int i = 2; i <= 35; i++) {
            write(fd[1], &i, sizeof(i));
        }
        close(fd[1]);

        wait(0);
        exit(0);
    }

    return 0;  
}
