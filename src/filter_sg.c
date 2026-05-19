#include "filter.h"

#include <stddef.h>

#if SG_WINDOW != 7
#error "SG coefficients are precomputed for SG_WINDOW = 7"
#endif

/* Savitzky-Golay використовує фіксовані коефіцієнти для згладжування вікна з 7 семплів. */
static const double coeff[SG_WINDOW] = {
    -2.0 / 21.0, 3.0 / 21.0, 6.0 / 21.0, 7.0 / 21.0,
    6.0 / 21.0,  3.0 / 21.0, -2.0 / 21.0,
};

static double buf[SG_WINDOW];
static size_t count;
static size_t idx;

void filter_init(void)
{
    count = 0;
    idx = 0;
}

double filter_push(double x)
{
    size_t i;

    if (count < SG_WINDOW) {
        /* Поки повне вікно ще не набране, використовуємо просте середнє як стартовий вихід. */
        double sum = 0.0;

        buf[count] = x;
        ++count;

        for (i = 0; i < count; ++i) {
            sum += buf[i];
        }
        return sum / (double)count;
    }

    buf[idx] = x;
    idx = (idx + 1U) % SG_WINDOW;

    /* Застосовуємо наперед розраховані коефіцієнти до впорядкованого кільцевого буфера. */
    double y = 0.0;
    for (i = 0; i < SG_WINDOW; ++i) {
        y += buf[(idx + i) % SG_WINDOW] * coeff[i];
    }
    return y;
}
