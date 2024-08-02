#include "list_algos.h"
#include "dbg.h"
#include "list.h"

void ListNode_swap(ListNode *cur, ListNode *next) {
    void *tmp_ptr = cur->value;
    cur->value = next->value;
    next->value = tmp_ptr;
}

int List_bubble_sort(List *list, List_compare cmp) {
    int sorted = 1;

    if (List_count(list) <= 1) {
        return 0;
    }

    do {
        sorted = 1;

        LIST_FOREACH(list, first, next, cur) {
            if (cur->next) {
                if (cmp(cur->value, cur->next->value) > 0) {
                    ListNode_swap(cur, cur->next);
                    sorted = 0;
                }
            }
        }
    } while (!sorted);

    return 0;
}

List *List_merge_sort(List *list, List_compare cmp) {

    return 0;
}
