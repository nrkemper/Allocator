//
//  blockdll.h
//  Allocator Attempt 2
//
//  Created by Nicholas Kemp on 2020-01-18.
//  Copyright © 2020 Nicholas Kemp. All rights reserved.
//

#ifndef __BLOCKDLL_H__
#define __BLOCKDLL_H__

#define SPARE_NODE_INCREASE_AMOUNT      20

#include <stdio.h>

struct blocknode {
    struct blocknode*   next;
    struct blocknode*   prev;
    struct blockdll*    dll;
    struct block*       ref;
};

struct blockdll {
    struct blocknode*   head;
    struct blocknode*   tail;
};

void __blockdll_push (struct blockdll* dll, struct blocknode* b);
void __blockdll_append (struct blockdll* dll, struct blocknode* b);
void __blockdll_insert (struct blockdll* dll, struct blocknode* des, struct blocknode* b);
struct blocknode* __blockdll_pop (struct blockdll* dll);
struct blocknode* __blockdll_remove (struct blocknode* b);
struct blocknode* __blockdll_grab_spare_node (void);
void __blockdll_alloc_spare_nodes (unsigned int n);
void __blockdll_dump (struct blockdll* dll, FILE* stream);
unsigned long __blockdll_destroy (struct blockdll* dll);
#endif /* __BLOCKDLL_H__ */
