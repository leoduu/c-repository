/*
 * @Author: your name
 * @Date: 2021-04-24 15:19:39
 * @LastEditTime: 2021-05-07 20:29:35
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \leetcode\b.c
 */
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

// printf 
void my_printf(const char *fmt, ...)
{
    va_list va_ptr; 
    int value, i;
    char *str = NULL;
    char buff[20];    
	const char *digits="0123456789ABCDEF";

    va_start(va_ptr, fmt);

    while (*fmt != '\0') {

        if (*fmt == '%') {
            switch (*(++fmt)) {
            case 'd':
                value = va_arg(va_ptr, int);
                i = 0;
                do {
                    buff[i++] = value % 10;
                    value /= 10;
                } while (value);
                while(i) putchar(digits[buff[--i]]);
                break;   
            
            case 'c':
                putchar(va_arg(va_ptr, int));
                break;

            case 's':
                str = va_arg(va_ptr, char*);
                while (*str) putchar(*str++);
                break;
            
            case 'x':
                value = va_arg(va_ptr, int);
                i = 0;
                while (value) {
                    buff[i++] = value & 0xF;
                    value >>= 4;
                }
                while(i) putchar(digits[buff[--i]]); 
                break;

            default:
                break;
            }
        }
        else {
            putchar(*fmt);
        }
        ++fmt;
    }
    va_end(va_ptr);    
}


// receive variable length parameter
void my_open(const char *path, int flag, ...)
{   
    va_list va_ptr;
    int mode;
    
    va_start(va_ptr, flag);

    mode = va_arg(va_ptr, int);

    if (mode>=0 && mode <10)
        printf("open with mode:%d\n", mode);
    else 
        printf("open without mode\n");

    va_end(va_ptr);
}


// receive variable types parameter 
 
void gcc_overload_int(int a)
{
    printf("overload_int: %d\n", a);
}
 
void gcc_overload_str(char *a)
{
    
    printf("overload_char[]: %s\n", a);
}
 
// warning: dereferencing type-punned pointer will break strict-aliasing rules
#define gcc_overload(A)\
    __builtin_choose_expr(__builtin_types_compatible_p(typeof(A), int),\
        gcc_overload_int((int)A),\
    __builtin_choose_expr(__builtin_types_compatible_p(typeof(A), char[]),\
        gcc_overload_str((char*)A),\
    (void)0))



int main()
{

    my_printf("123 %s %c %d %x\n\n", "test", 'c', 49, 200);

    int c = 11;
    typeof(c) d;

    my_open("./test", 1);
    my_open("./test", 1, 2);

    printf("\n");
    int a = 11;
    char b[] = "hello world";

    gcc_overload(a);
    gcc_overload(b);
 
    return 0;
}

