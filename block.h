//
//  block.h
//  Allocator Attempt 2
//
//  Created by Nicholas Kemp on 2020-01-18.
//  Copyright © 2020 Nicholas Kemp. All rights reserved.
//

#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <stdio.h>
#include <stdint.h>
#include "chunk.h"

#define MIN_BLOCK_SIZE      64

typedef char    bool;

enum blocklists {
    SORTED_BY_ADDR  = 0,
    SORTED_BY_SIZE  = 1
};

struct block {
    struct chunk*       parent;
    struct block*       next;       //used only if fixed
    struct block*       prev;       //used only if fixed
    void*               start;
    void*               end;
    struct blocknode*   nodes[2];   //[0]by_addr [1]by_size
    uint32_t            id;
    uint32_t            size;
    uint32_t            total_size;
};

bool __block_fixed_partition (struct chunk* c, uint32_t size);
bool __block_partition (struct chunk* c, void* offset, uint32_t size);
struct block* __block_split (struct block* b, void* break_point);
bool __block_combine (struct block* b1, struct block* b2);
bool __block_free (struct block* b);
void __block_dump (struct block* b, FILE* stream);
#endif /* __BLOCK_H__ */
