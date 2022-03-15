#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char *argv[])
{
  int i, k, pid;                        //Loop control and pid variables.
  printf(1, "Parent should print evens before 4 children print odds.\n");
  printf(1, "Parent priority is 0. Children priority is 200.\n");
  for(i = 0; i < 4; i++)                
  {
    if((pid = fork()) == 0)             //Fork a child.
    {
      setpriority(200);                 //Set child priority to 200.
      for(k = 0; k < 6; k++)
      {
        if((k % 2) == 1)                //Is k odd?
          printf(1, "%d\n", k);         //If yes, print k.
      }
      exit();                           //Kill the child process.
    }
    else if(pid < 0)                    //Error forking.
    {
      printf(1, "Fork failed.\n");
      exit();
    }
  }
  setpriority(0);                       //Set parent priority to 0.
  for(k = 0; k < 6; k++)
  {
    if((k % 2) == 0)                    //Is k even?
      printf(1, "%d\n", k);             //If yes, print k.
  }
  for(i = 0; i < 4; i++)                //Clean up the dead processes.
  {
    wait();
  }
  printf(1, "\nSwapping priorities...\n");
  printf(1, "A couple children should finish before parent starts.\n");
  printf(1, "Parent priority is 200. Children priority is 0.\n");
  for(i = 0; i < 4; i++)
  {
    if((pid = fork()) == 0)             //Fork a child.
    {
      setpriority(0);                   //Set the child priority to 0.
      for(k = 0; k < 6; k++)            
      {
        if((k % 2) == 1)                //Is k odd?
          printf(1, "%d\n", k);         //If yes, print k.
      }
      exit();                           //Kill the child process.
    }
    else if(pid < 0)                    //Error forking.
    {
      printf(1, "Fork failed.\n");
      exit();
    }
  }
  setpriority(200);                     //Set the parent priority to 200.
  for(k = 0; k < 6; k++)
  {
    if((k % 2) == 0)                    //Is k even?
      printf(1, "%d\n", k);             //If yes, print k.
  }
  for(i = 0; i < 4; i++)                //Clean up the dead processes.
  {
    wait();
  }
  exit();                               //End the user program.
}
