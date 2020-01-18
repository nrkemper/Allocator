//
//  chunkdll.c
//  Allocator Attempt 2
//
//  Created by Nicholas Kemp on 2020-01-18.
//  Copyright © 2020 Nicholas Kemp. All rights reserved.
//

#include <stdlib.h>
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
}

//Returns head. DOESN'T delete
struct chunknode* __chunkdll_pop (struct chunkdll* dll)
{
    struct chunknode*   head = dll->head;
    
    if (head) {
        dll->head           = head->next;
        
        if (dll->head)
            dll->head->prev = (struct chunknode*)0;
    }
    
    return head;
}

void __chunkdll_dump (struct chunkdll* dll, FILE* stream)
{
    struct chunknode*   np = dll->head;
    
    while (np) {
        __chunk_dump (np->ref, stream);
        np = np->next;
    }
}

unsigned int __chunkdll_destroy (struct chunkdll* dll)
{
    struct chunknode*   todel;
    unsigned int        i=0;
    
    while (dll->head) {
        todel = __chunkdll_pop (dll);
        free (todel);
        i++;
    }
    
    return i;
}
