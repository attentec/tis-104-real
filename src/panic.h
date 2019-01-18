#pragma once

#define panic() \
    panic_handler(__FILE__, __LINE__)

_Noreturn void panic_handler(const char *file, int line);
