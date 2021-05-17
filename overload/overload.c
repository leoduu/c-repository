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
typedef struct 
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RGB_t;

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

    RGB_t rgb;

    rgb = va_arg(va_ptr, RGB_t);

    printf("%d %d %d\n", rgb.r, rgb.g, rgb.b);

    // if (mode>=0 && mode <10)
    //     printf("open with mode:%d\n", mode);
    // else 
    //     printf("open without mode\n");

    va_end(va_ptr);
}


// receive variable types parameter 
 
void gcc_overload_int(int a)
{
    printf("get int: %d\n", a);
}
 
void gcc_overload_str(char *a)
{
    
    printf("get string: %s\n", a);
}
 
// warning: dereferencing type-punned pointer will break strict-aliasing rules
#define gcc_overload(A)\
    __builtin_choose_expr(__builtin_types_compatible_p(typeof(A), int),\
        gcc_overload_int((int)A),\
    __builtin_choose_expr(__builtin_types_compatible_p(typeof(A), char *),\
        gcc_overload_str((char*)A),(void)0))



int main()
{

    // my_printf("123 %s %c %d %x\n", "test", 'c', 49, 200);
    
    RGB_t rgb={100, 200, 250};

    // my_open("./test", 1);
    printf("in main %p\n", &rgb);
    my_open("./test", 1, rgb);

    // int a;
    // char *b;
    // a = 11;
    // b = "hello world";

    // gcc_overload(a);
    // gcc_overload(b);

    return 0;
}

