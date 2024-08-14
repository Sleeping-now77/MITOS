// read lines from the standard input and run a command for each line, 
// supplying the line as arguments to the command. 

// 管道会将左侧的标准输出作为右侧的标准输入

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// echo world | xargs echo hello

int
main(int argc, char **argv)
{   
    if (argc < 2)
    {
        fprintf(2, "usage: xargs <command> ...\n");
        exit(1);
    }

    char c[1];
    char buf[32];
    char *p;    //记录buf的结尾地址， =每次exec之后要恢复到 buf

    p = buf;

    for(int i = 0; i < argc-1; i++) {
        strcpy(argv[i], argv[i+1]);
    }
    while(read(0, c, sizeof(c)) > 0) {
        if(c[0] == '\n') {  // 读完一行了
            *p++ = '\0';
            
            strcpy(argv[argc-1], buf);

            int pid = fork();
            if(pid == 0) {
                exec(argv[0], argv);
            }
            else {
                wait(0);
                p = buf;
            }
        }
        else {
            *p++ = c[0];
        }
    }

    exit(0);
}