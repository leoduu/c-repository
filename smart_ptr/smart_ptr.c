#include <smart_ptr.h>

share_ptr_impl(int)

int main()
{
    {
        uinque_ptr(int*) p = NULL;
        p = (int*)malloc(sizeof(int));
        *p = 10;
    }

    {
        int a = 20;  
        printf("addr a :%p\n", &a);      
        share_ptr_create(int, p1, &a);
        {
            share_ptr_init(int, p2, p1);
            {
                share_ptr_empty(int, p3);                
                p3.reset_to(&p3, p2);
                printf("addr p3:%p  cnt:%d\n", p3.get(&p3), p3.use_count(&p3));   
            }
            printf("addr p2:%p  cnt:%d\n", p2.get(&p2), p2.use_count(&p2)); 
        }
        printf("addr p1:%p  cnt:%d\n", p1.get(&p1), p1.use_count(&p1)); 
    }
}
