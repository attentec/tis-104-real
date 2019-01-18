#include <stdio.h>
#include <stdlib.h>

#include "panic.h"

void panic_handler(const char* file, int line) {
    printf("PANIC!\n\n%s:%d\n", file, line);
    exit(1);
}
