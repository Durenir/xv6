#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "uproc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//Fill passed in uproc struct array with processes info.
//Return the number of processes in the array.
int
sys_getprocs(void)
{
  int max;                  //Holds the max procs retrieved.
  struct uproc *p;          //Struct array to be filled with proccess data.
  if(argint(0, &max) < 0)   //Get the passed in arg for max.
    return -1;              //If argint returns -1, failed to retrieve arg.
  if(argptr(1, (char **)&p, max * sizeof(struct uproc)) < 0)  //Get the passed in struct ptr.
    return -1;              //If argptr returns -1, failed to retrieve ptr.
  return getprocs(p);       //Call getprocs in proc.c and return the returned value.
}

int
sys_setpriority(void)
{
  int newpriority;                              //Holds the passed in new priority
  int oldpriority;                              //Holds the old priority
  if(argint(0, &newpriority) < 0)               //Fetch the argument for the priority
    return -1;
  if(newpriority > 200)                         //Is the argument greater than 200?
    newpriority = 200;                          //If yes, cap it at 200
  else if(newpriority < 0)                      //Is the argument less than 0?
    newpriority = 0;                            //If yes, cap it at 0
  oldpriority = myproc()->priority;             //Save the old priority
  myproc()->priority = newpriority;             //Set the current priority to the new priority
  if(newpriority > oldpriority)                 //Is the new priority less than the current priority?
    yield();                                    //Yes? Then yield and resched
  return oldpriority;                           //Return the old priority
}

int
sys_cps(void)
{
  return cps();                                 //Call cps to print the current process states.
}
