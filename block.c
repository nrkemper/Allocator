//
//  block.c
//  Allocator Attempt 2
//
//  Created by Nicholas Kemp on 2020-01-18.
//  Copyright © 2020 Nicholas Kemp. All rights reserved.
//

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "block.h"
#include "memzone.h"
#include "sys.h"

bool __block_fixed_partition (struct chunk* c, unsigned int size)
{
    struct block*       b;
    uint32_t            total_size;
    struct blocknode*   tmp_node;
    
    size        = size + calculate_padding (size);
    total_size  = sizeof (struct block)
                + calculate_padding (sizeof (struct block))
                + size;
    
    if (!c->fixed)
        return false;
    
    if (total_size > c->free_space)
        return false;
    
    if (c->last_block) {
        struct block*   last_block = c->last_block;
        
        b                   = (struct block*)((char*)last_block + last_block->total_size);
        last_block->next    = b;
        b->prev             = last_block;
    } else {
        b           = (struct block*)((char*)c
                                      + sizeof (struct chunk)
                                      + calculate_padding (sizeof (struct chunk)));
        b->prev     = (struct block*)0;
    }

    c->last_block       = b;
    memset (b, 0, sizeof (struct block));
    b->parent       = c;
    b->next         = (struct block*)0;
    b->id           = memid++;
    b->size         = size;
    b->total_size   = total_size;
    b->start        = (char*)b + sizeof (struct block)
                                + calculate_padding(sizeof (struct block));
    b->end          = (char*)b + b->total_size - 1;
    
    //FIXME: need error handling for this
    tmp_node = __blockdll_grab_spare_node();
    if (tmp_node == (struct blocknode*)0)
        return false;
    
    b->nodes[SORTED_BY_ADDR] = tmp_node;
    tmp_node->ref   = b;
    
    __blockdll_append (&c->blocks_by_addr, tmp_node);
    
    //FIXME: need error handling for this
    tmp_node = __blockdll_grab_spare_node();
    if (tmp_node == (struct blocknode*)0)
        return false;
    
    b->nodes[SORTED_BY_SIZE] = tmp_node;
    tmp_node->ref   = b;
    
    __blockdll_push (&c->blocks_by_size, tmp_node);
    
    c->free_space   -= total_size;
    return true;
}

bool __block_partition (struct chunk* c, void* offset, unsigned int size)
{
    unsigned int        total_size;
    struct block*       b;
    struct blocknode*   tmp_node;
    
    size        += calculate_padding(size);
    total_size  = sizeof (struct block) + calculate_padding(sizeof (struct block)) + size;
    offset      = align_address(offset);
    
    if (c->fixed)
        return false;
    
    if (offset < c->start || (void*)((char*)offset + total_size) > c->end)
        return false;
    
    //FIXME: quick search
    for (tmp_node=c->blocks_by_addr.head; tmp_node; tmp_node=tmp_node->next) {
        if (offset >= (void*)tmp_node->ref && offset <= tmp_node->ref->end)
            return false;
    }
    
    b                   = offset;
    memset (b, 0, sizeof (struct block));
    b->parent           = c;
    b->id               = memid++;
    b->start            = (char*)b + sizeof (struct block);
    b->end              = (char*)b + total_size - 1;
    b->size             = size;
    b->total_size       = total_size;
    b->next = b->prev   = (struct block*)0;
    
    //FIXME: need error handling here
    tmp_node = __blockdll_grab_spare_node ();
    if (tmp_node == (struct blocknode*)0)
        return false;
    
    tmp_node->ref   = b;
    b->nodes[SORTED_BY_ADDR] = tmp_node;
    __blockdll_push (&c->blocks_by_addr, tmp_node);
    
    tmp_node = __blockdll_grab_spare_node();
    if (tmp_node == (struct blocknode*)0)
        return false;
    
    tmp_node->ref   = b;
    b->nodes[SORTED_BY_SIZE] = tmp_node;
    __blockdll_push (&c->blocks_by_size, tmp_node);
    
    c->num_free_blocks++;
    
    return true;
}

struct block* __block_split (struct block* b, void* break_point)
{
    struct block*       new_block;
    struct blocknode*   tmp_node;
    struct chunk*       parent=b->parent;
    uint32_t            b1_size, b1_tot_size, b2_size, b2_tot_size;
    
    break_point = align_address(break_point);
    
    if (b->parent->fixed)
        return (struct block*)0;
    
    if (break_point < b->start ||
        break_point > b->end)
        return (struct block*)0;
    
    b2_tot_size     = (uint32_t)(b->end - break_point);
    b2_size         = b2_tot_size - sizeof (struct block);
    b1_tot_size     = b->total_size - b2_tot_size;
    b1_size         = b->size - b2_tot_size;
    
    if (b1_size < MIN_BLOCK_SIZE ||
        b2_size < MIN_BLOCK_SIZE)
        return (struct block*)0;
    
    b->size         = b1_size;
    b->total_size   = b1_tot_size;
    
    new_block               = break_point;
    new_block->start        = (struct block*)((char*)new_block + sizeof (struct block));
    new_block->end          = (char*)new_block + b2_tot_size;
    new_block->id           = memid++;
    new_block->parent       = parent;
    new_block->size         = b2_size;
    new_block->total_size   = b2_tot_size;
    new_block->next         = (struct block*)0;
    new_block->prev         = (struct block*)0;
    
    //FIXME: need error handling
    tmp_node = __blockdll_grab_spare_node ();
    if (tmp_node == (struct blocknode*)0)
        return (struct block*)0;
    tmp_node->ref   = new_block;
    new_block->nodes[SORTED_BY_ADDR] = tmp_node;
    __blockdll_push (&parent->blocks_by_addr, tmp_node);
    
    //FIXME: need error handling here
    tmp_node = __blockdll_grab_spare_node ();
    if (tmp_node == (struct blocknode*)0)
        return (struct block*)0;
    tmp_node->ref                       = new_block;
    new_block->nodes[SORTED_BY_SIZE]    = tmp_node;
    __blockdll_push (&parent->blocks_by_size, tmp_node);
    
    return new_block;
}

bool __block_combine (struct block* b1, struct block* b2)
{
    if (b1->parent != b2->parent)
        return false;
 
    if (b1->parent->fixed)
        return false;
    
    if ((void*)((char*)b1->end + 1) != b2 &&
        (void*)((char*)b2->end + 1) != b1)
        return false;
    
    b1->total_size  += b2->total_size;
    b1->end         = b2->end;;
    b1->size        += b2->size;

    __block_free (b2);
    return true;
}

bool __block_free (struct block* b)
{
    struct chunk*   parent = b->parent;
    struct blocknode*   tmp_block;
    
    if (parent->fixed)
        return false;
    
    tmp_block = __blockdll_remove (b->nodes[SORTED_BY_ADDR]);
    __blockdll_push (&sparenodes, tmp_block);
    
    tmp_block = __blockdll_remove (b->nodes[SORTED_BY_SIZE]);
    __blockdll_push (&sparenodes, tmp_block);
    
    parent->free_space  += b->total_size;
    parent->used_space  -= b->total_size;
    
    return true;
}
void __block_dump (struct block* b, FILE* stream)
{
    Sys_FPrintf (stream, "BLOCK  ID: %u ADDRESS: %p START: %p\n", b->id, b, b->start);
    Sys_FPrintf (stream, "      END: %p SIZE: %u TOTAL SIZE: %u\n", b->end, b->size, b->total_size);
}
