#include "filter.h"

#include <stddef.h>

/* Moving average зберігає останні MA_WINDOW семплів у кільцевому буфері. */
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
        /* На старті усереднюємо тільки ті семпли, які вже доступні. */
        buf[count] = x;
        sum += x;
        ++count;
        return sum / (double)count;
    }

    /* Замінюємо найстаріший семпл і тримаємо суму для швидкого усереднення. */
    sum -= buf[idx];
    buf[idx] = x;
    sum += x;
    idx = (idx + 1U) % MA_WINDOW;
    return sum / (double)MA_WINDOW;
}
