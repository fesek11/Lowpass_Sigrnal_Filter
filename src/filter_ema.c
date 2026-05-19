#include "filter.h"

static double y;
static int have_y;

void filter_init(void)
{
    y = 0.0;
    have_y = 0;
}

double filter_push(double x)
{
    if (!have_y) {
        y = x;
        have_y = 1;
        return y;
    }

    y = EMA_ALPHA * x + (1.0 - EMA_ALPHA) * y;
    return y;
}
