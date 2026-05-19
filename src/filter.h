#ifndef FILTER_H
#define FILTER_H

#define MA_WINDOW      3
#define EMA_ALPHA      0.17
#define MEDIAN_WINDOW  7
#define SG_WINDOW      7

/*
 * Спільний API фільтрів, який використовує main.c.
 * Кожна реалізація фільтра має надати ці дві функції,
 * тому Makefile може перемикати фільтри без зміни коду pipeline.
 */
void filter_init(void);
double filter_push(double x);

#endif
