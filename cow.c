#include "types.h"
#include "user.h"

int aaa = 0;
int bbb;

void test_var_change()
{
    int pid = fork();
    if (pid == 0)
    {
        int free_pages = get_num_free_pages();
        aaa = 1;
        assert("free pages after change < initial free pages", get_num_free_pages() < free_pages);
        exit();
    }
    else
    {
        wait();
    }
}

void test_fork()
{
    int ptaddr_of_aaa_parent = get_page_table_address_of(&aaa);
    int pid = fork();
    if (pid == 0)
    {
        int ptaddr_from_child = get_page_table_address_of(&aaa);
        assert("page table of (aaa) from parent process == page table of (aaa) from child process", ptaddr_of_aaa_parent == ptaddr_from_child);
        printf(1, "aaa == %d\n", aaa);
        aaa = 777;
        printf(1, "aaa == %d\n", aaa);
        assert("page table changed after variable modification", get_page_table_address_of(&aaa) != ptaddr_from_child);
        exit();
    }
    else
    {
        wait();
    }
}

int main()
{
    test_var_change();
    test_fork();
    exit();
}