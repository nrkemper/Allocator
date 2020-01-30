//
//  blockdll.c
//  Allocator Attempt 2
//
//  Created by Nicholas Kemp on 2020-01-18.
//  Copyright © 2020 Nicholas Kemp. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "blockdll.h"
#include "memzone.h"
#include "block.h"
#include "sys.h"

void __blockdll_push (struct blockdll* dll, struct blocknode* b)
{
    struct blocknode*   head = dll->head;
    
    if (head) {
        head->prev  = b;
        b->next     = head;
        dll->head   = b;
    } else {
        dll->head   = dll->tail = b;
        b->next     = (struct blocknode*)0;
    }
    
    b->prev         = (struct blocknode*)0;
    b->dll          = dll;
}


void __blockdll_append (struct blockdll* dll, struct blocknode* b)
{
    struct blocknode*   tail = dll->tail;
    
    if (tail) {
        tail->next  = b;
        b->prev     = tail;
        dll->tail   = b;
    } else {
        dll->head   = dll->tail = b;
        b->prev     = (struct blocknode*)0;
    }
    
    b->next         = (struct blocknode*)0;
    b->dll          = dll;
}

void __blockdll_insert (struct blockdll* dll, struct blocknode* des, struct blocknode* b)
{
    struct blocknode*   head, *tail;
    
    if (dll == (struct blockdll*)0)
        return;
    
    if (b == (struct blocknode*)0)
        return;
    
    if (des == (struct blocknode*)0)
        des = dll->head;
    else if (dll != des->dll)
        return;
    
    head = dll->head;
    tail = dll->tail;
    
    if (head != (struct blocknode*)0) {
        if (des != head &&
            des != tail) {
            b->next         = des;
            b->prev         = des->prev;
            des->prev->next = b;
            des->prev       = b;
        } else if (des == head &&
                   des != tail) {
            b->next         = des;
            b->prev         = (struct blocknode*)0;
            des->prev       = b;
            dll->head       = b;
        } else if (des != head &&
                   des == tail) {
            b->next         = des;
            b->prev         = des->prev;
            des->prev->next = b;
            des->prev       = b;
        } else if (des == head &&
                     des == tail) {
            b->next         = des;
            b->prev         = (struct blocknode*)0;
            des->prev       = b;
            dll->head       = b;
        }
    }
    else {
        dll->head = dll->tail = b;
        b->prev = b->next = (struct blocknode*)0;
    }
    
    b->dll          = dll;
}

struct blocknode* __blockdll_pop (struct blockdll* dll)
{
    struct blocknode*   head = dll->head;
    
    if (head) {
        dll->head           = dll->head->next;
        
        if (dll->head)
            dll->head->prev = (struct blocknode*)0;
    }
    
    head->dll   = (struct blockdll*)0;
    
    return head;
}

struct blocknode* __blockdll_remove (struct blocknode* b)
{
    struct blockdll*    dll = b->dll;
    
    if (b != dll->head &&
        b != dll->tail) {
        
        b->prev->next   = b->next;
        b->next->prev   = b->prev;
        
    } else if (b == dll->head &&
               b != dll->tail) {
        
        b->next->prev   = (struct blocknode*)0;
        dll->head       = b;
        
    } else if (b != dll->head &&
               b == dll->tail) {
        
        b->prev->next   = (struct blocknode*)0;
        dll->tail       = b;
        
    } else if (b == dll->head &&
               b == dll->tail) {
        
        dll->head = dll->tail = (struct blocknode*)0;
        
    }
    
    b->dll   = (struct blockdll*)0;
    
    return b;
}

struct blocknode* __blockdll_grab_spare_node (void)
{
    if (sparenodes.head == (struct blocknode*)0)
        __blockdll_alloc_spare_nodes(SPARE_NODE_INCREASE_AMOUNT);
    
    return __blockdll_pop (&sparenodes);
}

void __blockdll_alloc_spare_nodes (unsigned int n)
{
    struct blocknode*   tmp;
    unsigned long       snode   = sizeof (struct blocknode);
    
    while (n--) {
        tmp = (struct blocknode*)malloc (snode);
        memset (tmp, 0, snode);
        
        __blockdll_push (&sparenodes, tmp);
        totmemalloc += snode;
    }
}

void __blockdll_dump (struct blockdll* dll, FILE* stream)
{
    struct blocknode*   np = dll->head;
    
    while (np) {
        __block_dump(np->ref, stream);
        np = np->next;
    }
}

unsigned long __blockdll_destroy (struct blockdll* dll)
{
    struct blocknode*   todel;
    unsigned long       freed=0;
    unsigned int        snode = sizeof (struct blocknode);
    
    while (dll->head) {
        todel       = __blockdll_pop (dll);
        free (todel);
        
        totmemalloc -= snode;
        freed       += snode;
    }
    
    return freed;
}
