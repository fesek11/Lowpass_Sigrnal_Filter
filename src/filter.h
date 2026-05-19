#ifndef FILTER_H
#define FILTER_H

#define MA_WINDOW      4
#define EMA_ALPHA      0.20
#define MEDIAN_WINDOW  7
#define SG_WINDOW      7

void filter_init(void);
double filter_push(double x);

#endif
