#include "cgem/c/ADT/List/List.h"

#include <stdio.h>
#include <assert.h>

typedef struct TestList
{
    ListNode list;
    int      data;
} TestList;

int main()
{
    TestList t = {.list=LIST_NODE_INIT(t.list)};

    assert(List_isEmpty(&t.list));

    List_init(&t.list);

    assert(List_isEmpty(&t.list));// initialized with List_init

    for (int i = 0; i < 5; ++i)
    {
        //TestList tmp = (TestList*)malloc(sizeof(TestList));
        //List_init(&tmp->list);
        //List_add(&(tmp->list), &(test.list));
    }

    return 0;
}
