//
//  main.c
//  Allocator Attempt 2
//
//  Created by Nicholas Kemp on 2020-01-17.
//  Copyright © 2020 Nicholas Kemp. All rights reserved.
//

#include "memzone.h"



int main(int argc, const char * argv[]) {
    __memzone_init();
    __memzone_dump (stdout);
    __memzone_destroy();
    return 0;
}
