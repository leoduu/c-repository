#include <stdlib.h>
#include <assert.h>

typedef unsigned int u32;

#define _vec_add        5
#define _vec(T)         _vec_##T
#define _node(T)        _node_##T
#define _vec_init(T)    _vec_init_##T
#define _push_back(T)   _push_back_##T
#define _pop_back(T)    _pop_back_##T
#define _front(T)       _front_##T
#define _back(T)        _back_##T
#define _at(T)          _at_##T
#define _size(T)        _size_##T
#define _capacity(T)    _capacity_##T
#define _begin(T)       _begin_##T
#define _end(T)         _end_##T
#define _insert(T)      _insert_##T
#define _erase(T)       _erase_##T
#define _clear(T)       _clear_##T

#define vector_init(T, v, size)         \
_vec(T) v;                              \
_vec_init(T)(&v, size) 

#define vector_impl(T)                                          \
typedef struct _node(T) {                                       \
    T    *start;                                                \
    T    *finish;                                               \
    u32  _capacity;                                             \
                                                                \
    void (*push_back)(struct _node(T) *, T);                    \
    void (*pop_back) (struct _node(T) *);                       \
    T    (*front)    (struct _node(T) *);                       \
    T    (*back)     (struct _node(T) *);                       \
    T    (*at)       (struct _node(T) *, u32);                  \
    u32  (*size)     (struct _node(T) *);                       \
    u32  (*capacity) (struct _node(T) *);                       \
    T    *(*begin)   (struct _node(T) *);                       \
    T    *(*end)     (struct _node(T) *);                       \
    T    *(*insert)  (struct _node(T) *, T *, T);               \
    T    *(*erase)   (struct _node(T) *, T *);                  \
    void (*clear)    (struct _node(T) *);                       \
} _vec(T);                                                      \
                                                                \
void _push_back(T)(_vec(T) *this, T value) {                    \
    u32 size = this->size(this);                                \
    if (size == this->_capacity) {                              \
        T* p = realloc(this->start,sizeof(T)*(size+_vec_add));  \
        assert(p);                                              \
        this->start = p;                                        \
        this->finish = p+size;                                  \
        this->_capacity += _vec_add;                            \
    }                                                           \
    *(this->finish++) = value;                                  \
}                                                               \
                                                                \
void _pop_back(T)(_vec(T) *this) {                              \
    if (this->finish > this->start)                             \
        this->finish--;                                         \
}                                                               \
                                                                \
T _front(T)(_vec(T) *this) {                                    \
    return *(this->start);                                      \
}                                                               \
                                                                \
T _back(T)(_vec(T) *this) {                                     \
    return *(this->finish);                                     \
}                                                               \
                                                                \
T _at(T)(_vec(T) *this, u32 n) {                                \
    return *(this->start + n);                                  \
}                                                               \
                                                                \
u32 _size(T)(_vec(T) *this) {                                   \
    return this->finish - this->start;                          \
}                                                               \
                                                                \
u32 _capacity(T)(_vec(T) *this) {                               \
    return this->_capacity;                                     \
}                                                               \
                                                                \
T *_begin(T)(_vec(T) *this) {                                   \
    return this->start;                                         \
}                                                               \
                                                                \
T *_end(T)(_vec(T) *this) {                                     \
    return this->finish;                                        \
}                                                               \
                                                                \
T *_insert(T)(_vec(T) *this, T*pos, T value) {                  \
    u32 size = this->size(this);                                \
    if (size == this->_capacity) {                              \
        T* p = realloc(this->start,sizeof(T)*(size+_vec_add));  \
        assert(p);                                              \
        this->start = p;                                        \
        this->finish = p+size;                                  \
        this->_capacity += _vec_add;                            \
    }                                                           \
    for (T *it=this->finish; it!=pos; it--) {                   \
        *it = *(it-1);                                          \
    }                                                           \
    this->finish++;                                             \
    *pos = value;                                               \
    return pos;                                                 \
}                                                               \
                                                                \
T *_erase(T)(_vec(T) *this, T *pos) {                           \
    for (T *it=pos; it!=this->finish; it++) {                   \
        *it = *(it+1);                                          \
    }                                                           \
    this->finish--;                                             \
    return pos;                                                 \
}                                                               \
                                                                \
void _clear(T)(_vec(T) *this) {                                 \
    this->finish = this->start;                                 \
}                                                               \
                                                                \
void _vec_init(T)(_vec(T) *this, u32 size) {                    \
    this->start     = (T *)malloc(sizeof(T)*size);              \
    this->finish    = this->start;                              \
    this->_capacity  = size;                                    \
    this->push_back = _push_back(T);                            \
    this->pop_back  = _pop_back(T);                             \
    this->front     = _front(T);                                \
    this->back      = _back(T);                                 \
    this->at        = _at(T);                                   \
    this->capacity  = _capacity(T);                             \
    this->size      = _size(T);                                 \
    this->begin     = _begin(T);                                \
    this->end       = _end(T);                                  \
    this->insert    = _insert(T);                               \
    this->erase     = _erase(T);                                \
    this->clear     = _clear(T);                                \
}


