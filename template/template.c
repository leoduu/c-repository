#include <stdio.h>
#include "template.h"

max_impl(char)
max_impl(int)

int main()
{

    printf("int  %d\n", max(int)(3, 5));
    printf("char %d\n", max(char)('a', 'c'));

    return 0;
}