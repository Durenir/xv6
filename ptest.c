#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char *argv[])
{
    int i, pid;        //Loop control variable and int to hold PID.
    double k, j;       //Loop control variable and a double for computation to delay process.
    char *argv2[2];    //Char pointer to hold the args for ps.
    argv2[0] = "ps";
    argv2[1] = 0;
    printf(1, "***************************************************************\n");
    printf(1, "* Warning: This test perfoms millions of computations. There  *\n");
    printf(1, "* will be slight delays.                                      *\n");
    printf(1, "***************************************************************\n\n\n");

    printf(1, "***************************************************************\n");
    printf(1, "* Test 1: Fork 2 children. Each child will do several million *\n");
    printf(1, "* computations. Child process 1 will have a priority level of *\n");
    printf(1, "* 10 to start and child process 2 will have a priority level  *\n");
    printf(1, "* of 180 to start. A third process is forked that executes ps *\n");
    printf(1, "* with a priority level of 0 so that it is executed before    *\n");
    printf(1, "* the cpu hog processes finish. Child process 1 should show   *\n");
    printf(1, "* it's state as the running process and child 2 should show   *\n");
    printf(1, "* it's state as runnable. i.e. the process with the highest   *\n");
    printf(1, "* priority should be the process that shows as running.       *\n");
    printf(1, "***************************************************************\n");
    setpriority(50);
    for(i = 0; i < 1; i++)
    {
        if((pid = fork()) == 0)              //Fork a new process.
        {
            setpriority(10);                   //Set the priority to 10.
            for(j = 0; j < 500000; j++)        //Perform some big computation to delay the process.
            {
                k = k * 2 + 11;
                setpriority(10);
            }
            printf(1, "If correct, PID %d should be the running process.\n", getpid());
            exit();
        } else if(pid < 0)                   //Did the fork fail?
        {
            printf(1, "Fork failed\n");        //If yes, tell the user and exit.
            exit();
        }
    }
    setpriority(50);
    for(i = 0; i < 1; i++)
    {
        if((pid = fork()) == 0)              //Fork another process to compete for CPU time.
        {
            setpriority(180);                  //Set the priority to 180.
            for(j = 0; j < 500000; j++)        //Perform some big computation to delay the process.
            {
                k = k * 2 + 11;
                setpriority(180);
            }
            printf(1, "If correct, PID %d should be the runnable process.\n", getpid());
            exit();
        } else if(pid < 0)                   //Did the fork fail?
        {
            printf(1, "Fork failed\n");        //If yes, tell the user and exit.
            exit();
        }
    }
    setpriority(50);
    if((pid = fork()) == 0)                //Fork once more.
    {
        for(j = 0; j < 75000; j++)           //Perform some big computation to delay the process.
        {
            k = k * 2 + 11;
        }
        setpriority(0);                      //Set the priority to 0.
        exec(*argv2, argv2);                 //Execute the ps program to show process info.
    } else if(pid < 0)                     //Did the fork fail?
    {
        printf(1, "Fork failed\n");          //If yes, tell the user and exit.
        exit();
    } else {
        wait();                              //Wait for the child executing ps to finish.
    }
    for(i = 0; i < 2; i++)                 //wait for/clean up the previous 2 children
    {
        wait();
    }
    printf(1, "\n\n***************************************************************\n");
    printf(1, "* Test 2: This test will check if the priority scheduler will *\n");
    printf(1, "* keep the priority level of a process between 0 and 200      *\n");
    printf(1, "* inclusive. Two processes will be forked. One child will     *\n");
    printf(1, "* invoke the setpriority system call with -10 and the second  *\n");
    printf(1, "* will invoke setpriority with 300. A third child will execute*\n");
    printf(1, "* the ps program. If correct, the ps program should show a    *\n");
    printf(1, "* process with a priority level of 0, and another with a 200. *\n");
    printf(1, "* Even after aging, they should still stay within the range of*\n");
    printf(1, "* 0-200 inclusive.                                            *\n");
    printf(1, "***************************************************************\n");
    setpriority(50);
    for(i = 0; i < 1; i++)
    {
        if((pid = fork()) == 0)             //Fork a new process.
        {
            setpriority(-10);                 //Set the priority to -10.
            for(j = 0; j < 10000000; j++)     //Perform some big computation to delay the process.
            {
                k = k * 2 * 11;
            }
            printf(1, "Argument for PID %d priority level was -10. If correct, should be >= 0.\n", getpid());
            exit();
        } else if(pid < 0)                  //Did fork fail?
        {
            printf(1, "Fork failed\n");       //If yes, inform the user and exit.
            exit();
        }
    }
    setpriority(50);
    for(i = 0; i < 1; i++)
    {
        if((pid = fork()) == 0)             //Fork a competing process.
        {
            setpriority(300);                 //Set the priority to 300.
            for(j = 0; j < 10000000; j++)     //Perform some big computation to delay the process.
            {
                k = k * 2 * 11;
            }
            printf(1, "Argument for PID %d priority level was 300. If correct, should be <= 200.\n", getpid());
            exit();
        } else if(pid < 0)                  //Did fork fail?
        {
            printf(1, "Fork failed\n");       //If yes, inform the user and exit.
            exit();
        }
    }
    if((pid = fork()) == 0)               //Fork once more.
    {
        for(j = 0; j < 75000; j++)           //Perform some big computation to delay the process.
        {
            k = k * 2 + 11;
        }
        setpriority(0);                     //Set the child priority to 0.
        exec(*argv2, argv2);                //Execute the ps program.
    } else if(pid < 0)                    //Did fork fail?
    {
        printf(1, "Fork failed\n");         //If yes, inform the user and exit.
        exit();
    } else {
        wait();                             //Wait for child to finish.
    }
    for(i = 0; i < 2; i++)                //Wait for/clean up the previous two children.
    {
        wait();
    }
    exit();                               //End of user program.
}

