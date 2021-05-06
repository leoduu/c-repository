// template example
#define max(T) _max_##T

#define max_impl(T)     \
T max(T)(T a, T b) {    \
    return a>b?a:b;     \
}   

/* You need to declare for a certain type.
 *    max_impl(int)
 *
 * function reference: max(int)(1, 2)
 *     
 */



