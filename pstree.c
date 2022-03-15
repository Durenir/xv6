#include "types.h"
#include "stat.h"
#include "user.h"
#include "uproc.h"

#define MAX 64

int
main(int argc, char *argv[])
{
    struct uproc *proc = malloc(MAX * sizeof(struct uproc));      //Array of processes.
    struct uproc *parents = malloc(MAX * sizeof(struct uproc));   //Array of parents.
    int processCount = getprocs(MAX, proc);                       //Returns the num processses and fills proc array.
    if(processCount == -1)                                        //-1 returned for failed proc retrieval.
    {
        printf(1, "Error: Could not get processes.\n");
        exit();
    }
    int i;                                                       //Loop control variable.
    int levels = -1;                                             //Parent stack pointer.
    for(i = 0; i < processCount; i++)
    {
        if(proc[i].pid == 1)                                     //Are we init?
        {
            printf(1, "%s[%d]\n", proc[i].name, proc[i].pid);    //Print the proc info.
            parents[++levels] = proc[i];                         //Push init onto the stack as the root.
            continue;
        }
        while(proc[i].ppid != parents[levels].pid)               //Pop the stack and find the parent.
        {
            levels--;
        }
        int j;                                                   //Loop control variable
        for(j = 0; j <= levels; j++)                             //Make indention to format the tree.
        {
            printf(1, "   ");
        }
        printf(1, "%s[%d]\n", proc[i].name, proc[i].pid);       //Print the proc info.
        if(proc[i+1].ppid == proc[i].pid)                       //Are we the parent of the next proc?
        {
            parents[++levels] = proc[i];                        //Push the proc onto the parent stack.
        }
    }
    exit();                                                     //exit
}

