#include "cgem/c/ADT/List/List.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef struct TestList
{
    ListNode list1;
    int      data;
    ListNode list2;
} TestList;

int main()
{
    TestList t = {
        .list1=LIST_NODE_INIT(t.list1),
        .list2=LIST_NODE_INIT(t.list2),
        .data = -1
    };

    assert(List_isEmpty(&t.list1));

    List_init(&t.list1);
    List_init(&t.list2);

    assert(List_isEmpty(&t.list1));// initialized with List_init

    LIST_HEAD(list1);
    LIST_HEAD(list2);

    assert(List_isEmpty(&list1));

    for (int i = 0; i < 5; ++i)
    {
        TestList* tmp = (TestList*)malloc(sizeof(TestList));
        List_init(&tmp->list1);
        List_init(&tmp->list2);
        tmp->data = i;

        List_add(&list1, &tmp->list1);
        List_insertBefore(&list2, &tmp->list2);
    }

    // iter list#1        list#2
    //   0  h-0           0-h
    //   1  h-1-0         0-1-h
    //   2  h-2-1-0       0-1-2-h
    //   3  h-3-2-1-0     0-1-2-3-h
    //   4  h-4-3-2-1-0   0-1-2-3-4-h

    ListNode* iterator;
    int count = 4;

    List_forEach(&list1, iterator) {
        TestList* entry = List_entry(iterator, TestList, list1);
        printf(" %d", entry->data);
        assert(entry->data == count);
        --count;
    }
    printf("\n");

    count = 4;
    List_forEachReverse(&list2, iterator) {
        TestList* entry = List_entry(iterator, TestList, list2);
        printf(" %d", entry->data);
        assert(entry->data == count);
        --count;
    }
    printf("\n");

    count = 4;
    TestList* entry = NULL;
    List_forEachEntry(&list1, list1, entry) {
        printf(" %d", entry->data);
        assert(entry->data == count);
        --count;
    }
    printf("\n");

    entry = NULL;
    TestList* saver;
    List_forEachEntrySafe(&list1, list1, entry, saver) {
        printf(" %d", entry->data);
        List_delete(&entry->list1);
        free(entry);
    }
    printf("\n");

    return 0;
}
