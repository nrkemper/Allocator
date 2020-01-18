//
//  blockdll.c
//  Allocator Attempt 2
//
//  Created by Nicholas Kemp on 2020-01-18.
//  Copyright © 2020 Nicholas Kemp. All rights reserved.
//

#include <stdlib.h>
#include "blockdll.h"
#include "block.h"

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
}

struct blocknode* __blockdll_pop (struct blockdll* dll)
{
    struct blocknode*   head = dll->head;
    
    if (head) {
        dll->head           = dll->head->next;
        
        if (dll->head)
            dll->head->prev = (struct blocknode*)0;
    }
    
    return head;
}

void __blockdll_dump (struct blockdll* dll, FILE* stream)
{
    struct blocknode*   np = dll->head;
    
    while (np) {
        __block_dump(np->ref, stream);
        np = np->next;
    }
}

unsigned int __blockdll_destroy (struct blockdll* dll)
{
    struct blocknode*   todel;
    unsigned int    i=0;
    
    while (dll->head) {
        todel = __blockdll_pop (dll);
        free (todel);
        i++;
    }
    
    return i;
}
