#include <stdio.h>
#include <stdlib.h>

#include "panic.h"

void panic(void) {
    printf("PANIC!\n");
    exit(1);
}
