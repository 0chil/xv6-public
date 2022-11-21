#include "types.h"
#include "user.h"

int main(int argc, char *argv[])
{
    int pid = getpid();
    int priority = atoi(argv[1]);
    priority = set_proc_priority(pid, priority);
    printf(1, "pid (%d): loop started, with priority (%d)\n", pid, priority);
    while (1)
        ;
}