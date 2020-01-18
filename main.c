//
//  main.c
//  Allocator Attempt 2
//
//  Created by Nicholas Kemp on 2020-01-17.
//  Copyright © 2020 Nicholas Kemp. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sys.h"

#include "chunk.h"
#include "chunkdll.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    
    struct chunkdll     dll;
    struct chunknode*   tmp;
    struct chunk        chunks[20];
    char                name[17];
    int                 i;
    
    memset (chunks, 0, sizeof (struct chunk) * 20);
    
    for (i=0; i<20; i++) {
        chunks[i].id = i;
        
        memset(name, 0, 17);
        sprintf (name, "CHUNK %d", i);
        memcpy (chunks[i].name, name, 16);
        
        tmp         = (struct chunknode*)malloc (sizeof (struct chunknode));
        tmp->ref    = &chunks[i];
        
        __chunkdll_push (&dll, tmp);
    }
    
    __chunkdll_dump (&dll, stdout);
    
    Sys_Printf("TRANSVERSING FORWARDS\n");
    for (tmp=dll.head,i=0; tmp; tmp=tmp->next, i++);
    Sys_Printf("Transversed %d nodes\n", i);
    Sys_Printf("TRANSVERSING BACKWARDS\n");
    for (tmp=dll.tail, i=0; tmp; tmp=tmp->prev, i++);
    Sys_Printf("Transversed %d nodes\n", i);
    Sys_Printf ("Destroyed %u nodes\n", __chunkdll_destroy (&dll));
    return 0;
}
