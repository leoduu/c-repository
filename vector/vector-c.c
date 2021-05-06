#include <stdio.h>
#include "vector-c.h"

vector_impl(int)

int main()
{   
    vector_init(int, v, 5);
    printf("vector init\n");
    printf("size:     %d\n", v.size(&v));
    printf("capacity: %d\n", v.capacity(&v));


    v.push_back(&v, 1);   
    v.push_back(&v, 2);  
    v.push_back(&v, 3);   
    v.push_back(&v, 4);   
    v.push_back(&v, 5);     
    printf("\nvector push_back\n");
    for (int *it=v.begin(&v); it!=v.end(&v); it++) {
        printf("%d ", *it);
    }
    printf("\nsize:     %d\n", v.size(&v));
    printf("capacity: %d\n", v.capacity(&v));


    v.pop_back(&v);
    v.pop_back(&v);
    v.pop_back(&v);
    printf("\nvector pop_back\n");
    for (int *it=v.begin(&v); it!=v.end(&v); it++) {
        printf("%d ", *it);
    }
    printf("\nsize:     %d\n", v.size(&v));
    printf("capacity: %d\n", v.capacity(&v));


    v.clear(&v);
    printf("\nvector clear\n");    
    printf("size:     %d\n", v.size(&v));
    printf("capacity: %d\n", v.capacity(&v));


    v.push_back(&v, 1);   
    v.push_back(&v, 2);   
    v.push_back(&v, 3);   
    v.push_back(&v, 4);   
    v.push_back(&v, 5);   
    v.push_back(&v, 6);   
    v.push_back(&v, 7);   
    printf("\nvector push_back and enlarge capacity\n");    
    for (int *it=v.begin(&v); it!=v.end(&v); it++) {
        printf("%d ", *it);
    }
    printf("\nsize:     %d\n", v.size(&v));
    printf("capacity: %d\n", v.capacity(&v));

    v.insert(&v, v.begin(&v)+3, 11);
    v.insert(&v, v.begin(&v)+3, 12);
    v.insert(&v, v.begin(&v)+3, 13);
    v.insert(&v, v.begin(&v)+3, 14);
    v.insert(&v, v.begin(&v)+3, 15);    
    printf("\nvector insert and enlarge capacity\n");    
    for (int *it=v.begin(&v); it!=v.end(&v); it++) {
        printf("%d ", *it);
    }
    printf("\nsize:     %d\n", v.size(&v));
    printf("capacity: %d\n", v.capacity(&v));

    v.erase(&v, v.begin(&v)+3); 
    v.erase(&v, v.begin(&v)+3); 
    v.erase(&v, v.begin(&v)+3); 
    v.erase(&v, v.begin(&v)+3); 
    v.erase(&v, v.begin(&v)+3); 
    printf("\nvector erase\n");    
    for (int *it=v.begin(&v); it!=v.end(&v); it++) {
        printf("%d ", *it);
    }
    printf("\nsize:     %d\n", v.size(&v));
    printf("capacity: %d\n", v.capacity(&v));

    
    v.reserve(&v);
    printf("\nvector reserve\n");
    for (int *it=v.begin(&v); it!=v.end(&v); it++) {
        printf("%d ", *it);
    }
    printf("\nsize:     %d\n", v.size(&v));
    printf("capacity: %d\n", v.capacity(&v));

    return 0;
}