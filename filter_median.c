#include "filter.h"

#include <stddef.h>

static double buf[MEDIAN_WINDOW];
static size_t count;
static size_t idx;

static double median_of(const double *v, size_t n)
{
    double tmp[MEDIAN_WINDOW];
    size_t i;

    for (i = 0; i < n; ++i) {
        tmp[i] = v[i];
    }
    // сортування вставкою
    for (i = 1; i < n; ++i) {
        double key = tmp[i];
        size_t j = i;

        while (j > 0 && tmp[j - 1] > key) {
            tmp[j] = tmp[j - 1];
            --j;
        }
        tmp[j] = key;
    }

    return tmp[n / 2];
}

void filter_init(void)
{
    count = 0;
    idx = 0;
}

double filter_push(double x)
{
    if (count < MEDIAN_WINDOW) {
        buf[count] = x;
        ++count;
        return median_of(buf, count);
    }

    buf[idx] = x;
    idx = (idx + 1U) % MEDIAN_WINDOW;

    double ordered[MEDIAN_WINDOW];
    for (size_t i = 0; i < MEDIAN_WINDOW; ++i) {
        ordered[i] = buf[(idx + i) % MEDIAN_WINDOW];
    }
    return median_of(ordered, MEDIAN_WINDOW);
}
