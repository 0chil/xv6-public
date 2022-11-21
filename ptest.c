#include "types.h"
#include "user.h"

void test_get_and_set_priority()
{
    printf(1, "- Test Get and Set Priority\n");
    assert("pid 1 (initial) priority == 5?", get_proc_priority(1) == 5);
    assert("pid 2 (initial) priority == 5?", get_proc_priority(2) == 5);

    assert("pid 0 set_priority(4) == 4?", set_proc_priority(0, 4) == 4);
    assert("pid 1 set_priority(3) == 3?", set_proc_priority(1, 3) == 3);

    assert("pid 0 priority == 4?", get_proc_priority(0) == 4);
    assert("pid 1 priority == 3?", get_proc_priority(1) == 3);
    printf(1, "Test Done\n\n");
}

void test_fork_copies_priority()
{
    printf(1, "- Test Fork copies priority\n");
    int parent_pid = getpid();
    assert("set parent(%d) priority=3 == 3?", set_proc_priority(parent_pid, 3) == 3, parent_pid);

    int pid = fork();
    if (pid > 0)
    {
        assert("child(%d) priority == 3?", get_proc_priority(pid) == 3, pid);
        wait();
        printf(1, "Test Done\n\n");
    }
    if (pid == 0)
    {
        exit();
    }
}

int exec_loop_with_priority(char *priority)
{
    int pid = fork();
    if (pid > 0)
    {
        set_proc_priority(pid, atoi(priority));
    }
    if (pid == 0)
    {
        char *args[] = {"loop", priority, 0};
        exec(args[0], args);
    }
    return pid;
}

void test_starvation()
{
    printf(1, "- Test Starvation\n");
    int parent_pid = getpid();
    assert("set parent priority(5) == 5?", set_proc_priority(parent_pid, 5) == 5); // 부모 Priority를 5로 설정한다.
    int child = exec_loop_with_priority("4"); // Priority==4 인 자식 무한루프 RUNNABLE을 만든다.
    int child2 = exec_loop_with_priority("3"); // Priority==3 인 자식 무한루프 RUNNABLE을 만든다.
    sleep(10); // 자식 RUNNABLE -> RUNNING이 되도록 충분한 사이클을 쉰다.
    sleep(10); // CPU 2개 == 스케쥴러 2개이다.
    // 남는 CPU가 다시 부모 실행을 시작해도 다시 Sleep을 걸어 child2 까지 마저 실행시킨다.

    // 기아현상이 있다면 아래 코드는 실행되지 않고, 오직 자식만 실행된다.
    kill(child); // 기아 현상이 없다면 자식 프로세스를 종료하는 코드가 실행된다.
    kill(child2);
    wait(); // 종료를 기다린다.
    wait();

    assert("no starvation", 1, child, child2); // 기아 현상이 없는 경우 최종 실행되는 코드
}

int main(int argc, char *argv[])
{
    test_get_and_set_priority();

    test_fork_copies_priority();

    test_starvation();
    exit();
}