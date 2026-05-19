#include "filter.h"

#include <stddef.h>

static double buf[MA_WINDOW];
static size_t count;
static size_t idx;
static double sum;

void filter_init(void)
{
    count = 0;
    idx = 0;
    sum = 0.0;
}

double filter_push(double x)
{
    if (count < MA_WINDOW) {
        buf[count] = x;
        sum += x;
        ++count;
        return sum / (double)count;
    }

    sum -= buf[idx];
    buf[idx] = x;
    sum += x;
    idx = (idx + 1U) % MA_WINDOW;
    return sum / (double)MA_WINDOW;
}
