#include <malloc.h>

#ifndef __SAMRT_PTR_H__
#define __SAMRT_PTR_H__

#define destruct_unique __attribute__ ((cleanup(unique_ptr_free)))
#define uinque_ptr(T) destruct_unique T

static void unique_ptr_free(void *ptr)
{
    void **p = (void **)ptr;
    free(*p);
}



#define _sptr_free(T)   share_ptr_free_##T  
#define _pinfo(T)        pinfo_##T  
#define _sptr(T)        share_ptr_##T
#define _swap(T)        swap_##T
// function
#define _get(T)             get_##T
#define _use_count(T)       use_count_##T
#define _reset(T)           reset_##T
#define _reset_to(T)        reset_to_##T
#define _share_create(T)    share_ptr_create_##T
#define _share_init(T)      share_ptr_init_##T

#define share_ptr_create(T, p, _p)                              \
__attribute__ ((cleanup(_sptr_free(T)))) struct _sptr(T) p;     \
_share_create(T)(&p, _p);

#define share_ptr_init(T, p, _p)                                \
__attribute__ ((cleanup(_sptr_free(T)))) struct _sptr(T) p;     \
_share_init(T)(&p, &_p);

#define share_ptr_empty(T, p)                                   \
__attribute__ ((cleanup(_sptr_free(T)))) struct _sptr(T) p;     \
_share_init(T)(&p, (struct _sptr(T)*)0u);                       \

#define share_ptr_impl(T)                                       \
struct _pinfo(T) {                                              \
    int cnt;                                                    \
    T *ptr;                                                     \
};                                                              \
                                                                \
struct _sptr(T) {                                               \
    struct _pinfo(T) *pinfo;                                    \
                                                                \
    T    *(*get)        (struct _sptr(T) *);                    \
    int  (*use_count)   (struct _sptr(T) *);                    \
    void (*reset)       (struct _sptr(T) *);                    \
    void (*reset_to)    (struct _sptr(T) *, struct _sptr(T));   \
};                                                              \
                                                                \
static inline T *_get(T)(struct _sptr(T) *this) {               \
    if (this->pinfo) return this->pinfo->ptr;                   \
    else return NULL;                                           \
}                                                               \
                                                                \
static inline int _use_count(T)(struct _sptr(T) *this) {        \
    if (this->pinfo) return this->pinfo->cnt;                   \
    else return -1;                                             \
}                                                               \
                                                                \
static inline void _reset(T)(struct _sptr(T) *this) {           \
    this->pinfo = NULL;                                         \
}                                                               \
                                                                \
static inline void _reset_to(T)(struct _sptr(T) *this, struct _sptr(T) p) { \
    this->pinfo = p.pinfo;                                      \
    ++this->pinfo->cnt;                                         \
}                                                               \
                                                                \
void _swap(T)(struct _sptr(T) *p1, struct _sptr(T) *p2) {       \
    struct _pinfo(T) *temp = p1->pinfo;                         \
    p1->pinfo = p2->pinfo;                                      \
    p2->pinfo = temp;                                           \
}                                                               \
                                                                \
void _share_create(T)(struct _sptr(T) *this, T *p){             \
    this->pinfo = (struct _pinfo(T) *)malloc(sizeof(struct _pinfo(T))); \
    this->pinfo->cnt = 1;                                       \
    this->pinfo->ptr = p;                                       \
    /* fucntion */                                              \
    this->get       = _get(T);                                  \
    this->use_count = _use_count(T);                            \
    this->reset     = _reset(T);                                \
    this->reset_to  = _reset_to(T);                             \
}                                                               \
                                                                \
void _share_init(T)(struct _sptr(T) *this, struct _sptr(T) *p){ \
    if (p == 0) this->pinfo = NULL;                             \
    else {                                                      \
        this->pinfo = p->pinfo;                                 \
        ++this->pinfo->cnt;                                     \
    }                                                           \
    /* fucntion */                                              \
    this->get       = _get(T);                                  \
    this->use_count = _use_count(T);                            \
    this->reset     = _reset(T);                                \
    this->reset_to  = _reset_to(T);                             \
}                                                               \
                                                                \
static void _sptr_free(T)(void *ptr) {                          \
    struct _pinfo(T) **p = (struct _pinfo(T) **)ptr;            \
    printf("addr p :%p free\n", (*p)->ptr);                     \
    if (--(*p)->cnt == 0) free(*p);                             \
}               

#endif
