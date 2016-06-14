#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <time.h>

int get_rand(int l, int r) {
    return (rand() ^ (long long)rand() * (clock())) % (r - l) + l;
}
#endif //UTIL_H
