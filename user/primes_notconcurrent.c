// Write a concurrent version of prime sieve using pipes.  
// 实现素数筛 

// 不对，这样并不是并行的。。
// 应该一个一个传

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void PipeTrans(int left[2]) {
    close(left[1]);

    int temp[35];
    int n = read(left[0], temp, sizeof(temp));
    close(left[0]);

    if(n > 0) {
        int N = temp[0];
        int p = temp[1];
        printf("prime %d\n", p);

        int cnt = 0;
        for(int i = 2; i <= N; i++) {
            if(temp[i] % p != 0) {
                temp[++cnt] = temp[i];
            }
        }
        temp[0] = cnt; // t[0]记录传到下一个pipe的个数

        // for(int i = 0; i <= N; i++) {
        //     printf("%d ", temp[i]);
        // }
        // printf("\n");

        int right[2];
        if(cnt > 0 && pipe(right) >= 0) {
            int pid = fork();
            if(pid == 0) {
                // printf("%d\n",getpid());
                PipeTrans(right);
            }
            else {
                close(right[0]);
                write(right[1], temp, sizeof(temp));
                wait(0);
                exit(0);
            }
        }
    }  
}



int num[35];

int
main(int argc, char *argv[])
{
    if(argc > 1){
        fprintf(2, "primes: error command\n");
        exit(1);
    }

    // 初始化范围数组（2~35）
    for(int i = 2; i <= 35; i++) {
        num[i-1] = i;
    }
    num[0] = 34;

    int pi[2];
    if(pipe(pi) < 0) {   //第一个管道，输出2，筛掉偶数
        fprintf(2,"Error:fail to call pipe()...");
    }

    int pid = fork();
    if(pid == 0) {
        PipeTrans(pi);
    }
    else {
        close(pi[0]); //关闭 write-end
        write(pi[1], num, sizeof(num));
        wait(0);
        exit(0);
    }

    return 0;  
}
