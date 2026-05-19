#include "filter.h"

/* EMA зберігає тільки попередній вихід і згладжує кожен новий семпл. */
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
        /* Перший семпл ініціалізує фільтр без зміни значення. */
        y = x;
        have_y = 1;
        return y;
    }

    /* EMA_ALPHA визначає, наскільки сильно новий семпл впливає на вихід. */
    y = EMA_ALPHA * x + (1.0 - EMA_ALPHA) * y;
    return y;
}
