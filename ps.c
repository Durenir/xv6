#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char *argv[])
{
  if(argc > 1)
  {
    if(setpriority(atoi(argv[1])) < 0)
    {
      printf(1, "Error: Could not update priority.\n");
      exit();
    }
  }
  cps();
  exit();
}
