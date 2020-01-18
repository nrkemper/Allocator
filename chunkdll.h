//
//  chunkdll.h
//  Allocator Attempt 2
//
//  Created by Nicholas Kemp on 2020-01-18.
//  Copyright © 2020 Nicholas Kemp. All rights reserved.
//

#ifndef __CHUNKDLL_H__
#define __CHUNKDLL_H__

#include <stdio.h>

struct chunk;

struct chunknode {
    struct chunknode*   next;
    struct chunknode*   prev;
    struct chunk*       ref;
};

struct chunkdll {
    struct chunknode*   head;
    struct chunknode*   tail;
};

void __chunkdll_push (struct chunkdll* dll, struct chunknode* c);
void __chunkdll_append (struct chunkdll* dll, struct chunknode* c);
struct chunknode* __chunkdll_pop (struct chunkdll* dll);//returns head. DOESN'T delete
void __chunkdll_dump (struct chunkdll* dll, FILE* stream);
unsigned int __chunkdll_destroy (struct chunkdll* dll);
#endif /* __CHUNKDLL_H__ */