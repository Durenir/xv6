// Create a zombie process that
// must be reparented at exit.

#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
//    if(fork() > 0)
//    {
//        int pid = getpid();
//        printf(1, "Pid is: %d", pid);
//        sleep(5);  // Let child exit before parent.
//    }
//    exit();
  int i, pid;
  char *argv[2];
  //char *argv2[2];
  //argv2[0] = "sh";
  //argv2[1] = 0;
  argv[0] = "pstree";
  argv[1] = 0;
  if(fork() == 0) //child of root parent
  {
    for(i = 0; i < 2; i++) //Fork 2 more children
    {
      pid = fork();
      if(pid < 0)
      {
        exit();
      } else if(pid == 0)
      {
        sleep(1);
        exec(*argv, argv);
      }
    }
    for(i = 0; i < 2; i++)
    {
      wait();
    }
  } else { //root parent
    wait();
  }
  exit();
}

