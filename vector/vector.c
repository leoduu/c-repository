#include <stdio.h>
#include "vector.h"
#include <time.h>

vector_impl(int)

void spend_time(void)
{
    clock_t t1, t2;
    double t;
    vector_init(int, v, 5);

    printf("\n---spend time test---\n");

    t1 = clock();
    for (u32 i=0; i<10000; i++) v.push_back(&v, i);
    t2 = clock();
    t = (double)(t2-t1) / CLOCKS_PER_SEC * 1000;
    printf("push_back %.3lfms\n", t);


    t1 = clock();
    while (v.size(&v)) v.pop_back(&v);
    t2 = clock();
    t = (double)(t2-t1) / CLOCKS_PER_SEC * 1000;
    printf("pop_back %.3lfms\n", t);

}

void operation_test(void)
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
}

int main()
{   
    //operation_test();

    spend_time();

    return 0;
}