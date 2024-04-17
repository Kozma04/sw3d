#include "utils.h"


static clock_t start;


void util_init() {
    start = clock();
}

uint32_t time_ms() {
    clock_t end = clock();
    return (end - start) * 1000 / CLOCKS_PER_SEC;
}
