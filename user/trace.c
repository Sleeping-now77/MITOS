// You have to modify the xv6 kernel to print out a line when each system call is about to return, 
// if the system call's number is set in the mask.

//The line should contain the process id, the name of the system call and the return value

//The trace system call should enable tracing for the process that calls it and any children that it subsequently forks,


#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// trace 32 grep hello README

int
main(int argc, char *argv[])
{
    int i;
    char *nargv[MAXARG];

    if(argc < 3 || (argv[1][0] < '0' || argv[1][0] > '9')){
      fprintf(2, "Usage: %s mask command\n", argv[0]);
      exit(1);
    }

    if (trace(atoi(argv[1])) < 0) {
      fprintf(2, "%s: trace failed\n", argv[0]);
      exit(1);
    }
    
    for(i = 2; i < argc && i < MAXARG; i++){
      nargv[i-2] = argv[i];
    }
    exec(nargv[0], nargv);
    exit(0);

}
