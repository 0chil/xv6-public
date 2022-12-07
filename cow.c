#include "types.h"
#include "user.h"

int aaa = 0;

void test_cow_global_var()
{
    int initial_free_pages = get_num_free_pages();
    printf(1, "initial free pages: %d\n", initial_free_pages);
    int pid = fork();
    if (pid == 0)
    {
        printf(1, "child free pages: %d\n", get_num_free_pages());
        aaa = 1;
        assert("free pages after change < initial free pages", get_num_free_pages() < initial_free_pages);
        exit();
    }
    else
    {
        wait();
        assert("free pages after child process done: %d\n", initial_free_pages == get_num_free_pages());
    }
}

void test_cow_va_pa()
{
    printf(1, "parent va(aaa): %p\n", &aaa);
    int pid = fork();
    if (pid == 0)
    {
        printf(1, "child va(aaa): %p\n", &aaa);
        aaa = 1;
        printf(1, "child va(aaa) after change: %p\n", &aaa);
        exit();
    }
    else
    {
        wait();
    }
}

int main()
{
    // printf(1, "get_num_free_pages: %d\n", get_num_free_pages());
    test_cow_global_var();
    test_cow_va_pa();
    exit();
}