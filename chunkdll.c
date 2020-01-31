//
//  chunkdll.c
//  Allocator Attempt 2
//
//  Created by Nicholas Kemp on 2020-01-18.
//  Copyright © 2020 Nicholas Kemp. All rights reserved.
//

#include <stdlib.h>
#include "memzone.h"
#include "chunkdll.h"
#include "chunk.h"

void __chunkdll_push (struct chunkdll* dll, struct chunknode* c)
{
    struct chunknode*   head = dll->head;
    
    if (head) {
        head->prev  = c;
        c->next     = head;
        dll->head   = c;
    } else {
        dll->head   = dll->tail = c;
        c->next     = (struct chunknode*)0;
    }
    
    c->prev         = (struct chunknode*)0;
    c->dll          = dll;
}

void __chunkdll_append (struct chunkdll* dll, struct chunknode* c)
{
    struct chunknode*   tail = dll->tail;
    
    if (tail) {
        tail->next  = c;
        c->prev     = tail;
        dll->tail   = c;
    } else {
        dll->head   = dll->tail = c;
        c->next     = (struct chunknode*)0;
    }
    
    c->next         = (struct chunknode*)0;
    c->dll          = dll;
}

void __chunkdll_insert (struct chunknode* des, struct chunknode* c)
{
    
    if (c == (struct chunknode*)0)
        return;
}

//Returns head. DOESN'T delete
struct chunknode* __chunkdll_pop (struct chunkdll* dll)
{
    struct chunknode*   head = dll->head;
    
    if (head) {
        dll->head           = head->next;
        head->dll           = (struct chunkdll*)0;
        
        if (dll->head)
            dll->head->prev = (struct chunknode*)0;
    }
    
    head->dll       = (struct chunkdll*)0;
    
    return head;
}

struct chunknode* __chunkdll_remove (struct chunknode* c)
{

    return (struct chunknode*)0;
}

void __chunkdll_dump (struct chunkdll* dll, FILE* stream)
{
    struct chunknode*   np = dll->head;
    
    while (np) {
        __chunk_dump (np->ref, stream);
        np = np->next;
    }
}

unsigned long __chunkdll_destroy (struct chunkdll* dll)
{
    struct chunknode*   todel;
    unsigned long       freed = 0;
    unsigned int        snode = sizeof (struct chunknode);
    
    while (dll->head) {
        todel       = __chunkdll_pop (dll);
        //freed       += __blockdll_destroy (&todel->ref->blocks);
        //totmemalloc -= snode;
        freed       += snode;
        
        free (todel);
    }
    
    return freed;
}
