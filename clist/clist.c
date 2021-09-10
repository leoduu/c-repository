#include <stdio.h>
#include "clist.h"

int main() 
{
    clist_create(list);

    printf("clist init\n");

    printf("empty: %s\n", clist_empty(list)?"true":"false");
    for (int i=0; i<5; i++) {
        clist_push_back(list, i);
        printf("push_back: %d\n", i);
    }
    printf("empty: %s\n", clist_empty(list)?"true":"false");

    clist_printf(list);
    printf("front: %d\n", clist_front(list));
    printf("back : %d\n", clist_back(list));
    printf("\n\n");

    clist_traversal(node, list) {
        if (node->value <= 2) {
            clist_insert(list, node, 7);
            printf("insert %d, after %d\n", 7, node->value);
        }
    }
    
    clist_printf(list);
    printf("front: %d\n", clist_front(list));
    printf("back : %d\n", clist_back(list));
    printf("\n\n");

    clist_traversal(node, list) {
        if (node->value == 7) {
            printf("remove %d, after %d\n", 7, node->prev->value);
            node = clist_remove(list, &node);
        }
    }
    
    clist_printf(list);
    printf("front: %d\n", clist_front(list));
    printf("back : %d\n", clist_back(list));
    printf("\n\n");

    clist_pop_front(list);
    clist_pop_end(list);
    clist_pop_end(list);
    clist_pop_front(list);

    clist_printf(list);
    printf("front: %d\n", clist_front(list));
    printf("back : %d\n", clist_back(list));

    return 0;
}
