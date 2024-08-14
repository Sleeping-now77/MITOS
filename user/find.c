//find all the files in a directory tree with a specific name. 
//Use recursion to allow find to descend into sub-directories.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// char * fmtname(char *path) {
//     char *p;

//     // Find first character after last slash.
//     for(p=path+strlen(path); p >= path && *p != '/'; p--)
//         ;
//     p++;

//     return p;
// }

void Find_Loop(char *path, char* fname) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type) {
        case T_FILE:
            // if(strcmp(fmtname(path), fname) == 0) {
            //     printf("%s\n", path);
            // }
            break;

        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
                printf("ls: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';

            while(read(fd, &de, sizeof(de)) == sizeof(de)) {
                // 如果文件不存在，或者是 . 或者是 ..，则跳过
                if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;

                if(strcmp(p, fname) == 0) {
                    printf("%s\n", buf);
                }
                
                if(stat(buf, &st) < 0){
                    printf("ls: cannot stat %s\n", buf);
                    continue;
                }
                
                // 继续遍历进入子目录
                Find_Loop(buf, fname);
            }
            break;
        }
        close(fd);
}


int
main(int argc, char **argv)
{   
    if(argc != 3){
        fprintf(2, "Usage: find <path> <filename>\n");
        exit(1);
    }

    Find_Loop(argv[1], argv[2]);

    exit(0);

}