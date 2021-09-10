#ifndef __LIST_HH__
#define __LIST_HH__

#include <malloc.h>
#include <stdio.h>

int _i_i; //for count

struct _cnode {
    struct _cnode *prev;
    struct _cnode *next;
    int value;
};

struct _clist{
    struct _cnode *head;
    struct _cnode *tail;
    int size;
};

#define clist_create(clist)                                         \
    struct _clist *clist;                                           \
    clist = (struct _clist *)malloc(sizeof(struct _clist));         \
    clist->head = clist->tail = NULL;                               \
    clist->size = 0;                                                \
                                                   
#define clist_traversal(iterator, clist)                            \
    _i_i = 0;                                                       \
    for (struct _cnode *iterator=clist->head;                       \
         _i_i<clist->size; iterator=iterator->next, ++_i_i)


/**********************************************************
 *          private
 *              
**********************************************************/
void _clist_push_empty(struct _clist *clist, int value) 
{
    clist->head = (struct _cnode *)malloc(sizeof(struct _cnode));
    clist->head->next = clist->head;
    clist->head->prev = clist->head;
    clist->head->value = value;

    clist->tail = clist->head;
}

struct _cnode *_clist_push_next(struct _cnode **cnode, int value) 
{
    struct _cnode *temp = (struct _cnode *)malloc(sizeof(struct _cnode));
    temp->value = value;
    temp->prev = *cnode;
    temp->next = (*cnode)->next;
    
    (*cnode)->next->prev = temp;
    (*cnode)->next = temp;

    return temp;
}

static inline void _clist_remove(struct _cnode *cnode) 
{
    cnode->prev->next = cnode->next;
    cnode->next->prev = cnode->prev;
}

static inline void _clist_reset(struct _clist *clist) 
{
    clist->head = clist->tail = NULL;
    clist->size = 0;
}

/**********************************************************
 *          public
 *          information
**********************************************************/

static inline int clist_size(struct _clist *clist) {
    return clist->size;
}

static inline int clist_front(struct _clist *clist) {
    return clist->head->value;
}

static inline int clist_back(struct _clist *clist) {
    return clist->tail->value;
}

static inline int clist_empty(struct _clist *clist) {
    return clist->size?0:1;
}

void clist_printf(struct _clist *clist) 
{
    printf("clist size: %d >", clist->size);
    clist_traversal(cnode, clist) {
        printf(" %d", cnode->value);
    }
    printf("\n");
}

/**********************************************************
 *          public
 *          operation
**********************************************************/

void clist_push_front(struct _clist *clist, int value) 
{

    if (clist->size == 0) 
        _clist_push_empty(clist, value);
    else 
        clist->head = _clist_push_next(&clist->tail, value);

    ++clist->size;
}

void clist_push_back(struct _clist *clist, int value) 
{
    if (clist->size == 0) 
        _clist_push_empty(clist, value);
    else 
        clist->tail = _clist_push_next(&clist->tail, value);
    
    ++clist->size;
}

void clist_insert(struct _clist *clist, struct _cnode *cnode, int value) 
{
    if (cnode == clist->tail) 
        clist_push_back(clist, value);
    else {
        _clist_push_next(&cnode, value);
        ++clist->size;
    }
}

void clist_pop_front(struct _clist *clist) 
{
    if (clist->size == 0) 
        return;
    
    if (clist->size == 1) {
        free(clist->head);
        _clist_reset(clist);
        return;
    }

    struct _cnode *temp = clist->head->next;
    _clist_remove(clist->head);        
    free(clist->head);
    clist->head = temp;
    --clist->size;
}

void clist_pop_end(struct _clist *clist) 
{
    if (clist->size == 0) 
        return;
    
    if (clist->size == 1) {
        free(clist->tail);
        _clist_reset(clist);
        return;
    }

    struct _cnode *temp = clist->tail->prev;
    _clist_remove(clist->tail);    
    free(clist->tail);
    clist->tail = temp;    
    --clist->size;
}

struct _cnode *clist_remove(struct _clist *clist, struct _cnode **pnode) 
{
    if (*pnode == clist->head) {
        clist_pop_front(clist);
        return clist->head;
    }
    if (*pnode == clist->tail) {
        clist_pop_end(clist);
        return clist->tail;
    }

    struct _cnode *temp = (*pnode)->prev;
    _clist_remove(*pnode);     
    free(*pnode); 
    --clist->size;
    return temp;
}

#endif
